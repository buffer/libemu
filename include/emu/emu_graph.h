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

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "emu/emu_list.h"

struct emu_vertex;

header_list_typedefs(emu_edge_root,emu_edge,emu_edge_link);

/**
 * The edge connects two vertexes.
 * 
 * The following fields of a emu_vertex are to your own purpose:
 *  - data, attach your own data.
 * 
 * @see emu_vertex
 * @see emu_graph
 */
struct emu_edge
{
	struct emu_vertex *destination;
	emu_edge_link link;
	uint32_t count;
	void *data;
};

header_list_functions(emu_edges,emu_edge_root, emu_edge, link);

struct emu_edge *emu_edge_new(void);
void emu_edge_free(struct emu_edge *ee);


header_list_typedefs(emu_vertex_root,emu_vertex,emu_vertex_link);
header_list_functions(emu_vertexes,emu_vertex_root, emu_vertex, link);




enum emu_color { black, blue, cyan, green, grey, magenta, red, white, yellow };

/**
 * The vertex is a vertex in a graph.
 * It can have edges to other vertexes, 
 * and has backedges from the vertexes where it has edges too.
 * The following fields of the emu_vertex are for your own purposes:
 *  - color, usefull for bfs & dfs
 *  - distance, usefull for distance measurement
 *  - data, attach your own data
 * 
 * @see emu_edge
 * @see emu_graph
 */
struct emu_vertex
{
	void *data;
	emu_edge_root  *edges;

	emu_vertex_link link;
	enum emu_color color;

	emu_edge_root  *backedges;
	uint32_t backlinks;

	uint32_t distance;
};


struct emu_vertex *emu_vertex_new(void);
void emu_vertex_free(struct emu_vertex *ev);
void emu_vertex_data_set(struct emu_vertex *ev, void *data);
void *emu_vertex_data_get(struct emu_vertex *ev);
struct emu_edge *emu_vertex_edge_add(struct emu_vertex *ev, struct emu_vertex *to);

typedef void (*emu_graph_destructor)(void *data);
struct emu_graph
{
	emu_vertex_root 	*vertexes;
	emu_graph_destructor	vertex_destructor;
};

struct emu_graph *emu_graph_new(void);
void emu_graph_free(struct emu_graph *eg);
void emu_graph_vertex_add(struct emu_graph *eg, struct emu_vertex *ev);

bool emu_graph_path_exists(struct emu_graph *eg, struct emu_vertex *from, struct emu_vertex *to);
bool emu_graph_loop_detect(struct emu_graph *eg, struct emu_vertex *from);
int32_t emu_graph_distance(struct emu_graph *eg, struct emu_vertex *from, struct emu_vertex *to);
