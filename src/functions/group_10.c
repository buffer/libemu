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
#include "emu/emu_cpu_stack.h"
#include "emu/emu_memory.h"

int32_t instr_group_10_8f_pop(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	if (i->prefixes & PREFIX_OPSIZE)
	{
    /* 8F /0  
	 * Pop top of stack into m16; increment stack pointer  
	 * POP m16 
	 */
	}else
	{
    /* 8F /0  
	 * Pop top of stack into m32; increment stack pointer  
	 * POP m32 
	 */	
		uint32_t m32;
		/* support pop dword fs:[0x00] */
		enum emu_segment oldseg = emu_memory_segment_get(c->mem);
		emu_memory_segment_select(c->mem, s_ss);
		POP_DWORD(c, &m32);
		emu_memory_segment_select(c->mem, oldseg);
		MEM_DWORD_WRITE(c, i->modrm.ea, m32);
	}

	return 0;
}


int32_t instr_group_10_8f(struct emu_cpu *c, struct emu_cpu_instruction *i)
{

	static int32_t (*group_10_8f_fn[8])(struct emu_cpu *c, struct emu_cpu_instruction *i) = {
		/* 0 */ instr_group_10_8f_pop,
		/* 1 */ NULL,
		/* 2 */ NULL,
		/* 3 */ NULL,
		/* 4 */ NULL,
		/* 5 */ NULL,
		/* 6 */ NULL,
		/* 7 */ NULL,
	};

	if ( group_10_8f_fn[i->modrm.opc] != NULL )
		return group_10_8f_fn[i->modrm.opc](c, i);
	else
		return -1;
}

