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


#include <string.h>

#include "emu/emu.h"
#include "emu/emu_cpu.h"
#include "emu/emu_cpu_data.h"
#include "emu/emu_instruction.h"
#include "emu/emu_track.h"
#include "emu/emu_source.h"
#include "emu/emu_hashtable.h"
#include "emu/emu_graph.h"

struct emu_track_and_source *emu_track_and_source_new(void)
{
	struct emu_track_and_source *et = (struct emu_track_and_source *)malloc(sizeof(struct emu_track_and_source));
	memset(et, 0, sizeof(struct emu_track_and_source));
	et->track.reg[esp] = 0xffffffff;
	return et;
}

void emu_track_and_source_free(struct emu_track_and_source *et)
{
	if (et->static_instr_table != NULL)
		emu_hashtable_free(et->static_instr_table);

	if (et->static_instr_graph != NULL)
		emu_graph_free(et->static_instr_graph);

	if (et->run_instr_table != NULL)
		emu_hashtable_free(et->run_instr_table);

	if (et->run_instr_graph != NULL)
		emu_graph_free(et->run_instr_graph);

	free(et);

}


void debug_instruction(struct emu_instruction *i);


#include "emu/emu_cpu_functions.h"

int32_t emu_track_instruction_check(struct emu *e, struct emu_track_and_source *et)
{
	struct emu_cpu *c = emu_cpu_get(e);
	int i;

	if (c->instr.is_fpu)
	{
		if (c->instr.track.need.fpu  > et->track.fpu )
			return -1;

		et->track.fpu |= c->instr.track.init.fpu;

	}else
	{
//		debug_instruction(&c->instr);
/*
		if (c->cpu_instr_info->function == instr_xchg_9x)
		{
			uint32_t reg1 = et->reg[eax];
			et->reg[eax] = et->reg[c->instr.cpu.opc & 7];
			et->reg[c->instr.cpu.opc & 7] = reg1;
		}
*/
		for (i=0;i<8;i++)
		{
			if (i == esp)
				continue;

//			printf("0x%08x 0x%08x\n", c->instr.track.need.reg[i], et->track.reg[i]);
			if (c->instr.track.need.reg[i] > et->track.reg[i])
				return -1;
		}

		for (i=0;i<8;i++)
		{
//			printf("0x%1x 0x%1x\n", (c->instr.track.need.eflags & 1 << i), (et->track.eflags & 1 << i));
			if ( (c->instr.track.need.eflags & 1 << i) > (et->track.eflags & 1 << i))
				return -1;
		}

		for (i=0;i<8;i++)
		{
//			printf("reg %i before %08x after %08x\n", i, et->track.reg[i], c->instr.track.init.reg[i]);
			et->track.reg[i] |= c->instr.track.init.reg[i];
		}

		et->track.eflags |= c->instr.track.init.eflags;

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
	if( CPU_DEBUG_FLAG_ISSET(cpu, instruction_string ) || CPU_DEBUG_FLAG_ISSET(cpu, instruction_size ) )
		etii->instrstring = strdup(cpu->instr_string);
	else
		etii->instrstring = NULL;

	if ( cpu->instr.is_fpu )
	{
		etii->source.norm_pos 		= cpu->instr.source.norm_pos;
		etii->track.init.fpu 		= cpu->instr.track.init.fpu;
	}else
	{
		etii->source.has_cond_pos 	= cpu->instr.source.has_cond_pos;
		etii->source.cond_pos 		= cpu->instr.source.cond_pos;
		etii->source.norm_pos 		= cpu->instr.source.norm_pos;

		etii->track.init.eflags 	= cpu->instr.track.init.eflags;
		memcpy(etii->track.init.reg, cpu->instr.track.init.reg, sizeof(uint32_t)*8);

		etii->track.need.eflags 	= cpu->instr.track.need.eflags;
		memcpy(etii->track.need.reg, cpu->instr.track.need.reg, sizeof(uint32_t)*8);
	}
	return etii;
}

void emu_source_and_track_instr_info_free(struct emu_source_and_track_instr_info *etii)
{
	if (etii->instrstring != NULL)
		free(etii->instrstring);

	free(etii);
}

void emu_source_and_track_instr_info_free_void(void *x)
{
	emu_source_and_track_instr_info_free((struct emu_source_and_track_instr_info *)x);
}

void emu_tracking_info_diff(struct emu_tracking_info *a, struct emu_tracking_info *b, struct emu_tracking_info *result)
{
	int i;
	for (i=0;i<8;i++)
	{
		result->reg[i] = a->reg[i] & ~b->reg[i];
	}
	result->eflags = a->eflags & ~b->eflags;
	result->fpu = a->fpu & ~b->fpu;
}

struct emu_tracking_info *emu_tracking_info_new(void)
{
	struct emu_tracking_info *eti = malloc(sizeof(struct emu_tracking_info));
	memset(eti, 0, sizeof(struct emu_tracking_info));
	eti->reg[esp] = 0xffffffff;
	return eti;
}

void emu_tracking_info_free(struct emu_tracking_info *eti)
{
	free(eti);
}

void emu_tracking_info_clear(struct emu_tracking_info *eti)
{
	memset(eti, 0, sizeof(struct emu_tracking_info));
	eti->reg[esp] = 0xffffffff;
}

void emu_tracking_info_copy(struct emu_tracking_info *from, struct emu_tracking_info *to)
{
	memcpy(to, from, sizeof(struct emu_tracking_info));
}

bool emu_tracking_info_covers(struct emu_tracking_info *a, struct emu_tracking_info *b)
{
	int i;
	for (i=0;i<8;i++)
	{
		if (i == esp)
			continue;

		if (b->reg[i] > a->reg[i])
			return false;
	}

	for (i=0;i<8;i++)
	{
		if ( (b->eflags & 1 << i) > (a->eflags & 1 << i))
			return false;
	}

	if ( b->fpu > a->fpu )
		return false;


	return true;
}



void emu_tracking_info_debug_print(struct emu_tracking_info *a)
{
	return;

	static const char *regm32[] = {
		"eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi"
	};

/*	static const char *regm16[] = {
		"ax", "cx", "dx", "bx", "sp", "bp", "si", "di"
	};

	static const char *regm8[] = {
		"al", "cl", "dl", "bl", "ah", "ch", "dh", "bh"
	};
*/
	/* 0     1     2     3      4       5       6     7 */
	const char *eflagm[] = 
	{ 
		"CF", "  ", "PF", "  " , "AF"  , "    ", "ZF", "SF", 
		"TF", "IF", "DF", "OF" , "IOPL", "IOPL", "NT", "  ",
		"RF", "VM", "AC", "VIF", "RIP" , "ID"  , "  ", "  ",
		"  ", "  ", "  ", "   ", "    ", "    ", "  ", "  "
	};

	int i;

	printf("tracking_info %08x :\n\tregs: ", (unsigned int)(uintptr_t)a);
	for ( i=0; i<7; i++ )
	{
		if ( a->reg[i] > 0 )
		{
			printf("%s ", regm32[i]);
		}
		else
		{
			printf("    ");
		}
	}
	printf("\n\tflags:");

	for ( i=0; i<8; i++ )
	{
		if ( (a->eflags & 1 << i) )
		{
			printf("%.4s ", eflagm[i]);
		}
		else
		{
			printf("     ");
		}
	}

	printf("\n\tfpu:");
	if (a->fpu)
	{
		printf("initialized\n");
	}else
	{
		printf("\n");
	}

	

}
