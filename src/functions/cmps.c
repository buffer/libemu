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
UINTOF(bits) operand_a = a; \
UINTOF(bits) operand_b = b; \
UINTOF(bits) operation_result = operand_a - operand_b; 
//printf(" a %02x b %02x c %02x \n",operand_a, operand_b, operation_result);


#include "emu/emu.h"
#include "emu/emu_cpu.h"
#include "emu/emu_cpu_data.h"

#include "emu/emu_cpu_stack.h"
#include "emu/emu_memory.h"

/*Intel Architecture Software Developer's Manual Volume 2: Instruction Set Reference (24319102.PDF) page 127*/

#include <stdio.h>

#define INSTR_CALC_AND_SET_FLAGS(bits, cpu, a, b) \
INSTR_CALC(bits, a, b) \
INSTR_SET_FLAG_ZF(cpu) \
INSTR_SET_FLAG_PF(cpu) \
INSTR_SET_FLAG_SF(cpu) \
INSTR_SET_FLAG_CF(cpu, -) \
INSTR_SET_FLAG_OF(cpu, -,bits) 



#define INSTR_CALC_EDI_ESI(cpu,bits) \
{ \
	if ( !CPU_FLAG_ISSET(cpu,f_df) ) \
	{ \
		cpu->reg[edi]+=bits/8; \
		cpu->reg[esi]+=bits/8; \
	} \
	else \
	{ \
		cpu->reg[edi]-=bits/8; \
		cpu->reg[esi]-=bits/8; \
	} \
}



int32_t instr_cmps_a6(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	if ( i->prefixes & PREFIX_ADSIZE )
	{
		/* A6 
		 * Compares byte at address DS:SI with byte at address ES:DI and sets the status flags accordingly
		 * CMPSB         
		 * Compares byte at address DS:SI with byte at address ES:DI and sets the status flags accordingly
		 * CMPS m8, m8   
		 */
	}
	else
	{
		/* A6 
		 * Compares byte at address DS:ESI with byte at address ES:EDI and sets the status flags accordingly
		 * CMPSB         
		 * Compares byte at address DS:ESI with byte at address ES:EDI and sets the status flags accordingly
		 * CMPS m8, m8   
		 */

		if ( i->prefixes & PREFIX_F3 )
		{
			/* F3 A6 
			 * Find nonmatching bytes in ES:[EDI] and DS:[ESI]
			 * REPE CMPS m8,m8    
			 */
			if ( c->reg[ecx] > 0 )
			{
				c->reg[ecx]--;
				c->repeat_current_instr = true;

				enum emu_segment oldseg = emu_memory_segment_get(c->mem);

				emu_memory_segment_select(c->mem,s_ds);

				uint8_t m8a;
				MEM_BYTE_READ(c, c->reg[esi], &m8a);

				emu_memory_segment_select(c->mem,s_es);
				uint8_t m8b;
				MEM_BYTE_READ(c, c->reg[edi], &m8b);

				emu_memory_segment_select(c->mem,oldseg);

				INSTR_CALC_AND_SET_FLAGS(8,
										 c,
										 m8a,
										 m8b)

				INSTR_CALC_EDI_ESI(c, 8)

				/* REPE */
				if ( CPU_FLAG_ISSET(c, f_zf) == 0 )
					c->repeat_current_instr = false;
			}
			else
			{
				c->repeat_current_instr = false;
			}
		
		}
		else
		{
			enum emu_segment oldseg = emu_memory_segment_get(c->mem);

			emu_memory_segment_select(c->mem,s_ds);

			uint8_t m8a;
			MEM_BYTE_READ(c, c->reg[esi], &m8a);

			emu_memory_segment_select(c->mem,s_es);
			uint8_t m8b;
			MEM_BYTE_READ(c, c->reg[edi], &m8b);

			emu_memory_segment_select(c->mem,oldseg);

			INSTR_CALC_AND_SET_FLAGS(8,
									 c,
									 m8a,
									 m8b)

			INSTR_CALC_EDI_ESI(c, 8)
			
		}
	}
	return 0;
}


int32_t instr_cmps_a7(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	if ( i->prefixes & PREFIX_ADSIZE )
	{
		if ( i->prefixes & PREFIX_OPSIZE )
		{

			/* A7 
			 * Compares word at address DS:SI with word at address ES:DI and sets the status flags accordingly
			 * CMPS m16, m16 
			 * Compares word at address DS:SI with word at address ES:DI and sets the status flags accordingly
			 * CMPSW         
			 */
		}
		else
		{

			/* A7 
			 * Compares doubleword at address DS:SI with doubleword at address ES:DI and sets the status flags accordingly
			 * CMPS m32, m32 
			 * CMPSD         
			 * Compares doubleword at address DS:SI with doubleword at address ES:DI and sets the status flags accordingly
			 */
		}
	}
	else
	{
		if ( i->prefixes & PREFIX_OPSIZE )
		{

			/* A7 
			 * Compares word at address DS:(E)SI with word at address ES:(E)DI and sets the status flags accordingly
			 * CMPS m16, m16 
			 * Compares word at address DS:(E)SI with word at address ES:(E)DI and sets the status flags accordingly
			 * CMPSW         
			 */

			enum emu_segment oldseg = emu_memory_segment_get(c->mem);

			emu_memory_segment_select(c->mem,s_ds);
			uint16_t m16a;
			MEM_WORD_READ(c, c->reg[esi], &m16a);

			emu_memory_segment_select(c->mem,s_es);
			uint16_t m16b;
			MEM_WORD_READ(c, c->reg[edi], &m16b);

			emu_memory_segment_select(c->mem,oldseg);

			INSTR_CALC_AND_SET_FLAGS(16,
									 c,
									 m16a,
									 m16b)

			INSTR_CALC_EDI_ESI(c, 16)

		}
		else
		{

			/* A7 
			 * Compares doubleword at address DS:(E)SI with doubleword at address ES:(E)DI and sets the status flags accordingly
			 * CMPS m32, m32 
			 * CMPSD         
			 * Compares doubleword at address DS:(E)SI with doubleword at address ES:(E)DI and sets the status flags accordingly
			 */

			enum emu_segment oldseg = emu_memory_segment_get(c->mem);

			emu_memory_segment_select(c->mem,s_ds);
			uint32_t m32a;
			MEM_DWORD_READ(c, c->reg[esi], &m32a);

			emu_memory_segment_select(c->mem,s_es);
			uint32_t m32b;
			MEM_DWORD_READ(c, c->reg[edi], &m32b);

			emu_memory_segment_select(c->mem,oldseg);

			INSTR_CALC_AND_SET_FLAGS(32,
									 c,
									 m32a,
									 m32b)

			INSTR_CALC_EDI_ESI(c, 32)

		}

	}
	return 0;
}
