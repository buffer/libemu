/* @header@ */

#include <string.h>

#include "emu/emu.h"
#include "emu/emu_cpu.h"
#include "emu/emu_cpu_data.h"
#include "emu/emu_instruction.h"
#include "emu/emu_track.h"
#include "emu/emu_source.h"
#include "emu/emu_hashtable.h"
#include "emu/emu_graph.h"

struct emu_track_and_source *emu_track_and_source_new()
{
	struct emu_track_and_source *et = (struct emu_track_and_source *)malloc(sizeof(struct emu_track_and_source));
	memset(et, 0, sizeof(struct emu_track_and_source));
	et->reg[esp] = 0xffffffff;
	return et;
}

void emu_track_and_source_free(struct emu_track_and_source *et)
{
	if (et->static_instr_table != NULL)
		emu_hashtable_free(et->static_instr_table);

	if (et->static_instr_graph != NULL)
		emu_graph_free(et->static_instr_graph);

	free(et);

}


void debug_instruction(struct emu_cpu_instruction *i);


#include "emu/emu_cpu_functions.h"

int32_t emu_track_instruction_check(struct emu *e, struct emu_track_and_source *et)
{
	struct emu_cpu *c = emu_cpu_get(e);
	int i;

	if (c->instr.is_fpu)
	{

	}else
	{
//		debug_instruction(&c->instr.cpu);

		if (c->cpu_instr_info->function == instr_xchg_9x)
		{
			uint32_t reg1 = et->reg[eax];
			et->reg[eax] = et->reg[c->instr.cpu.opc & 7];
			et->reg[c->instr.cpu.opc & 7] = reg1;
		}

		for (i=0;i<8;i++)
		{
//			printf("0x%08x 0x%08x\n", c->instr.cpu.track.need.reg[i], et->reg[i]);
			if (c->instr.cpu.track.need.reg[i] > et->reg[i])
				return -1;
		}

		for (i=0;i<8;i++)
		{
//			printf("0x%1x 0x%1x\n", (c->instr.cpu.track.need.eflags & 1 << i), (et->eflags & 1 << i));
			if ( (c->instr.cpu.track.need.eflags & 1 << i) > (et->eflags & 1 << i))
				return -1;
		}

		for (i=0;i<8;i++)
		{
//			printf("reg %i before %08x after %08x\n", i, et->reg[i], c->instr.cpu.track.init.reg[i]);
			et->reg[i] |= c->instr.cpu.track.init.reg[i];
		}
		et->eflags |= c->instr.cpu.track.init.eflags;

	}


	return 0;
}


struct emu_source_and_track_instr_info *emu_source_and_track_instr_info_new(struct emu_cpu *cpu, uint32_t eip_before_instruction)
{
	struct emu_source_and_track_instr_info *etii = (struct emu_source_and_track_instr_info *)malloc(sizeof(struct emu_source_and_track_instr_info));
	if( etii == NULL )
	{
		return NULL;
	}
	memset(etii, 0, sizeof(struct emu_source_and_track_instr_info));

	etii->eip = eip_before_instruction;
	etii->instrstring = strdup(cpu->instr_string);

	if ( cpu->instr.is_fpu )
	{
		etii->source.norm_pos 		= cpu->instr.fpu.source.norm_pos;		
	}else
	{
		etii->source.has_cond_pos 	= cpu->instr.cpu.source.has_cond_pos;
		etii->source.cond_pos 		= cpu->instr.cpu.source.cond_pos;
		etii->source.norm_pos 		= cpu->instr.cpu.source.norm_pos;

		etii->track.init.eflags 	= cpu->instr.cpu.track.init.eflags;
		memcpy(etii->track.init.reg, cpu->instr.cpu.track.init.reg, sizeof(uint32_t)*8);

		etii->track.need.eflags 	= cpu->instr.cpu.track.need.eflags;
		memcpy(etii->track.need.reg, cpu->instr.cpu.track.need.reg, sizeof(uint32_t)*8);
	}
	return etii;
}

void emu_source_and_track_instr_info_free(struct emu_source_and_track_instr_info *etii)
{
	if (etii->instrstring != NULL)
		free(etii->instrstring);

	free(etii);
}

bool emu_source_and_track_instr_info_cmp(void *a, void *b)
{
	if ((uint32_t)a == (uint32_t)b)
		return true;

	return false;
}

uint32_t emu_source_and_track_instr_info_hash(void *key)
{
	uint32_t ukey = (uint32_t)key;
	ukey++;
	return ukey;
}

