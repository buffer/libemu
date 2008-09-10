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


#include <string.h>

#include "emu/emu.h"
#include "emu/emu_cpu.h"
#include "emu/emu_cpu_data.h"
#include "emu/emu_instruction.h"
#include "emu/emu_track.h"
#include "emu/emu_source.h"
#include "emu/emu_hashtable.h"
#include "emu/emu_graph.h"
#include "emu/emu_queue.h"

uint32_t emu_source_instruction_graph_create(struct emu *e, struct emu_track_and_source *es, uint32_t datastart, uint32_t datasize)
{
//	printf("tracking from %x to %x\n", datastart, datastart+datasize);
	struct emu_cpu *c = emu_cpu_get(e);

	es->static_instr_graph = emu_graph_new();
	es->static_instr_table = emu_hashtable_new(datasize/2, emu_hashtable_ptr_hash,  emu_hashtable_ptr_cmp);
	es->static_instr_graph->vertex_destructor = emu_source_and_track_instr_info_free_void;

	uint32_t i;
	for (i=datastart;i<datastart+datasize;i++)
	{
		emu_cpu_eip_set(c, i);

		if ( emu_cpu_parse(c) != 0)
		{
//			printf("parse error %s\n", emu_strerror(e));
			continue;
		}

		if ( emu_cpu_step(c) != 0)
		{
//			printf("step error %s\n", emu_strerror(e));
//			continue;
		}

        struct emu_source_and_track_instr_info *etii = emu_source_and_track_instr_info_new(c,i);
		struct emu_vertex *ev = emu_vertex_new();
		ev->data = etii;
		emu_hashtable_insert(es->static_instr_table, (void *)(uintptr_t)i, ev);
		emu_graph_vertex_add(es->static_instr_graph, ev);
	}

	struct emu_vertex *ev;
	for ( ev = emu_vertexes_first(es->static_instr_graph->vertexes); !emu_vertexes_attail(ev); ev = emu_vertexes_next(ev) )
	{
		struct emu_source_and_track_instr_info *etii = (struct emu_source_and_track_instr_info *)ev->data;

		struct emu_hashtable_item *ehi = emu_hashtable_search(es->static_instr_table, (void *)(uintptr_t)etii->source.norm_pos);
//		printf("NORM from %08x to %08x\n",((struct emu_source_and_track_instr_info *)ev->data)->eip, etii->source.norm_pos);
		if (ehi != NULL)
		{
			struct emu_vertex *to = (struct emu_vertex *)ehi->value;
			emu_vertex_edge_add(ev, to);
			
		}else
		{
//			printf("NORM IS UNKNOWN %08x\n", etii->source.norm_pos);
		}

		if (etii->source.has_cond_pos == 1)
		{
//			printf("COND from %08x to %08x\n",((struct emu_source_and_track_instr_info *)ev->data)->eip, etii->source.cond_pos);
			ehi = emu_hashtable_search(es->static_instr_table, (void *)(uintptr_t)etii->source.cond_pos);
			if (ehi != NULL)
			{
				struct emu_vertex *to = (struct emu_vertex *)ehi->value;
				emu_vertex_edge_add(ev, to);
				
			}else
			{
//				printf("COND IS UNKNOWN\n");
			}
		}
	}
	return 0;
}



void emu_source_backward_bfs(struct emu_track_and_source *et, struct emu_vertex *ev)
{
	struct emu_vertex *it;
	for ( it = emu_vertexes_first(et->static_instr_graph->vertexes); !emu_vertexes_attail(it); it = emu_vertexes_next(it) )
		it->color = white;
    
	it = ev;

	struct emu_queue *eq = emu_queue_new();

	emu_queue_enqueue(eq, ev);

	while (emu_queue_empty(eq) == false)
	{
		ev = (struct emu_vertex *)emu_queue_dequeue(eq);

		struct emu_edge *ee;
		for ( ee = emu_edges_first(ev->backedges); !emu_edges_attail(ee); ee = emu_edges_next(ee) )
		{
			if (ee->destination->color != white)
				continue;

			ee->destination->color = grey;
			emu_queue_enqueue(eq, ee->destination);
			
		}

		if ( emu_edges_length(ev->backedges) == 0 )
			ev->color = green;
		else
			ev->color = black;

	}

	emu_queue_free(eq);

	it->color = red;
}

void emu_source_forward_bfs(struct emu_track_and_source *et, struct emu_vertex *ev)
{
	struct emu_vertex *it;
	for ( it = emu_vertexes_first(et->static_instr_graph->vertexes); !emu_vertexes_attail(it); it = emu_vertexes_next(it) )
		it->color = white;

	it = ev;

	struct emu_queue *eq = emu_queue_new();

	emu_queue_enqueue(eq, ev);

	while (emu_queue_empty(eq) == false)
	{
		ev = (struct emu_vertex *)emu_queue_dequeue(eq);

		struct emu_edge *ee;
		for ( ee = emu_edges_first(ev->edges); !emu_edges_attail(ee); ee = emu_edges_next(ee) )
		{
			if (ee->destination->color != white)
				continue;

			ee->destination->color = grey;
			emu_queue_enqueue(eq, ee->destination);

		}

		if ( emu_edges_length(ev->edges) == 0 )
			ev->color = yellow;
		else
			ev->color = black;

	}

	emu_queue_free(eq);

	it->color = red;
}

