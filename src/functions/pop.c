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

#include "emu/emu_cpu_stack.h"
#include "emu/emu_memory.h"

/*Intel Architecture Software Developer's Manual Volume 2: Instruction Set Reference (24319102.PDF) page 571*/

int32_t instr_pop_07(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	/* 07     
	 * Pop top of stack into ES; increment stack pointer   
	 * POP ES  
	 */
	UNIMPLEMENTED(c, NNY);

	return 0;
}

int32_t instr_pop_17(struct emu_cpu *c, struct emu_cpu_instruction *i)
{

	/* 17     
	 * Pop top of stack into SS; increment stack pointer   
	 * POP SS  
	 */
	UNIMPLEMENTED(c, NNY);

	return 0;
}

int32_t instr_pop_1f(struct emu_cpu *c, struct emu_cpu_instruction *i)
{

	/* 1F     
	 * Pop top of stack into DS; increment stack pointer   
	 * POP DS  
	 */
	UNIMPLEMENTED(c, NNY);
	
	return 0;
}

int32_t instr_pop_5x(struct emu_cpu *c, struct emu_cpu_instruction *i)
{

	if (i->prefixes & PREFIX_OPSIZE)
	{
		/* 58+ rw 
		 * Pop top of stack into r16; increment stack pointer  
		 * POP r16 
		 */
		TRACK_INIT_REG16(c->instr, i->opc & 7);
		POP_WORD(c, c->reg16[i->opc & 7 ]);
	}else
	{
		/* 58+ rd 
		 * Pop top of stack into r32; increment stack pointer  
		 * POP r32 
		 */
		TRACK_INIT_REG32(c->instr, i->opc & 7);
		POP_DWORD(c, &c->reg[i->opc & 7]);
	}
	return 0;
}

int32_t instr_pop_0fa1(struct emu_cpu *c, struct emu_cpu_instruction *i)
{

	/* 0F A1  
	 * Pop top of stack into FS; increment stack pointer   
	 * POP FS  
	 */
	UNIMPLEMENTED(c, NNY);

	return 0;
}

int32_t instr_pop_0fa9(struct emu_cpu *c, struct emu_cpu_instruction *i)
{

	/* 0F A9  
	 * Pop top of stack into GS; increment stack pointer   
	 * POP GS  
	 */
	UNIMPLEMENTED(c, NNY);

	return 0;
}

int32_t instr_popad_61(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	int32_t j;
	
	if( i->prefixes & PREFIX_OPSIZE )
	{
		for( j = 7; j >= 0; j-- )
		{
			if( j != 4 )
			{
				POP_WORD(c, c->reg16[j])
			}
			else
			{
				c->reg[esp] += 2;
			}
		}
	}
	else
	{
		for( j = 7; j >= 0; j-- )
		{
			if( j != 4 )
			{
				POP_DWORD(c, &c->reg[j]);
			}
			else
			{
				c->reg[esp] += 4;
			}
		}
	}
	
	return 0;
}

int32_t instr_mov_89(struct emu_cpu *c, struct emu_cpu_instruction *i);

int32_t instr_leave(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
    struct emu_cpu_instruction back = *i;
    // mov esp, ebp
    // pop ebp
    i->modrm.mod = 3;
    i->modrm.opc = ebp;
    i->modrm.rm = esp;
    instr_mov_89(c, i);
    i->opc = ebp;
    instr_pop_5x(c, i);
    *i = back;
    return 0;
}
