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
#include <stdio.h>

#define INSTR_CALC(dbits,bits,cpu,dividend,divisor,quotient,remainder)\
{\
	if (divisor == 0) \
	{ \
		emu_strerror_set(cpu->emu,"div by zero (%i bits)\n",bits); \
		emu_errno_set(cpu->emu,EINVAL); \
		return -1; \
	} \
	UINTOF(dbits) q_result = dividend / divisor; \
	UINTOF(dbits) r_result = dividend % divisor; \
\
	quotient = q_result; \
	remainder = r_result; \
	if ( q_result >  max_inttype_borders[bits/8][1][1]) \
	{ \
		emu_strerror_set(cpu->emu,"div quotient larger than intborder (%i bits)\n",bits); \
		emu_errno_set(cpu->emu,EINVAL); \
		return -1; \
	} \
} 

#include "emu/emu.h"
#include "emu/emu_cpu.h"
#include "emu/emu_cpu_data.h"

#include "emu/emu_cpu_stack.h"
#include "emu/emu_memory.h"

/*Intel Architecture Software Developer's Manual Volume 2: Instruction Set Reference (24319102.PDF) page 188*/

int32_t instr_group_3_f6_div(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	if ( i->modrm.mod != 3 )
	{
		/* F6 /6 
		 * Unsigned divide AX by r/m8; AL <- Quotient, AH <- Remainder
		 * DIV r/m8  
		 */
		uint8_t m8;
		MEM_BYTE_READ(c, i->modrm.ea, &m8);
		INSTR_CALC(16, 
				   8,
				   c,
				   *c->reg16[ax],
				   m8,
				   *c->reg8[al],
				   *c->reg8[ah])
	}
	else
	{
		/* F6 /6 
		 * Unsigned divide AX by r/m8; AL <- Quotient, AH <- Remainder
		 * DIV r/m8  
		 */
		INSTR_CALC(16, 
				   8,
				   c,
				   *c->reg16[ax],
				   *c->reg8[i->modrm.rm],
				   *c->reg8[al],
				   *c->reg8[ah])
	}
	return 0;
}

int32_t instr_group_3_f7_div(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	if ( i->modrm.mod != 3 )
	{
		if ( i->prefixes & PREFIX_OPSIZE )
		{
			/* F7 /6 
			 * Unsigned divide DX:AX by r/m16; AX <- Quotient, DX <- Remainder
			 * DIV r/m16 
			 */
			uint16_t m16;
			MEM_WORD_READ(c, i->modrm.ea, &m16);

			uint32_t dividend;
			DWORD_FROM_WORDS(dividend,*c->reg16[dx],*c->reg16[ax]);

			INSTR_CALC(32, 
					   16,
					   c,
					   dividend,
					   m16,
					   *c->reg16[ax],
					   *c->reg16[dx])

		}
		else
		{
			/* F7 /6 
			 * Unsigned divide EDX:EAX by r/m32 doubleword; EAX <- Quotient, EDX <- Remainder
			 * DIV r/m32 
			 */
			uint32_t m32;
			MEM_DWORD_READ(c, i->modrm.ea, &m32);

			uint64_t dividend;
			QWORD_FROM_DWORDS(dividend,c->reg[edx],c->reg[eax]);

			INSTR_CALC(64, 
					   32,
					   c,
					   dividend,
					   m32,
					   c->reg[eax],
					   c->reg[edx])
		}
	}
	else
	{
		if ( i->prefixes & PREFIX_OPSIZE )
		{
			/* F7 /6 
			 * Unsigned divide DX:AX by r/m16; AX <- Quotient, DX <- Remainder
			 * DIV r/m16 
			 */

			uint32_t dividend;
			DWORD_FROM_WORDS(dividend,*c->reg16[dx],*c->reg16[ax]);

			INSTR_CALC(32, 
					   16,
					   c,
					   dividend,
					   *c->reg16[i->modrm.rm],
					   *c->reg16[ax],
					   *c->reg16[dx])

		}
		else
		{
			/* F7 /6 
			 * Unsigned divide EDX:EAX by r/m32 doubleword; EAX <- Quotient, EDX <- Remainder
			 * DIV r/m32 
			 */
			uint64_t dividend;
			QWORD_FROM_DWORDS(dividend,c->reg[edx],c->reg[eax]);

			INSTR_CALC(64, 
					   32,
					   c,
					   dividend,
					   c->reg[i->modrm.rm],
					   c->reg[eax],
					   c->reg[edx])

		}
	}
	return 0;
}

