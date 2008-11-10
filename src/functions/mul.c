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
#include <string.h>

#define INSTR_CALC(inbits, outbits, cpu, a, b) \
UINTOF(inbits) operand_a = a; \
UINTOF(inbits) operand_b = b; \
UINTOF(outbits) operation_result = operand_a * operand_b; \

#include "emu/emu.h"
#include "emu/emu_cpu.h"
#include "emu/emu_cpu_data.h"

#include "emu/emu_cpu_stack.h"
#include "emu/emu_memory.h"

/*Intel Architecture Software Developer's Manual Volume 2: Instruction Set Reference (24319102.PDF) page 488*/

#define INSTR_SET_FLAGS(cpu, upper_result)	\
if (upper_result == 0) \
{ \
	CPU_FLAG_UNSET(cpu,f_cf); \
	CPU_FLAG_UNSET(cpu,f_of); \
} \
else \
{ \
	CPU_FLAG_SET(cpu,f_cf); \
	CPU_FLAG_SET(cpu,f_of); \
}




int32_t instr_group_3_f6_mul(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	if ( i->modrm.mod != 3 )
	{
		/* F6 /4 
		 * Unsigned multiply (AX <- AL * r/m8)             
		 * MUL r/m8  
		 */
		uint8_t m8;
		MEM_BYTE_READ(c, i->modrm.ea, &m8);
		INSTR_CALC(8, 
				   16,
				   c,
				   *c->reg8[al],
				   m8)
		*c->reg16[ax] = operation_result;
		uint8_t high;
		WORD_UPPER_TO_BYTE(high,operation_result);
		INSTR_SET_FLAGS(c,high);

	}
	else
	{
		/* F6 /4 
		 * Unsigned multiply (AX <- AL * r/m8)             
		 * MUL r/m8  
		 */
		INSTR_CALC(8,
				   16,
				   c,
				   *c->reg8[al],
				   *c->reg8[i->modrm.rm])

		*c->reg16[ax] = operation_result;
		uint8_t high;
		WORD_UPPER_TO_BYTE(high,operation_result);
		INSTR_SET_FLAGS(c,high);

	}
	return 0;
}

int32_t instr_group_3_f7_mul(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	if ( i->modrm.mod != 3 )
	{
		if ( i->prefixes & PREFIX_OPSIZE )
		{
			/* F7 /4 
			 * Unsigned multiply (DX:AX <- AX * r/m16)         
			 * MUL r/m16 
			 */          
			uint16_t m16;
			MEM_WORD_READ(c, i->modrm.ea, &m16);
			INSTR_CALC(16,
					   32,
					   c,
					   *c->reg16[al],
					   m16)

			DWORD_UPPER_TO_WORD(*c->reg16[dx],operation_result);
			DWORD_LOWER_TO_WORD(*c->reg16[ax],operation_result);

			INSTR_SET_FLAGS(c,*c->reg16[dx]);
		}
		else
		{
			/* F7 /4 
			 * Unsigned multiply (EDX:EAX <- EAX * r/m32)      
			 * MUL r/m32 
			 */
			uint32_t m32;
			MEM_DWORD_READ(c, i->modrm.ea, &m32);
			INSTR_CALC(32,
					   64,
					   c,
					   c->reg[eax],
					   m32)

			QWORD_UPPER_TO_DWORD(c->reg[edx],operation_result);
			QWORD_LOWER_TO_DWORD(c->reg[eax],operation_result);

			INSTR_SET_FLAGS(c,c->reg[edx]);
		}
	}
	else
	{
		if ( i->prefixes & PREFIX_OPSIZE )
		{
			/* F7 /4 
			 * Unsigned multiply (DX:AX <- AX * r/m16)         
			 * MUL r/m16 
			 */                                                                      
			INSTR_CALC(16,
					   32,
					   c,
					   *c->reg16[al],
					   *c->reg16[i->modrm.rm])
			DWORD_UPPER_TO_WORD(*c->reg16[dx],operation_result);
			DWORD_LOWER_TO_WORD(*c->reg16[ax],operation_result);

			INSTR_SET_FLAGS(c,*c->reg16[dx]);
		}
		else
		{
			/* F7 /4 
			 * Unsigned multiply (EDX:EAX <- EAX * r/m32)      
			 * MUL r/m32 
			 */
			INSTR_CALC(32,
					   64,
					   c,
					   c->reg[eax],
					   c->reg[i->modrm.rm])

			QWORD_UPPER_TO_DWORD(c->reg[edx],operation_result);
			QWORD_LOWER_TO_DWORD(c->reg[eax],operation_result);

			INSTR_SET_FLAGS(c,c->reg[edx]);
		}
	}
	return 0;
}
