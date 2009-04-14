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




#include "emu/emu_graph.h"
#include "emu/emu_queue.h"


source_list_functions(emu_vertexes,emu_vertex_root, emu_vertex, link);

source_list_functions(emu_edges,emu_edge_root, emu_edge, link);


struct emu_edge *emu_edge_new(void)
{
	struct emu_edge *ee = (struct emu_edge *)malloc(sizeof(struct emu_edge));
	if( ee == NULL )
	{
		return NULL;
	}
	memset(ee, 0, sizeof(struct emu_edge));
	emu_edges_init_link(ee);
	return ee;
}

void emu_edge_free(struct emu_edge *ee)
{
	free(ee);
}


struct emu_vertex *emu_vertex_new(void)
{
	struct emu_vertex *ev = (struct emu_vertex *)malloc(sizeof(struct emu_vertex));
	if( ev == NULL )
	{
		return NULL;
	}
	memset(ev, 0, sizeof(struct emu_vertex));
	ev->edges = emu_edges_create();
	ev->backedges = emu_edges_create();
	return ev;
}

void emu_vertex_free(struct emu_vertex *ev)
{
	// FIXME
}

void emu_vertex_data_set(struct emu_vertex *ev, void *data)
{
	ev->data = data;
}

void *emu_vertex_data_get(struct emu_vertex *ev)
{
	return ev->data;
}


struct emu_edge *emu_vertex_edge_add(struct emu_vertex *ev, struct emu_vertex *to)
{

	struct emu_edge *ee;
	for (ee = emu_edges_first(ev->edges); !emu_edges_istail(ee); ee = emu_edges_next(ee))
	{
		if (ee->destination == to)
		{
			ee->count++;
			return ee;
		}
	}

	ee = emu_edge_new();
	ee->destination = to;
	ee->count++;
	to->backlinks++;
	emu_edges_insert_last(ev->edges, ee);

	struct emu_edge *bee;
	int8_t has_edge = 0;
	for (bee = emu_edges_first(to->backedges); !emu_edges_istail(bee); bee = emu_edges_next(bee))
	{
		if (bee->destination == ev)
		{
			has_edge = 1;
			break;
		}
	}

	if (has_edge == 0)
	{
		bee = emu_edge_new();
		bee->destination = ev;
		bee->count++;
		emu_edges_insert_last(to->backedges, bee);
	}

	return ee;
}

struct emu_graph *emu_graph_new(void)
{
	struct emu_graph *eg = (struct emu_graph *)malloc(sizeof(struct emu_graph));
	if( eg == NULL )
	{
		return NULL;
	}
	memset(eg, 0, sizeof(struct emu_graph));
	eg->vertexes = emu_vertexes_create();
	return eg;

}

void emu_graph_free(struct emu_graph *eg)
{
	struct emu_vertex *ev;
	for ( ev = emu_vertexes_first(eg->vertexes); !emu_vertexes_attail(ev); ev = emu_vertexes_next(ev) )
	{
		if (eg->vertex_destructor != NULL)
        	eg->vertex_destructor(ev->data);
		emu_edges_destroy(ev->edges);
		emu_edges_destroy(ev->backedges);
	}
	emu_vertexes_destroy(eg->vertexes);
	free(eg);
}

void emu_graph_vertex_add(struct emu_graph *eg, struct emu_vertex *ev)
{
	emu_vertexes_insert_last(eg->vertexes, ev);
}



bool emu_graph_path_exists(struct emu_graph *eg, struct emu_vertex *from, struct emu_vertex *to)
{
	struct emu_vertex *it;
	struct emu_vertex *ev;
	for ( it = emu_vertexes_first(eg->vertexes); !emu_vertexes_attail(it); it = emu_vertexes_next(it) )
	{
		it->color = white;
		it->distance = 0;
	}

	it = from;

	struct emu_queue *eq = emu_queue_new();

	emu_queue_enqueue(eq, from);

	while ( emu_queue_empty(eq) == false )
	{
		ev = (struct emu_vertex *)emu_queue_dequeue(eq);

		if ( ev == to )
		{
			emu_queue_free(eq);
			return true;
		}


		struct emu_edge *ee;
		for ( ee = emu_edges_first(ev->edges); !emu_edges_attail(ee); ee = emu_edges_next(ee) )
		{
			if ( ee->destination->color != white )
				continue;

			ee->destination->distance = ev->distance + 1;
			ee->destination->color = grey;
			emu_queue_enqueue(eq, ee->destination);
		}

		ev->color = black;
	}

	emu_queue_free(eq);

	return false;
}

/*
#include <stdio.h>
#include "emu/emu_track.h"
*/

bool emu_graph_loop_detect(struct emu_graph *eg, struct emu_vertex *from)
{
	struct emu_vertex *it;
	struct emu_vertex *ev;
	for ( it = emu_vertexes_first(eg->vertexes); !emu_vertexes_attail(it); it = emu_vertexes_next(it) )
		it->color = white;

	it = from;

	struct emu_queue *eq = emu_queue_new();

	emu_queue_enqueue(eq, from);

	while ( emu_queue_empty(eq) == false )
	{
		ev = (struct emu_vertex *)emu_queue_dequeue(eq);

		struct emu_edge *ee;
		for ( ee = emu_edges_first(ev->edges); !emu_edges_attail(ee); ee = emu_edges_next(ee) )
		{
			if ( ee->destination->color != white )
				continue;

			ee->destination->color = grey;
			emu_queue_enqueue(eq, ee->destination);
		}

		ev->color = black;
	}

	for ( it = emu_vertexes_first(eg->vertexes); !emu_vertexes_attail(it); it = emu_vertexes_next(it) )
	{
//		printf("%08x \n\tcolor %i\n\tedges %i\n\tpath %i\n",((struct emu_source_and_track_instr_info *)it->data)->eip, it->color, emu_edges_length(it->edges), (int)emu_graph_path_exists(eg, from, it));

		if (it->color == white)
			continue;

//		if (emu_edges_length(it->edges) < 2)
//			continue;

/*
		if (emu_graph_path_exists(eg, from, it) == false)
			continue;
*/

/*
		printf("%08x => %08x\n", ((struct emu_source_and_track_instr_info *)from->data)->eip, 
			   ((struct emu_source_and_track_instr_info *)it->data)->eip); 
*/
		emu_queue_enqueue(eq, it);
	}

	while ( emu_queue_empty(eq) == false )
	{
		ev = (struct emu_vertex *)emu_queue_dequeue(eq);

		struct emu_edge *ee;
		for ( ee = emu_edges_first(ev->edges); !emu_edges_attail(ee); ee = emu_edges_next(ee) )
		{
			if ( emu_graph_path_exists(eg, ee->destination, ev) == true )
			{
				emu_queue_free(eq);
				return true;
			}
		}
	}

	emu_queue_free(eq);

	return false;
}

int32_t emu_graph_distance(struct emu_graph *eg, struct emu_vertex *from, struct emu_vertex *to)
{
	if ( emu_graph_path_exists(eg, from, to) == false )
		return -1;

	return to->distance;
}
