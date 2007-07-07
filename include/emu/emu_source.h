/* @header@ */

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
