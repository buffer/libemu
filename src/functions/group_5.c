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
#include <errno.h>

#include "emu/emu.h"
#include "emu/emu_cpu.h"
#include "emu/emu_cpu_data.h"
#include "emu/emu_cpu_functions.h"
#include "emu/emu_cpu_stack.h"
#include "emu/emu_memory.h"




int32_t instr_group_5_ff(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	static int32_t (*group_5_fn[8])(struct emu_cpu *c, struct emu_cpu_instruction *i) = {
		/* 0 */ instr_group_5_ff_inc,
		/* 1 */ instr_group_5_ff_dec,
		/* 2 */ instr_group_5_ff_call,
		/* 3 */ instr_group_5_ff_call,
		/* 4 */ instr_group_5_ff_jmp,
		/* 5 */ instr_group_5_ff_jmp,
		/* 6 */ instr_group_5_ff_push,
		/* 7 */ 0,
	};

	if (group_5_fn[i->modrm.opc] != NULL)
		return group_5_fn[i->modrm.opc](c, i);
	else
		return -1;
}
