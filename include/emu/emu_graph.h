/* @header@ */
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "emu/emu_list.h"

struct emu_vertex;

header_list_typedefs(emu_edge_root,emu_edge,emu_edge_link);

struct emu_edge
{
	struct emu_vertex *destination;
	emu_edge_link link;
	uint32_t count;
};

header_list_functions(emu_edges,emu_edge_root, emu_edge, link);

struct emu_edge *emu_edge_new();
void emu_edge_free(struct emu_edge *ee);


header_list_typedefs(emu_vertex_root,emu_vertex,emu_vertex_link);
header_list_functions(emu_vertexes,emu_vertex_root, emu_vertex, link);

enum emu_vertex_color { white, grey, black };

struct emu_vertex
{
	void *data;
	emu_edge_root  *edges;

	emu_vertex_link link;
	enum emu_vertex_color color;

	uint32_t backlinks;
};


struct emu_vertex *emu_vertex_new();
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

struct emu_graph *emu_graph_new();
void emu_graph_free(struct emu_graph *eg);
void emu_graph_vertex_add(struct emu_graph *eg, struct emu_vertex *ev);




