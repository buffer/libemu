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

#include <stdint.h>

#include "emu/emu_cpu.h"
#include "emu/emu_cpu_data.h"
#include "emu/emu_cpu_functions.h"
#include "emu/emu_memory.h"

int32_t instr_group_1_80(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	static int32_t (*group_1_80_fn[8])(struct emu_cpu *c, struct emu_cpu_instruction *i) = {
		/* 0 */ instr_group_1_80_add,
		/* 1 */ instr_group_1_80_or,
		/* 2 */ instr_group_1_80_adc,
		/* 3 */ instr_group_1_80_sbb,
		/* 4 */ instr_group_1_80_and,
		/* 5 */ instr_group_1_80_sub,
		/* 6 */ instr_group_1_80_xor,
		/* 7 */ instr_group_1_80_cmp,
	};

	return group_1_80_fn[i->modrm.opc](c, i);
}

int32_t instr_group_1_81(struct emu_cpu *c, struct emu_cpu_instruction *i)
{

	static int32_t (*group_1_81_fn[8])(struct emu_cpu *c, struct emu_cpu_instruction *i) = {
		/* 0 */ instr_group_1_81_add,
		/* 1 */ instr_group_1_81_or,
		/* 2 */ instr_group_1_81_adc,
		/* 3 */ instr_group_1_81_sbb,
		/* 4 */ instr_group_1_81_and,
		/* 5 */ instr_group_1_81_sub,
		/* 6 */ instr_group_1_81_xor,
		/* 7 */ instr_group_1_81_cmp,
	};

	return group_1_81_fn[i->modrm.opc](c, i);
}

int32_t instr_group_1_82(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	return 0;
}

int32_t instr_group_1_83(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	static int32_t (*group_1_83_fn[8])(struct emu_cpu *c, struct emu_cpu_instruction *i) = {
		/* 0 */ instr_group_1_83_add,
		/* 1 */ instr_group_1_83_or,
		/* 2 */ instr_group_1_83_adc,
		/* 3 */ instr_group_1_83_sbb,
		/* 4 */ instr_group_1_83_and,
		/* 5 */ instr_group_1_83_sub,
		/* 6 */ instr_group_1_83_xor,
		/* 7 */ instr_group_1_83_cmp,
	};

	return group_1_83_fn[i->modrm.opc](c, i);
}


