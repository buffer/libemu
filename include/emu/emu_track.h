/* @header@ */

#include <stdint.h>
#include <stdbool.h>

struct emu;
struct emu_graph;
struct emu_instruction;


struct emu_track_instr_info
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

struct emu_track_instr_info *emu_track_instr_info_new(struct emu_cpu *cpu, uint32_t eip_before_instruction);
void emu_track_instr_info_free();

bool emu_track_instr_info_cmp(void *a, void *b);
uint32_t emu_track_instr_info_hash(void *key);


struct emu_track
{
	uint32_t eflags;
	uint32_t reg[8];

	struct emu_graph 		*trackgraph;
	struct emu_hashtable 	*instrtable;
};


struct emu_track *emu_track_new();
void emu_track_free(struct emu_track *et);
uint32_t emu_track_tree_create(struct emu *e, struct emu_track *et, uint32_t datastart, uint32_t datasize);
int32_t emu_track_instruction_check(struct emu *e, struct emu_track *et);

