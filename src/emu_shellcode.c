/********************************************************************************
 *                               libemu
 *
 *                    - x86 shellcode emulation -
 *
 *
 * Copyright (C) 2007  Paul Baecher & Markus Koetter
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 * 
 * 
 *             contact nepenthesdev@users.sourceforge.net  
 *
 *******************************************************************************/

#include "../config.h"

#include "emu/emu_shellcode.h"

#include "emu/emu.h"
#include "emu/emu_memory.h"
#include "emu/emu_cpu.h"
#include "emu/emu_cpu_data.h"
#include "emu/emu_track.h"
#include "emu/emu_source.h"
#include "emu/emu_getpc.h"
#include "emu/environment/emu_env.h"
#include "emu/environment/win32/emu_env_w32.h"
#include "emu/environment/win32/emu_env_w32_dll_export.h"
#include "emu/emu_hashtable.h"
#include "emu/emu_graph.h"
#include "emu/emu_list.h"
#include "emu/emu_queue.h"

#include "emu/emu_log.h"


#define STATIC_OFFSET 0x00471000
#define EMU_SHELLCODE_TEST_MAX_STEPS 128


bool cmp_uint32_t(void *a, void *b)
{
	if ( (uint32_t)a == (uint32_t)b )
		return true;

	return false;
}

uint32_t hash_uint32_t(void *key)
{
	uint32_t ukey = (uint32_t)key;
	ukey++;
	return ukey;
}


int tested_positions_cmp(struct emu_list_item *a, struct emu_list_item *b)
{
	if ( ((struct emu_stats *)a->data)->cpu.steps > ((struct emu_stats *)b->data)->cpu.steps )
		return 0;
	else
	if ( ((struct emu_stats *)a->data)->cpu.steps == ((struct emu_stats *)b->data)->cpu.steps )
    	if ( ((struct emu_stats *)a->data)->eip <= ((struct emu_stats *)b->data)->eip )
			return 0;

	return 1;
}


int32_t     emu_shellcode_run_and_track(struct emu *e, 
										uint8_t *data, 
										uint16_t datasize, 
										uint16_t eipoffset,
										uint32_t steps,
//										struct emu_env_w32 *env, 
										struct emu_track_and_source *etas,
										struct emu_hashtable *known_positions,
										struct emu_list_root *stats_tested_positions_list,
										bool brute_force
									   )
{
	struct emu_cpu *cpu = emu_cpu_get(e);
	struct emu_memory *mem = emu_memory_get(e);


	struct emu_queue *eq = emu_queue_new();
	emu_queue_enqueue(eq, (void *)((uint32_t)eipoffset+STATIC_OFFSET));

//	struct emu_list_root *tested_positions = emu_list_create();

	struct emu_env *env = NULL;

	while ( !emu_queue_empty(eq) )
	{
		
		uint32_t current_offset = (uint32_t)emu_queue_dequeue(eq);

		/* init the cpu/memory 
		 * scooped to keep number of used varnames small 
		 */
        {
			logDebug(e, "running at offset %i %08x\n", current_offset, current_offset);

			emu_memory_clear(mem);
			if (env)
				emu_env_free(env);

			/* write the code to the offset */
			emu_memory_write_block(mem, STATIC_OFFSET, data, datasize);
			
			env = emu_env_new(e);

			/* set the registers to the initial values */
			int reg;
			for ( reg=0;reg<8;reg++ )
				emu_cpu_reg32_set(cpu,reg ,0x0);

			emu_cpu_reg32_set(cpu, esp, 0x00120000);
			emu_cpu_eip_set(cpu, current_offset);

			/* set the flags */
			emu_cpu_eflags_set(cpu,0x0);
		}

		emu_tracking_info_clear(&etas->track);

		int j;
		for ( j=0;j<steps;j++ )
		{
//			emu_cpu_debug_print(cpu);
			uint32_t eipsave;
			eipsave = emu_cpu_eip_get(cpu);

			struct emu_env_hook *hook = NULL;

			hook = emu_env_w32_eip_check(env);


			if ( hook != NULL )
			{
				if ( hook->hook.win->fnhook == NULL )
					break;
			}
			else
			{

				int32_t ret = emu_cpu_parse(emu_cpu_get(e));
				if ( ret == -1 )
				{
					logDebug(e, "error at %s\n", cpu->instr_string);
					break;
				}

				ret = emu_cpu_step(emu_cpu_get(e));
				if ( ret == -1 )
				{
					logDebug(e, "error at %s (%s)\n", cpu->instr_string, strerror(emu_errno(e)));
					if (brute_force)
					{
						logDebug(e, "goto traversal\n");
						goto traversal;
					}
					else
						break;
				}

				if ( emu_track_instruction_check(e, etas) == -1 )
				{
traversal:
					logDebug(e, "failed instr %s\n", cpu->instr_string);
					logDebug(e, "tracking at eip %08x\n", eipsave);
					if ( 0 && cpu->instr.is_fpu )
					{

					}
					else
					{

						/* save the requirements of the failed instruction */
//						struct emu_tracking_info *instruction_needs_ti = emu_tracking_info_new();
//						emu_tracking_info_copy(&cpu->instr.cpu.track.need, instruction_needs_ti);

						struct emu_queue *bfs_queue = emu_queue_new();

						/*
						 * the current starting point is the first position used to bfs
						 * scooped to avoid varname collisions 
						 */
						{ 
							struct emu_tracking_info *eti = emu_tracking_info_new();
							emu_tracking_info_diff(&cpu->instr.track.need, &etas->track, eti);
							eti->eip = current_offset;
							emu_tracking_info_debug_print(eti);
							emu_queue_enqueue(bfs_queue, eti);
						}

/*						{ // mark all vertexes white

							struct emu_vertex *x;
							for ( x= emu_vertexes_first(etas->static_instr_graph->vertexes); !emu_vertexes_attail(x); x = emu_vertexes_next(x))
							{
								x->color = white;
							}

						}
*/
						while ( !emu_queue_empty(bfs_queue) )
						{
							logDebug(e, "loop %s:%i\n", __FILE__, __LINE__);

							struct emu_tracking_info *current_pos_ti_diff = (struct emu_tracking_info *)emu_queue_dequeue(bfs_queue);
							struct emu_hashtable_item *current_pos_ht = emu_hashtable_search(etas->static_instr_table, (void *)current_pos_ti_diff->eip);
							if (current_pos_ht == NULL)
							{
								logDebug(e, "current_pos_ht is NULL?\n");
								exit(-1);
							}

							struct emu_vertex *current_pos_v = (struct emu_vertex *)current_pos_ht->value;
							struct emu_source_and_track_instr_info *current_pos_satii = (struct emu_source_and_track_instr_info *)current_pos_v->data;

							if (current_pos_v->color == red)
								continue;

							current_pos_v->color = red;

							while ( !emu_tracking_info_covers(&current_pos_satii->track.init, current_pos_ti_diff) || brute_force )
							{
								logDebug(e, "loop %s:%i\n", __FILE__, __LINE__);

								if ( current_pos_v->backlinks == 0 )
								{
									break;
								}
								else
								if ( current_pos_v->backlinks > 1 )
								{ /* queue all to diffs to the bfs queue */
									struct emu_edge *ee;
									struct emu_vertex *ev;
									for ( ee = emu_edges_first(current_pos_v->backedges); !emu_edges_attail(ee); ee=emu_edges_next(ee) )
									{
										ev = ee->destination;
										struct emu_tracking_info *eti = emu_tracking_info_new();
										emu_tracking_info_diff(current_pos_ti_diff, &current_pos_satii->track.init, eti);
										eti->eip = ((struct emu_source_and_track_instr_info *)ev->data)->eip;
										emu_queue_enqueue(bfs_queue, eti);

									}
									/* the new possible positions and requirements got queued into the bfs queue, 
									 *  we break here, so the new queued positions can try to work it out
									 */
									break;
								}
								else
								if ( current_pos_v->backlinks == 1 )
								{ /* follow the single link */

									current_pos_v = emu_edges_first(current_pos_v->backedges)->destination;
									current_pos_satii = (struct emu_source_and_track_instr_info *)current_pos_v->data;
									emu_tracking_info_diff(current_pos_ti_diff, &current_pos_satii->track.init, current_pos_ti_diff);
								}
							}

							if ( emu_tracking_info_covers(&current_pos_satii->track.init, current_pos_ti_diff) || brute_force )
							{
								logDebug(e, "found position which satiesfies the requirements %i %08x\n", current_pos_satii->eip, current_pos_satii->eip);
								emu_tracking_info_debug_print(&current_pos_satii->track.init);
								emu_queue_enqueue(eq, (void *)((uint32_t)current_pos_satii->eip));
							}
							emu_tracking_info_free( current_pos_ti_diff);
						}
						emu_queue_free(bfs_queue);
					}
					/* the shellcode did not run correctly as he was missing instructions initializing required registers
					 * we did what we could do in the prev lines of code to find better offsets to start from
					 * the working offsets got queued into the main queue, so we break here to give them a chance
                     */
					break;
				}else
				{
					logDebug(e, "%s\n", cpu->instr_string);
				}
			}
		}

		struct emu_stats *es = emu_stats_new();
		es->eip = current_offset;
		es->cpu.steps = j;
		struct emu_list_item *eli = emu_list_item_create();
		eli->data = es;
		emu_list_insert_last(stats_tested_positions_list, eli);
	}

	emu_queue_free(eq);


	/* sort all tested positions by the number of steps ascending */
	emu_list_qsort(stats_tested_positions_list, tested_positions_cmp);

	struct emu_list_item *eli = emu_list_first(stats_tested_positions_list);
	struct emu_stats *es = (struct emu_stats *)eli->data;
	uint32_t best_offset = es->eip;

	return best_offset - STATIC_OFFSET;
}

enum
{
	EMU_SCTEST_SUSPECT_GETPC,
	EMU_SCTEST_SUSPECT_MOVFS
} emu_shellcode_suspect;



int32_t emu_shellcode_test(struct emu *e, uint8_t *data, uint16_t size)
{
	logPF(e);
/*

	
	bool found_good_candidate_after_getpc = false;

	uint32_t best_eip=0;
*/
	uint32_t offset;
	struct emu_list_root *el;
	el = emu_list_create();


	for ( offset=0; offset<size ; offset++ )
	{
		if ( emu_getpc_check(e, (uint8_t *)data, size, offset) != 0 )
		{
			logDebug(e, "possible getpc at offset %i (%08x)\n", offset, offset);
			struct emu_list_item *eli = emu_list_item_create();
			eli->uint32 = offset;
			emu_list_insert_last(el, eli);
		}
	}

	if ( emu_list_length(el) == 0 )
	{
		emu_list_destroy(el);
		return -1;
	}

	{
		struct emu_cpu *cpu = emu_cpu_get(e);
		struct emu_memory *mem = emu_memory_get(e);

		/* write the code to the offset */
		emu_memory_write_block(mem, STATIC_OFFSET, data, size);

		/* set the registers to the initial values */
		int reg;
		for ( reg=0;reg<8;reg++ )
			emu_cpu_reg32_set(cpu,reg ,0x0);

		emu_cpu_reg32_set(cpu, esp, 0x00120000);
		emu_cpu_eip_set(cpu, 0);

		/* set the flags */
		emu_cpu_eflags_set(cpu,0x0);
	}

	struct emu_track_and_source *etas = emu_track_and_source_new();

	logDebug(e, "creating static callgraph\n");
	/* create the static analysis graph 
	set memory read only so instructions can't change it*/
	emu_memory_mode_ro(emu_memory_get(e));
	emu_source_instruction_graph_create(e, etas, STATIC_OFFSET, size);
	emu_memory_mode_rw(emu_memory_get(e));

	struct emu_hashtable *eh = emu_hashtable_new(size+4/4, hash_uint32_t, cmp_uint32_t);
	struct emu_list_item *eli;
//	struct emu_env_w32 *env = emu_env_w32_new(e);
	

	struct emu_list_root *results = emu_list_create();

	for ( eli = emu_list_first(el); !emu_list_attail(eli); eli = emu_list_next(eli) )
	{
		logDebug(e, "testing offset %i %08x\n", eli->uint32, eli->uint32);
		emu_shellcode_run_and_track(e, data, size, eli->uint32, 256, etas, eh,
									results, false);
	}

	/* for all positions we got, take the best, maybe take memory access into account later */
	emu_list_qsort(results, tested_positions_cmp);
	if ( ((struct emu_stats *)emu_list_first(results)->data)->cpu.steps != 256 )
	{
		logDebug(e, "brute force!\n");
		struct emu_list_root *new_results = emu_list_create();
		for ( eli = emu_list_first(results); !emu_list_attail(eli); eli = emu_list_next(eli) )
		{
			struct emu_stats *es = (struct emu_stats *)eli->data;
			logDebug(e, "brute at offset 0x%08x \n",es->eip - STATIC_OFFSET);
			emu_shellcode_run_and_track(e, data, size, es->eip - STATIC_OFFSET, 256, etas, eh,
										new_results, true);
			
		}

		emu_list_concat(results, new_results);
		emu_list_qsort(results, tested_positions_cmp);

		/* uniq */
		for ( eli = emu_list_first(results); !emu_list_attail(eli); eli = emu_list_next(eli) )
		{
			struct emu_list_item *next = emu_list_next(eli);
			if (!emu_list_attail(next) &&
            	((struct emu_stats *)eli->data)->eip == ((struct emu_stats *)next->data)->eip )
				emu_list_remove(next);
		}
	}

	

	emu_hashtable_free(eh);
	emu_list_destroy(el);
//	emu_env_w32_free(env);
	emu_track_and_source_free(etas);


	{
		struct emu_list_item *eli;
		for ( eli = emu_list_first(results); !emu_list_attail(eli); eli = emu_list_next(eli) )
		{
			struct emu_stats *es = (struct emu_stats *)eli->data;
			logDebug(e, "b offset 0x%08x steps %i\n",es->eip, es->cpu.steps);
		}
	}

	eli = emu_list_first(results);
	struct emu_stats *es = (struct emu_stats *)eli->data;

	if ( es->cpu.steps > 100 )
	{
		offset = es->eip;
	}
	else
	{
		offset = -1;
	}

	for (eli = emu_list_first(results); !emu_list_attail(eli); eli = emu_list_next(eli))
	{
		emu_stats_free((struct emu_stats *)eli->data);
	}
	emu_list_destroy(results);

	return offset - STATIC_OFFSET;
}

struct emu_stats *emu_stats_new()
{
	struct emu_stats *es = malloc(sizeof(struct emu_stats));
	memset(es, 0, sizeof(struct emu_stats));
	return es;
}

void emu_stats_free(struct emu_stats *es)
{
	free(es);
}

