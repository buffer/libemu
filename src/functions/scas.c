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

#define INSTR_CALC(bits, a, b) \
UINTOF(bits) operation_result = (a) - (b); \
UINTOF(bits) operand_a = a; \
UINTOF(bits) operand_b = b;


#include "emu/emu.h"
#include "emu/emu_cpu.h"
#include "emu/emu_cpu_data.h"

#include "emu/emu_cpu_stack.h"
#include "emu/emu_memory.h"

/*Intel Architecture Software Developer's Manual Volume 2: Instruction Set Reference (24319102.PDF) page 669*/

#define INSTR_CALC_AND_SET_FLAGS(bits, cpu, a, b) \
INSTR_CALC(bits, a, b) \
INSTR_SET_FLAG_ZF(cpu) \
INSTR_SET_FLAG_PF(cpu) \
INSTR_SET_FLAG_SF(cpu) \
INSTR_SET_FLAG_CF(cpu, -) \
INSTR_SET_FLAG_OF(cpu, -,bits) 


#define INSTR_CALC_EDI(cpu, bits) \
{ \
	if ( !CPU_FLAG_ISSET(cpu,f_df) ) \
	cpu->reg[edi]+=bits/8; \
else \
	cpu->reg[edi]-=bits/8; \
}


int32_t instr_scas_ae(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	if ( i->prefixes & PREFIX_ADSIZE )
	{
		/* AE 
		 * Compare AL with byte at ES:DI and set status flags
		 * SCAS m8  
		 * Compare AL with byte at ES:DI and set status flags
		 * SCASB    
		 */
		UNIMPLEMENTED(c, SST);

	}
	else
	{
		/* AE 
		 * Compare AL with byte at ES:EDI and set status flags
		 * SCAS m8  
		 * Compare AL with byte at ES:EDI and set status flags
		 * SCASB    
		 */
		enum emu_segment oldseg = emu_memory_segment_get(c->mem);
		emu_memory_segment_select(c->mem,s_es);

		uint8_t m8;
		MEM_BYTE_READ(c, c->reg[edi], &m8);

		emu_memory_segment_select(c->mem,oldseg);

		INSTR_CALC_AND_SET_FLAGS(8,
								 c,
								 *c->reg8[al],
								 m8)
		INSTR_CALC_EDI(c, 8)
		
	}
	return 0;
}


int32_t instr_scas_af(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	if ( i->prefixes & PREFIX_ADSIZE )
	{
		if ( i->prefixes & PREFIX_OPSIZE )
		{
			/* AF 
			 * Compare AX with word at ES:DI and set status flags
			 * SCAS m16 
			 * Compare AX with word at ES:DI and set status flags
			 * SCASW    
			 */
			UNIMPLEMENTED(c, SST);
		}
		else
		{
			/* AF 
			 * Compare EAX with doubleword at ES:DI and set status flags
			 * SCAS m32 
			 * Compare EAX with doubleword at ES:DI and set status flags
			 * SCASD    
			 */
			UNIMPLEMENTED(c, SST);
		}


	}else
	{
		if ( i->prefixes & PREFIX_OPSIZE )
		{
			/* AF 
			 * Compare AX with word at ES:EDI and set status flags
			 * SCAS m16 
			 * Compare AX with word at ES:EDI and set status flags
			 * SCASW    
			 */

			enum emu_segment oldseg = emu_memory_segment_get(c->mem);
			emu_memory_segment_select(c->mem,s_es);

			uint16_t m16;
			MEM_WORD_READ(c, c->reg[edi], &m16);

			emu_memory_segment_select(c->mem,oldseg);

			INSTR_CALC_AND_SET_FLAGS(8,
									 c,
									 *c->reg16[ax],
									 m16)

			INSTR_CALC_EDI(c, 16)
		}
		else
		{
			/* AF 
			 * Compare EAX with doubleword at ES:EDI and set status flags
			 * SCAS m32 
			 * Compare EAX with doubleword at ES:EDI and set status flags
			 * SCASD    
			 */

			enum emu_segment oldseg = emu_memory_segment_get(c->mem);
			emu_memory_segment_select(c->mem,s_es);

			uint32_t m32;
			MEM_DWORD_READ(c, c->reg[edi], &m32);

			emu_memory_segment_select(c->mem,oldseg);

			INSTR_CALC_AND_SET_FLAGS(32,
									 c,
									 c->reg[eax],
									 m32)

			INSTR_CALC_EDI(c, 32)
		}
	}
	return 0;
}
