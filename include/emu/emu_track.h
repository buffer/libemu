/* @header@ */

#ifndef HAVE_EMU_TRACK_H
#define HAVE_EMU_TRACK_H

#include <stdint.h>
#include <stdbool.h>

struct emu;
struct emu_graph;
struct emu_instruction;
struct emu_source;


struct emu_source_and_track_instr_info
{
	uint32_t eip;
	char *instrstring;

	struct 
	{
		struct 
		{
			uint32_t eflags;
			uint32_t reg[8];
		}init, need;

	} track;

	struct 
	{
		uint8_t has_cond_pos : 1;
		uint32_t norm_pos;
		uint32_t cond_pos;
	} source;
};

struct emu_source_and_track_instr_info *emu_source_and_track_instr_info_new(struct emu_cpu *cpu, uint32_t eip_before_instruction);
void emu_source_track_instr_info_free(struct emu_source_and_track_instr_info *esantii);

bool emu_source_and_track_instr_info_cmp(void *a, void *b);
uint32_t emu_source_and_track_instr_info_hash(void *key);


struct emu_track_and_source
{
	uint32_t eflags;
	uint32_t reg[8];

	struct emu_graph 		*instr_graph;
	struct emu_hashtable 	*instr_table;

	struct emu_source 		*source;
};


struct emu_track_and_source *emu_track_and_source_new();
void emu_track_and_source_free(struct emu_track_and_source *et);

int32_t emu_track_instruction_check(struct emu *e, struct emu_track_and_source *et);

#endif
