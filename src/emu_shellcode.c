
#include "emu/emu_shellcode.h"

#include "emu/emu.h"
#include "emu/emu_memory.h"
#include "emu/emu_cpu.h"
#include "emu/emu_cpu_data.h"
#include "emu/emu_track.h"
#include "emu/emu_source.h"
#include "emu/emu_getpc.h"
#include "emu/environment/win32/emu_env_w32.h"
#include "emu/environment/win32/emu_env_w32_dll_export.h"
#include "emu/emu_hashtable.h"
#include "emu/emu_graph.h"

#define STATIC_OFFSET 0x00471000
#define EMU_SHELLCODE_TEST_MAX_STEPS 128


int32_t emu_shellcode_run_and_track(struct emu *e, struct emu_track_and_source *et, struct emu_env_w32 *env, uint32_t steps)
{
	int ret = -1;
	int track = 0;
	int j;
	/* run the code */

	struct emu_vertex *last_vertex = NULL;
	struct emu_vertex *ev          = NULL;
	struct emu_hashtable_item *ehi = NULL;

	struct emu_cpu *cpu = emu_cpu_get(e);

	et->run_instr_graph = emu_graph_new();
	et->run_instr_table = emu_hashtable_new(steps, emu_source_and_track_instr_info_hash,  emu_source_and_track_instr_info_cmp);


	for ( j=0;j<steps;j++ )
	{
		uint32_t eipsave = emu_cpu_eip_get(emu_cpu_get(e));

		struct emu_env_w32_dll_export *dllhook = NULL;

		ret = 0;
		eipsave = emu_cpu_eip_get(emu_cpu_get(e));

		dllhook = emu_env_w32_eip_check(env);


		if ( (ehi = emu_hashtable_search(et->run_instr_table, (void *)eipsave)) == NULL )
		{
			ev = emu_vertex_new();
			emu_graph_vertex_add(et->run_instr_graph, ev);
			emu_hashtable_insert(et->run_instr_table, (void *)eipsave, ev);
		}else
		{
			ev = (struct emu_vertex *)ehi->value;
		}

		if ( last_vertex != NULL )
		{
			struct emu_edge *ee = emu_vertex_edge_add(last_vertex, ev);

			if ( cpu->instr.is_fpu == 0 && cpu->instr.cpu.source.cond_pos == eipsave && cpu->instr.cpu.source.has_cond_pos == 1)
				ee->data = (void *)0x1;
		}

		last_vertex = ev;


		if ( dllhook != NULL )
		{
			if (ev->data == NULL)
			{
				struct emu_source_and_track_instr_info *esatii = emu_source_and_track_instr_info_new(cpu, eipsave);
				free(esatii->instrstring);
				esatii->instrstring = strdup(dllhook->fnname);
				ev->data = esatii;
			}

		}
		else
		{

			ret = emu_cpu_parse(emu_cpu_get(e));
			if ( ret != -1 )
			{
				ret = emu_cpu_step(emu_cpu_get(e));
				if (ev->data == NULL)
				{
					struct emu_source_and_track_instr_info *esatii = emu_source_and_track_instr_info_new(cpu, eipsave);
					ev->data = esatii;
				}
			}


			if ( ret != -1 )
			{
				track = emu_track_instruction_check(e, et);
				if ( track == -1 )
				{
					printf("tracking complained\n");
					break;
				}
			}

			if ( ret == -1 )
			{
				printf("cpu error %s\n", emu_strerror(e));
				break;
			}
		}

	}

	printf("stepcount %i\n",j);
	return j;
}



int32_t emu_shellcode_test(struct emu *e, uint8_t *data, uint16_t size)
{
	struct emu_cpu *cpu = emu_cpu_get(e);
	struct emu_memory *mem = emu_memory_get(e);
	struct emu_env_w32 *env = emu_env_w32_new(e);
	struct emu_track_and_source *et = emu_track_and_source_new();

	uint32_t offset;
	bool found_good_candidate_after_getpc = true;

	uint32_t best_eip=0;

	for ( offset=0; offset<size && found_good_candidate_after_getpc == false; offset++ )
	{

		if ( emu_getpc_check(e, (uint8_t *)data, size, offset) == 1 )
		{
			int failed = 0;


			int j=0;

			/* set the registers to the initial values */
			for ( j=0;j<8;j++ )
				emu_cpu_reg32_set(cpu,j ,0x0);

			/* set the flags */
			emu_cpu_eflags_set(cpu,0x0);

			/* write the code to the offset */
			for ( j = 0; j < size; j++ )
            	emu_memory_write_byte(mem, STATIC_OFFSET+j, data[j]);
			



			/* set eip to the getpc code */
			emu_cpu_eip_set(emu_cpu_get(e), STATIC_OFFSET+offset);

			int ret = -1;
			int track = 0;
			int stepped_steps = 0;

			/* run the code */
			for ( j=0;j<EMU_SHELLCODE_TEST_MAX_STEPS;j++ )
			{
				uint32_t eipsave = emu_cpu_eip_get(emu_cpu_get(e));

				struct emu_env_w32_dll_export *dllhook = NULL;

				ret = 0;
				eipsave = emu_cpu_eip_get(emu_cpu_get(e));

				dllhook = emu_env_w32_eip_check(env);

				if ( dllhook != NULL )
				{

				}
				else
				{

					ret = emu_cpu_parse(emu_cpu_get(e));

					if ( ret != -1 )
					{
						track = emu_track_instruction_check(e, et);
						if ( track == -1 )
						{
							printf("tracking found uninitialised var\n");
							break;
						}
					}

					if ( ret != -1 )
					{
						ret = emu_cpu_step(emu_cpu_get(e));
					}

					if ( ret == -1 )
					{
						printf("cpu error %s\n", emu_strerror(e));
						break;
					}
				}

			}

			printf("stepcount %i\n",j);
			stepped_steps = j;

			

			if ( track == -1 )
			{
				/* rewrite the shellcode */
				for ( j = 0; j < size; j++ )
					emu_memory_write_byte(mem, STATIC_OFFSET+j, data[j]);


				/* recreate the env */
				emu_env_w32_free(env);
				env = emu_env_w32_new(e);


				/* set memory read only so instructions can't change it*/
				emu_memory_mode_ro(emu_memory_get(e));
				emu_source_instruction_graph_create(e, et, STATIC_OFFSET, size);
				emu_memory_mode_rw(emu_memory_get(e));


				
				struct emu_vertex *ev;
				struct emu_hashtable_item *ehi = emu_hashtable_search(et->static_instr_table, (void *)(STATIC_OFFSET+offset));
				if ( ehi != NULL )
				{
					/* static instruction analysis, try to detect a loop*/
					ev = (struct emu_vertex *)ehi->value;
					if( emu_graph_loop_detect(et->static_instr_graph, ev) == false)
					{
						printf("NO LOOP DETECTED (static)\n");
					}else
					{
						printf("LOOP DETECTED (static)\n");
					}

					/* static backwards analysis, binary backwards traversal using breath first search */
					ev = (struct emu_vertex *)ehi->value;
					emu_source_backward_bfs(et, ev);

					for ( ev = emu_vertexes_first(et->static_instr_graph->vertexes); 
						!emu_vertexes_attail(ev) && found_good_candidate_after_getpc == false; 
						ev = emu_vertexes_next(ev) )
					{
						if ( ev->color == green )
						{
							printf("POSSIBLE\n");
							struct emu_source_and_track_instr_info *etii = (struct emu_source_and_track_instr_info *)ev->data;

							for ( j=0;j<8;j++ )
								emu_cpu_reg32_set(cpu,j ,0x0);

							emu_cpu_eflags_set(cpu,0x0);

							for ( j = 0; j < size; j++ )
								emu_memory_write_byte(mem, STATIC_OFFSET+j, data[j]);

							emu_env_w32_free(env);
							env = emu_env_w32_new(e);
							uint32_t dist = emu_graph_distance(et->static_instr_graph, ev, (struct emu_vertex *)ehi->value);

							printf("step distance new_eip -> old_eip: %i\n", dist);
							printf("step distance old_eip -> b0rked : %i\n", stepped_steps);

							best_eip = etii->eip;
							emu_cpu_eip_set(emu_cpu_get(e), etii->eip);

							if ( emu_shellcode_run_and_track(e, et, env, MAX((dist + stepped_steps) * 2, 256) ) >= dist + stepped_steps )
							{
								struct emu_vertex *x;
								struct emu_hashtable_item *y = emu_hashtable_search(et->run_instr_table, (void *)etii->eip);

								if ( y != NULL )
								{
									x = (struct emu_vertex *)y->value;

									if ( emu_graph_loop_detect(et->run_instr_graph, x) == false )
									{
										printf("NO LOOP DETECTED (runtime)\n");
									}
									else
									{
										printf("LOOP DETECTED (runtime)\n");
										found_good_candidate_after_getpc = true;
									}
								}
							}
						}
					}
				}
			}

			/* bail out on *any* error */
			if ( failed != 0 )
				return -1;
		}
	}

	if ( found_good_candidate_after_getpc == true )
	{
		printf("GREAT\n");
	}

	if (found_good_candidate_after_getpc == true)
		return best_eip;
	else
		return -1;
}


        production -- prod_production 
        production -- prod_title 
        production -- prod_runtime 
        production -- prod_year 
        production -- production_ubermenge 
        movie -- production_ubermenge 
        movie -- movie_type 
        series -- ser_series 
        series -- ser_title 
        series -- ser_year 
        episode -- production_ubermenge 
        episode -- episode_from_series -- series 
        episode -- epi_season 
        episode -- epi_episode 
        episode -- epi_air_date 
        genre -- gen_genre 
        genre -- production_genre -- production 
        release -- production_release -- production  
        release -- rel_country 
        release -- rel_year 
        release -- rel_month 
        release -- rel_day 
        link -- lin_link  
        link -- production [label="production1"]
        production -- link [label="production2"]
        person -- pers_person 
        person -- pers_name 
        person -- pers_real_name 
        person -- pers_sex 
        person -- pers_height 
        person -- pers_birth_date 
        person -- pers_birth_country 
        person -- pers_birth_location 
        person -- pers_death_date 
        person -- pers_death_country 
        person -- pers_death_location 
        works -- wor_credit_pos 
        works -- wor_job 
        works -- works_person -- person 
        works -- works_production -- production 
        plays -- pla_role 
        plays -- plays_person -- person 
        plays -- plays_production -- production 

