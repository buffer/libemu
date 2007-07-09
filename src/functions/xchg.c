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


#include <inttypes.h>

#include "emu/emu.h"
#include "emu/emu_cpu.h"
#include "emu/emu_cpu_data.h"

#include "emu/emu_cpu_stack.h"
#include "emu/emu_memory.h"

#include "emu/emu_track.h"

/*Intel Architecture Software Developer's Manual Volume 2: Instruction Set Reference (24319102.PDF) page 754*/

int32_t instr_xchg_86(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	if ( i->modrm.mod != 3 )
	{
		/* 86 /r 
		 * Exchange r8 (byte register) with byte from r/m8
		 * XCHG r/m8,r8   
		 * Exchange byte from r/m8 with r8 (byte register)
		 * XCHG r8,r/m8   
		 */     
		uint8_t m8;
		MEM_BYTE_READ(c, i->modrm.ea, &m8);
		MEM_BYTE_WRITE(c, i->modrm.ea, *c->reg8[i->modrm.opc]);
		*c->reg8[i->modrm.opc] = m8;
	}
	else
	{
		/* 86 /r 
		 * Exchange r8 (byte register) with byte from r/m8
		 * XCHG r/m8,r8   
		 * Exchange byte from r/m8 with r8 (byte register)
		 * XCHG r8,r/m8   
		 */     
		uint8_t swap8 = *c->reg8[i->modrm.rm];
		*c->reg8[i->modrm.rm] = *c->reg8[i->modrm.opc];
		*c->reg8[i->modrm.opc] = swap8;
	}
	return 0;
}


int32_t instr_xchg_87(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	if ( i->modrm.mod != 3 )
	{
		if ( i->prefixes & PREFIX_OPSIZE )
		{
			/* 87 /r 
			 * Exchange r16 with word from r/m16
			 * XCHG r/m16,r16 
			 * Exchange word from r/m16 with r16
			 * XCHG r16,r/m16 
			 */     
			uint16_t m16;
			MEM_WORD_READ(c, i->modrm.ea, &m16);
			MEM_WORD_WRITE(c, i->modrm.ea, *c->reg16[i->modrm.opc]);
			*c->reg16[i->modrm.opc] = m16;

		}
		else
		{
			/* 87 /r 
			 * Exchange r32 with doubleword from r/m32
			 * XCHG r/m32,r32 
			 * Exchange doubleword from r/m32 with r32
			 * XCHG r32,r/m32 
			 */     
			uint32_t m32;
			MEM_DWORD_READ(c, i->modrm.ea, &m32);
			MEM_DWORD_WRITE(c, i->modrm.ea, c->reg[i->modrm.opc]);
			c->reg[i->modrm.opc] = m32;

		}
	}
	else
	{
		if ( i->prefixes & PREFIX_OPSIZE )
		{
			/* 87 /r 
			 * Exchange r16 with word from r/m16
			 * XCHG r/m16,r16 
			 * Exchange word from r/m16 with r16
			 * XCHG r16,r/m16 
			 */     
			uint16_t swap16 = *c->reg16[i->modrm.rm];
			*c->reg16[i->modrm.rm] = *c->reg16[i->modrm.opc];
			*c->reg16[i->modrm.opc] = swap16;

		}
		else
		{
			/* 87 /r 
			 * Exchange r32 with doubleword from r/m32
			 * XCHG r/m32,r32 
			 * Exchange doubleword from r/m32 with r32
			 * XCHG r32,r/m32 
			 */     
			uint32_t swap32 = c->reg[i->modrm.rm];
			c->reg[i->modrm.rm] = c->reg[i->modrm.opc];
			c->reg[i->modrm.opc] = swap32;

		}

	}
	return 0;
}

int32_t instr_xchg_9x(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	if ( i->prefixes & PREFIX_OPSIZE )
	{
		/* 90+rw 
		 * Exchange r16 with AX
		 * XCHG AX,r16    
		 * Exchange AX with r16
		 * XCHG r16,AX    
		 */     
		uint16_t swap16 = *c->reg16[ax];
		*c->reg16[ax] = *c->reg16[i->opc & 7];
		*c->reg16[i->opc & 7] = swap16;


	}
	else
	{
		/* 90+rd 
		 * Exchange r32 with EAX
		 * XCHG EAX,r32   
		 * Exchange EAX with r32
		 * XCHG r32,EAX   
		 */

		uint32_t swap32 = c->reg[eax];
		c->reg[eax] = c->reg[i->opc & 7];
		c->reg[i->opc & 7] = swap32;

		if ( c->tracking != NULL )
		{
			swap32 = c->tracking->track.reg[eax];
			c->tracking->track.reg[eax] = c->tracking->track.reg[i->opc & 7];
			c->tracking->track.reg[i->opc & 7] = swap32;
		}

	}
	return 0;
}

