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

#define HAVE_GETOPT_H
#ifdef HAVE_GETOPT_H
# include <getopt.h>
#endif

#ifdef HAVE_STDLIB_H
# include <stdlib.h>
#endif


#include <stdint.h>

#define HAVE_UNISTD
#ifdef HAVE_UNISTD
# include <unistd.h>
#endif
#include <stdio.h>

#include <stdarg.h>


#include "emu/emu.h"
#include "emu/emu_memory.h"
#include "emu/emu_cpu.h"
#include "emu/emu_log.h"
#include "emu/emu_cpu_data.h"
#include "emu/emu_cpu_stack.h"
#include "emu/environment/emu_profile.h"
#include "emu/environment/emu_env.h"
#include "emu/environment/win32/emu_env_w32.h"
#include "emu/environment/win32/emu_env_w32_dll.h"
#include "emu/environment/win32/emu_env_w32_dll_export.h"
#include "emu/environment/win32/env_w32_dll_export_kernel32_hooks.h"
#include "emu/environment/linux/emu_env_linux.h"
#include "emu/emu_getpc.h"
#include "emu/emu_graph.h"
#include "emu/emu_string.h"
#include "emu/emu_hashtable.h"

#include "emu/emu_shellcode.h"

#include "dot.h"
#include "options.h"

struct instr_vertex *instr_vertex_new(uint32_t theeip, const char *instr_string)
{
	struct instr_vertex *iv = (struct instr_vertex *)malloc(sizeof(struct instr_vertex));
	memset(iv, 0, sizeof(struct instr_vertex));
	iv->eip = theeip;
	iv->instr_string = emu_string_new();
	emu_string_append_format(iv->instr_string, "0x%08x %s\\l",iv->eip, instr_string);
	return iv;
}

void instr_vertex_free(struct instr_vertex *iv)
{
	emu_string_free(iv->instr_string);
	free(iv);
}

struct instr_vertex *instr_vertex_copy(struct instr_vertex *from)
{
	struct instr_vertex *iv = (struct instr_vertex *)malloc(sizeof(struct instr_vertex));
	memset(iv, 0, sizeof(struct instr_vertex));
	iv->eip = from->eip;
	iv->instr_string = emu_string_new();
	iv->dll = from->dll;
	iv->syscall = from->syscall;

	emu_string_append_char(iv->instr_string, from->instr_string->data);
	return iv;
}



void instr_vertex_destructor(void *data)
{
	instr_vertex_free((struct instr_vertex *)data);
}



int graph_draw(struct emu_graph *graph)
{
	struct emu_vertex *ev;
	struct instr_vertex *iv;

	FILE *f = fopen(opts.graphfile,"w+");

	struct emu_graph *sgraph = emu_graph_new();
	struct emu_hashtable *ht = emu_hashtable_new(2047, emu_hashtable_ptr_hash, emu_hashtable_ptr_cmp);

	struct emu_vertex *nev;
	struct instr_vertex *niv=NULL;

	printf("copying vertexes\n");
	for ( ev = emu_vertexes_first(graph->vertexes); !emu_vertexes_attail(ev); ev = emu_vertexes_next(ev) )
	{
		iv = (struct instr_vertex *)ev->data;

		nev = emu_vertex_new();
		emu_graph_vertex_add(sgraph, nev);

		niv = instr_vertex_copy(iv); 
		nev->data = niv;

		emu_hashtable_insert(ht, (void *)iv, nev);
		ev->color = white;
	}

	printf("optimizing graph\n");
	for ( ev = emu_vertexes_first(graph->vertexes); !emu_vertexes_attail(ev); ev = emu_vertexes_next(ev) )
	{
		// ignore known 
		if ( ev->color == black )
			continue;


		printf("vertex %p\n", (void *)ev);

		// find the first in a chain
		iv = (struct instr_vertex *)ev->data;
		while ( emu_edges_length(ev->backedges) == 1 && emu_edges_length(ev->edges) <= 1 && ev->color == white && iv->dll == NULL && iv->syscall == NULL )
		{
			ev->color = grey;

			struct emu_vertex *xev = emu_edges_first(ev->backedges)->destination;
			iv = (struct instr_vertex *)xev->data;
			if ( emu_edges_length(xev->backedges) > 1 || emu_edges_length(xev->edges) > 1 || iv->dll != NULL || iv->syscall != NULL )
				break;

			ev = xev;
			printf(" -> vertex %p\n",(void *)ev);
		}


		iv = (struct instr_vertex *)ev->data;

		// create the new vertex 
		nev = (struct emu_vertex *)emu_hashtable_search(ht, (void *)iv)->value;
		niv = (struct instr_vertex *)nev->data;

		iv = (struct instr_vertex *)ev->data;

		printf("going forwards from %p\n", (void *)ev);
		while ( emu_edges_length(ev->edges) == 1 && emu_edges_length(ev->backedges) <= 1 && ev->color != black && iv->dll == NULL && iv->syscall == NULL )
		{
			ev->color = black;
			struct emu_vertex *xev = emu_edges_first(ev->edges)->destination;
			iv = (struct instr_vertex *)xev->data;

			if ( emu_edges_length(xev->backedges) > 1 || emu_edges_length(xev->edges) > 1 ||
				 iv->dll != NULL || iv->syscall != NULL )
				break;

			ev = xev;

			iv = (struct instr_vertex *)ev->data;
			emu_string_append_char(niv->instr_string, emu_string_char(iv->instr_string));
			printf(" -> vertex %p\n",(void *)ev);
		}

		ev->color = black;

		printf("copying edges for %p\n",(void *)ev);
		struct emu_edge *ee;
		for ( ee = emu_edges_first(ev->edges); !emu_edges_attail(ee); ee = emu_edges_next(ee) )
		{
			struct instr_vertex *ivto = emu_vertex_data_get(ee->destination);
			struct emu_hashtable_item *ehi = emu_hashtable_search(ht, (void *)ivto);
			struct emu_vertex *to = (struct emu_vertex *)ehi->value;
			if ( 1 )// nev != to )//&& to->color != black )
			{
				struct emu_edge *nee = emu_vertex_edge_add(nev, to);
				nee->count = ee->count;
				nee->data = ee->data;
				printf(" -> %p\n", (void *)to);
			}
		}

	}

	graph->vertex_destructor = instr_vertex_destructor;
//	emu_graph_free(graph);
	graph = sgraph;

	emu_hashtable_free(ht);
	fprintf(f, "digraph G {\n\t//rankdir=LR\n\tnode [fontname=Courier, labeljust=r];\n");

#if 0
	int numdlls=0;
	while ( env->loaded_dlls[numdlls] != NULL )
	{
		int has_dll = 0;
		struct emu_string *fs = emu_string_new();
		emu_string_append_format(fs, "\t subgraph cluster%i {\n\t\t node [shape=box, style=filled, color=\".7 .3 1.0\"];\n\t\tstyle=filled;\n\t\tcolor=lightgrey;\n\t\tlabel=\"%s\"\n\t\t", numdlls, env->loaded_dlls[numdlls]->dllname);
		for ( ev = emu_vertexes_first(graph->vertexes); !emu_vertexes_attail(ev); ev = emu_vertexes_next(ev) )
		{
			struct instr_vertex *iv = emu_vertex_data_get(ev);
			if ( iv->dll == env->loaded_dlls[numdlls] )
			{
				emu_string_append_format(fs, "\t\%i [label = \"%s\"];\n", iv->eip, emu_string_char(iv->instr_string));

				has_dll = 1;
			}

		}

		emu_string_append_char(fs, "\t}\n");

		fprintf(f, "%s", emu_string_char(fs));
		numdlls++;
	}
#endif // 0
	for ( ev = emu_vertexes_first(graph->vertexes); !emu_vertexes_attail(ev); ev = emu_vertexes_next(ev) )
	{
		if ( emu_edges_length(ev->edges) == 0 && emu_edges_length(ev->backedges) == 0 )
			continue;

		struct instr_vertex *iv = emu_vertex_data_get(ev);
#if 0
		if ( iv->dll != NULL )
			continue;
#endif // 0
		if ( iv->dll != NULL || iv->syscall != NULL )
			fprintf(f, "\t \"%p\" [shape=box, style=filled, color=\".7 .3 1.0\", label = \"%s\"]\n",(void *)iv, emu_string_char(iv->instr_string));
		else
			fprintf(f, "\t \"%p\" [shape=box, label = \"%s\"]\n",(void *)iv, emu_string_char(iv->instr_string));
	}

	for ( ev = emu_vertexes_first(graph->vertexes); !emu_vertexes_attail(ev); ev = emu_vertexes_next(ev) )
	{
		struct instr_vertex *ivfrom = emu_vertex_data_get(ev);

		struct emu_edge *ee;
		for ( ee = emu_edges_first(ev->edges); !emu_edges_attail(ee); ee = emu_edges_next(ee) )
		{
			struct instr_vertex *ivto = emu_vertex_data_get(ee->destination);
			struct emu_string *fs = emu_string_new();

			if ( ee->data != (void *)0x0 )
				emu_string_append_format(fs, "\t \"%p\" -> \"%p\" [style = dashed", (void *)ivfrom, (void *)ivto);
			else
				emu_string_append_format(fs, "\t \"%p\" -> \"%p\" [style = bold", (void *)ivfrom, (void *)ivto);

			if ( ee->count > 100 )
				emu_string_append_char(fs, ", color=red");
			else
				if ( ee->count > 50 )
				emu_string_append_char(fs, ", color=blue");
			else
				if ( ee->count > 25 )
				emu_string_append_char(fs, ", color=green");
			else
				if ( ee->count > 1 )
				emu_string_append_char(fs, ", color=orange");



			emu_string_append_char(fs, " ]\n");

			fprintf(f, "%s", emu_string_char(fs));
			emu_string_free(fs);
		}

	}


	fprintf(f, "}");
	fclose(f);

	graph->vertex_destructor = instr_vertex_destructor;
	emu_graph_free(sgraph);
//	emu_hashtable_free(eh);

	return 0;
}
