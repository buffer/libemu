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

#define INSTR_CALC(bits, a, cpu) \
UINTOF(bits) operand_a = a; \
UINTOF(bits) operation_result = 0-operand_a; 

#define INSTR_SET_FLAG_OF(cpu, bits) \
{ \
	int64_t sx = (INTOF(bits))operand_a; \
	int64_t sz = 0; \
 \
	sz = 0-sx; \
 \
	if (sz < max_inttype_borders[sizeof(operation_result)][0][0] || sz > max_inttype_borders[sizeof(operation_result)][0][1] \
	|| sz != (INTOF(bits))operation_result )									    \
	{                                                                           \
		CPU_FLAG_SET(cpu, f_of);                                                 \
	}else                                                                       \
	{                                                                           \
		CPU_FLAG_UNSET(cpu, f_of);                                               \
	}                                                                           \
}

#define INSTR_SET_FLAG_CF(cpu, a)											\
{																				\
	if (a == 0)																\
	{                                                                           \
		CPU_FLAG_SET(cpu, f_cf);                                                 \
	}else                                                                       \
	{                                                                           \
		CPU_FLAG_UNSET(cpu, f_cf);                                               \
	}                                                                           \
}



#include "emu/emu.h"
#include "emu/emu_cpu.h"
#include "emu/emu_cpu_data.h"

#include "emu/emu_cpu_stack.h"
#include "emu/emu_memory.h"

/*Intel Architecture Software Developer's Manual Volume 2: Instruction Set Reference (24319102.PDF) page 494*/

#define INSTR_CALC_AND_SET_FLAGS(bits, cpu, a)	\
INSTR_CALC(bits, a, cpu) \
INSTR_SET_FLAG_CF(cpu, a) \
INSTR_SET_FLAG_OF(cpu, bits) \
INSTR_SET_FLAG_SF(cpu) \
INSTR_SET_FLAG_ZF(cpu) \
INSTR_SET_FLAG_PF(cpu)





int32_t instr_group_3_f6_neg(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	if ( i->modrm.mod != 3 )
	{
		/* F6 /3 
		 * Two's complement negate r/m8
		 * NEG r/m8  
		 */
		uint8_t m8;
		MEM_BYTE_READ(c, i->modrm.ea, &m8);
		INSTR_CALC_AND_SET_FLAGS(8,
								 c,
								 m8)
		MEM_BYTE_WRITE(c, i->modrm.ea, m8);

	}
	else
	{
		/* F6 /3 
		 * Two's complement negate r/m8
		 * NEG r/m8  
		 */
		INSTR_CALC_AND_SET_FLAGS(8,c,*c->reg8[i->modrm.rm]);
	}
	return 0;
}


int32_t instr_group_3_f7_neg(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	if ( i->modrm.mod != 3 )
	{
		if ( i->prefixes & PREFIX_OPSIZE )
		{
			/* F7 /3 
			 * Two's complement negate r/m16
			 * NEG r/m16 
			 */
			uint16_t m16;
			MEM_WORD_READ(c, i->modrm.ea, &m16);
			INSTR_CALC_AND_SET_FLAGS(16,
									 c,
									 m16)
			MEM_WORD_WRITE(c, i->modrm.ea, m16);

		}
		else
		{
			/* F7 /3 
			 * Two's complement negate r/m32
			 * NEG r/m32 
			 */
			uint32_t m32;
			MEM_DWORD_READ(c, i->modrm.ea, &m32);
			INSTR_CALC_AND_SET_FLAGS(32,
									 c,
									 m32)
			MEM_DWORD_WRITE(c, i->modrm.ea, m32);

		}
	}
	else
	{
		if ( i->prefixes & PREFIX_OPSIZE )
		{
			/* F7 /3 
			 * Two's complement negate r/m16
			 * NEG r/m16 
			 */
			INSTR_CALC_AND_SET_FLAGS(16,
									 c,
									 *c->reg16[i->modrm.rm])
		}
		else
		{
			/* F7 /3 
			 * Two's complement negate r/m32
			 * NEG r/m32 
			 */
			INSTR_CALC_AND_SET_FLAGS(32,
									 c,
									 c->reg[i->modrm.rm])

		}
	}


	return 0;
}



