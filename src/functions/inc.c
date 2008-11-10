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

#define INSTR_CALC(bits, a)						\
UINTOF(bits) operand_a = a;								\
UINTOF(bits) operation_result = operand_a+1;				\
a = operation_result;



#define INSTR_SET_FLAG_OF(cpu, bits)									\
{																				\
	int64_t sz = (INTOF(bits))operand_a;                                            \
																				\
	sz++;																	\
																				\
	if (sz < max_inttype_borders[sizeof(operation_result)][0][0] || sz > max_inttype_borders[sizeof(operation_result)][0][1] \
	|| sz != (INTOF(bits))operation_result )									    \
	{                                                                           \
		CPU_FLAG_SET(cpu, f_of);                                                \
	}else                                                                       \
	{                                                                           \
		CPU_FLAG_UNSET(cpu, f_of);                                              \
	}                                                                           \
}


#include "emu/emu_cpu.h"
#include "emu/emu_cpu_data.h"

#include "emu/emu_memory.h"

/*Intel Architecture Software Developer's Manual Volume 2: Instruction Set Reference (24319102.PDF) page 341*/


#ifdef INSTR_CALC_AND_SET_FLAGS
#undef INSTR_CALC_AND_SET_FLAGS
#endif // INSTR_CALC_AND_SET_FLAGS

#define INSTR_CALC_AND_SET_FLAGS(bits, cpu, a)	\
INSTR_CALC(bits, a)								\
INSTR_SET_FLAG_ZF(cpu)								\
INSTR_SET_FLAG_PF(cpu)								\
INSTR_SET_FLAG_SF(cpu)								\
INSTR_SET_FLAG_OF(cpu,bits)								




int32_t instr_inc_4x(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	if ( i->prefixes & PREFIX_OPSIZE )
	{
		/* 40+ rw 
		 * Increment word register by 1
		 * INC r16 
		 */
		INSTR_CALC_AND_SET_FLAGS(16, c, *c->reg16[i->opc & 7])
	}else
	{
		/* 40+ rd 
		 * Increment doubleword register by 1
		 * INC r32 
		 */
		INSTR_CALC_AND_SET_FLAGS(32, c, c->reg[i->opc & 7])
	}
	return 0;
}


int32_t instr_group_4_fe_inc(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	/* FE /0
	 * INC r/m8 
	 * Increment r/m byte by 1
	 */
	if ( i->modrm.mod != 3 )
	{
		uint8_t dst;
		MEM_BYTE_READ(c, i->modrm.ea, &dst);

		INSTR_CALC_AND_SET_FLAGS(8, c, dst)

		MEM_BYTE_WRITE(c, i->modrm.ea, dst);

	}
	else
	{
		INSTR_CALC_AND_SET_FLAGS(8, c, *c->reg8[i->modrm.rm])
	}
	return 0;
}

	  
int32_t instr_group_5_ff_inc(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	if ( i->modrm.mod != 3 )
	{
		if ( i->prefixes & PREFIX_OPSIZE )
		{
			/* FF /0 
			 * Increment r/m word by 1
			 * INC r/m16 
			 */   
			uint16_t dst;
			MEM_WORD_READ(c, i->modrm.ea, &dst);

			INSTR_CALC_AND_SET_FLAGS(16, c, dst)

			MEM_WORD_WRITE(c, i->modrm.ea, dst);
		}
		else
		{
			/* FF /0 
			 * Increment r/m doubleword by 1
			 * INC r/m32 
			 */
			uint32_t dst;
			MEM_DWORD_READ(c, i->modrm.ea, &dst);

			INSTR_CALC_AND_SET_FLAGS(32, c, dst)

			MEM_DWORD_WRITE(c, i->modrm.ea, dst);
		}   
	}
	else
	{
		if ( i->prefixes & PREFIX_OPSIZE )
		{
			/* FF /0 
			 * Increment r/m word by 1
			 * INC r/m16 
			 */   
			INSTR_CALC_AND_SET_FLAGS(16, c, *c->reg16[i->modrm.rm])
		}
		else
		{
			/* FF /0 
			 * Increment r/m doubleword by 1
			 * INC r/m32 
			 */
			INSTR_CALC_AND_SET_FLAGS(32, c, c->reg[i->modrm.rm])
		}   
	}
	return 0;
}
