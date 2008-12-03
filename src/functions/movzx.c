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

#include "emu/emu_memory.h"

/*Intel Architecture Software Developer's Manual Volume 2: Instruction Set Reference (24319102.PDF) page 486*/

#include <ctype.h>
#include <stdio.h>

int32_t instr_movzx_0fb6(struct emu_cpu *c, struct emu_cpu_instruction *i)
{

	if ( i->modrm.mod != 3 )
	{
		if ( i->prefixes & PREFIX_OPSIZE )
		{
			/* 0F B6 /r 
			 * Move byte to word with zero-extension
			 * MOVZX r16,r/m8 
			 */
			uint8_t m8;
			MEM_BYTE_READ(c, i->modrm.ea, &m8);
			*c->reg16[i->modrm.opc] = m8;
		}
		else
		{
			/* 0F B6 /r 
			 * Move byte to doubleword, zero-extension
			 * MOVZX r32,r/m8
			 */
			uint8_t m8;
			MEM_BYTE_READ(c, i->modrm.ea, &m8);
			c->reg[i->modrm.opc] = m8;
		}
	}
	else
	{
		if ( i->prefixes & PREFIX_OPSIZE )
		{
			/* 0F B6 /r 
			 * Move byte to word with zero-extension
			 * MOVZX r16,r/m8 
			 */
			*c->reg16[i->modrm.opc] = *c->reg8[i->modrm.rm];
		}
		else
		{
			/* 0F B6 /r 
			 * Move byte to doubleword, zero-extension
			 * MOVZX r32,r/m8
			 */
			c->reg[i->modrm.opc] = *c->reg8[i->modrm.rm];
		}
	}
	return 0;
}

int32_t instr_movzx_0fb7(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	if ( i->modrm.mod != 3 )
	{
		/* 0F B7 /r 
		 * Move word to doubleword, zero-extension
		 * MOVZX r32,m16
		 */
		uint16_t m16;
		MEM_WORD_READ(c, i->modrm.ea, &m16);
		c->reg[i->modrm.opc] = m16;

	}
	else
	{
		/* 0F B7 /r 
		 * Move word to doubleword, zero-extension
		 * MOVZX r32,r16
		 */
		c->reg[i->modrm.opc] = *c->reg16[i->modrm.rm];
	}
	return 0;
}

