/* @header@ */

#ifndef HAVE_EMU_TRACK_H
#define HAVE_EMU_TRACK_H

#include <stdint.h>
#include <stdbool.h>

struct emu;
struct emu_cpu;
struct emu_graph;
struct emu_instruction;

struct emu_tracking_info
{
	uint32_t eflags;
	uint32_t reg[8];
	uint32_t eip;
};


struct emu_source_and_track_instr_info
{
	uint32_t eip;
	char *instrstring;

	struct 
	{
		struct emu_tracking_info init;
		struct emu_tracking_info need;
	} track;

	struct 
	{
		uint8_t has_cond_pos : 1;
		uint32_t norm_pos;
		uint32_t cond_pos;
	} source;
};

struct emu_source_and_track_instr_info *emu_source_and_track_instr_info_new(struct emu_cpu *cpu, uint32_t eip_before_instruction);
void emu_source_and_track_instr_info_free(struct emu_source_and_track_instr_info *esantii);
void emu_source_and_track_instr_info_free_void(void *x);

bool emu_source_and_track_instr_info_cmp(void *a, void *b);
uint32_t emu_source_and_track_instr_info_hash(void *key);

struct emu_track_and_source
{
	struct emu_tracking_info track;

	struct emu_graph        *static_instr_graph;
	struct emu_hashtable    *static_instr_table;

	struct emu_graph        *run_instr_graph;
	struct emu_hashtable    *run_instr_table;

};


struct emu_track_and_source *emu_track_and_source_new();
void emu_track_and_source_free(struct emu_track_and_source *et);

int32_t emu_track_instruction_check(struct emu *e, struct emu_track_and_source *et);



struct emu_tracking_info *emu_tracking_info_new();
void emu_tracking_info_free(struct emu_tracking_info *eti);

void emu_tracking_info_clear(struct emu_tracking_info *eti);

void emu_tracking_info_diff(struct emu_tracking_info *a, struct emu_tracking_info *b, struct emu_tracking_info *result);

void emu_tracking_info_copy(struct emu_tracking_info *from, struct emu_tracking_info *to);

bool emu_tracking_info_covers(struct emu_tracking_info *a, struct emu_tracking_info *b);

void emu_tracking_info_debug_print(struct emu_tracking_info *a);



#endif
