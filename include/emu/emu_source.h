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


#ifndef HAVE_EMU_SOURCE_H
#define HAVE_EMU_SOURCE_H

struct emu_track_and_source;
struct emu_vertex;


/**
 * Create the callgraph of the shellcode being stored in the emu memory.
 * 
 * @param e         the emu
 * @param es        the emu_source_and_track struct which stores the graph
 * @param datastart where to start
 * @param datasize  where to stop
 * 
 * @return 
 */
uint32_t emu_source_instruction_graph_create(struct emu *e, struct emu_track_and_source *es, uint32_t datastart, uint32_t datasize);


void emu_source_forward_bfs(struct emu_track_and_source *et, struct emu_vertex *from);
void emu_source_backward_bfs(struct emu_track_and_source *et, struct emu_vertex *ev);

#endif
