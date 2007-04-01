/* @header@ */



#include "emu/emu_graph.h"


source_list_functions(emu_vertexes,emu_vertex_root, emu_vertex, link);

source_list_functions(emu_edges,emu_edge_root, emu_edge, link);


struct emu_edge *emu_edge_new()
{
	struct emu_edge *ee = (struct emu_edge *)malloc(sizeof(struct emu_edge));
	memset(ee, 0, sizeof(struct emu_edge));
	emu_edges_init_link(ee);
	return ee;
}

void emu_edge_free(struct emu_edge *ee)
{
	free(ee);
}


struct emu_vertex *emu_vertex_new()
{
	struct emu_vertex *ev = (struct emu_vertex *)malloc(sizeof(struct emu_vertex));
	memset(ev, 0, sizeof(struct emu_vertex));
	ev->edges = emu_edges_create();
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
	return ee;
}

struct emu_graph *emu_graph_new()
{
	struct emu_graph *eg = (struct emu_graph *)malloc(sizeof(struct emu_graph));
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
	}
	emu_vertexes_destroy(eg->vertexes);
	free(eg);
}

void emu_graph_vertex_add(struct emu_graph *eg, struct emu_vertex *ev)
{
	emu_vertexes_insert_last(eg->vertexes, ev);
}



