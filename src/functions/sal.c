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

#define INSTR_CALC(bits, a, b, cpu) \
UINTOF(bits) operation_result = (a); \
uint8_t operand_b = (b); \
{ \
	operand_b &= 0x1f; \
	if( operand_b > 0 ) \
	{ \
		if( operation_result & (1 << (bits - operand_b)) ) \
		{ \
			CPU_FLAG_SET(cpu, f_cf); \
		} \
		operation_result <<= operand_b; \
		if( operand_b == 1 ) \
		{ \
			if( (operation_result >> (bits - 1)) ^ (cpu->eflags >> f_cf) ) \
			{ \
				CPU_FLAG_SET(cpu, f_of); \
			} \
			else \
			{ \
				CPU_FLAG_UNSET(cpu, f_of); \
			} \
		} \
		a = operation_result; \
	} \
}

#define INSTR_CALC_AND_SET_FLAGS(bits, cpu, a, b) \
INSTR_CALC(bits, a, b, cpu) \
if (b > 0) \
{ \
	INSTR_SET_FLAG_ZF(cpu) \
	INSTR_SET_FLAG_PF(cpu) \
	INSTR_SET_FLAG_SF(cpu) \
}

#include "emu/emu.h"
#include "emu/emu_cpu.h"
#include "emu/emu_cpu_data.h"

#include "emu/emu_cpu_stack.h"
#include "emu/emu_memory.h"

/*Intel Architecture Software Developer's Manual Volume 2: Instruction Set Reference (24319102.PDF) page 662*/


int32_t instr_group_2_c0_sal(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	if ( i->modrm.mod != 3 )
	{

		/* C0 /4 ib 
		 * Multiply r/m8 by 2, imm8 times
		 * SAL r/m8,imm8  
		 */      
		uint8_t m8;
		MEM_BYTE_READ(c, i->modrm.ea, &m8);
		INSTR_CALC_AND_SET_FLAGS(8,
								 c,
								 m8,
								 *i->imm8);

		MEM_BYTE_WRITE(c, i->modrm.ea, m8);     
	}
	else
	{
		/* C0 /4 ib 
		 * Multiply r/m8 by 2, imm8 times
		 * SAL r/m8,imm8  
		 */      
		INSTR_CALC_AND_SET_FLAGS(8,
								 c,
								 *c->reg8[i->modrm.rm],
								 *i->imm8);

	}

	return 0;
}

int32_t instr_group_2_c1_sal(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	if ( i->modrm.mod != 3 )
	{
		if ( i->prefixes & PREFIX_OPSIZE )
		{
			/* C1 /4 ib 
			 * Multiply r/m16 by 2, imm8 times
			 * SAL r/m16,imm8 
			 */
			uint16_t m16;
			MEM_WORD_READ(c, i->modrm.ea, &m16);
			INSTR_CALC_AND_SET_FLAGS(16,
									 c,
									 m16,
									 *i->imm8);

			MEM_WORD_WRITE(c, i->modrm.ea, m16);        

		}
		else
		{
			/* C1 /4 ib 
			 * Multiply r/m32 by 2, imm8 times
			 * SAL r/m32,imm8 
			 */
			uint32_t m32;
			MEM_DWORD_READ(c, i->modrm.ea, &m32);
			INSTR_CALC_AND_SET_FLAGS(32,
									 c,
									 m32,
									 *i->imm8);
			MEM_DWORD_WRITE(c, i->modrm.ea, m32);       
		}
	}
	else
	{
		if ( i->prefixes & PREFIX_OPSIZE )
		{
			/* C1 /4 ib 
			 * Multiply r/m16 by 2, imm8 times
			 * SAL r/m16,imm8 
			 */
			INSTR_CALC_AND_SET_FLAGS(16,
									 c,
									 *c->reg16[i->modrm.rm],
									 *i->imm8);

		}
		else
		{

			/* C1 /4 ib 
			 * Multiply r/m32 by 2, imm8 times
			 * SAL r/m32,imm8 
			 */
			INSTR_CALC_AND_SET_FLAGS(32,
									 c,
									 c->reg[i->modrm.rm],
									 *i->imm8);

		}
	}

	return 0;
}

int32_t instr_group_2_d0_sal(struct emu_cpu *c, struct emu_cpu_instruction *i)
{

	if ( i->modrm.mod != 3 )
	{
		/* D0 /4	
		 * Multiply r/m8 by 2, once          
		 * SAL r/m8,1     
		 */
		uint8_t m8;
		MEM_BYTE_READ(c, i->modrm.ea, &m8);
		INSTR_CALC_AND_SET_FLAGS(8,
								 c,
								 m8,
								 1);

		MEM_BYTE_WRITE(c, i->modrm.ea, m8);     
	}
	else
	{
		/* D0 /4	
		 * Multiply r/m8 by 2, once          
		 * SAL r/m8,1     
		 */
		INSTR_CALC_AND_SET_FLAGS(8,
								 c,
								 *c->reg8[i->modrm.rm],
								 1);

	}

	return 0;
}

int32_t instr_group_2_d1_sal(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	if ( i->modrm.mod != 3 )
	{
		if ( i->prefixes & PREFIX_OPSIZE )
		{
			/* D1 /4	
			 * Multiply r/m16 by 2, once               
			 * SAL r/m16,1    
			 */
			uint16_t m16;
			MEM_WORD_READ(c, i->modrm.ea, &m16);
			INSTR_CALC_AND_SET_FLAGS(16,
									 c,
									 m16,
									 1);

			MEM_WORD_WRITE(c, i->modrm.ea, m16);        

		}
		else
		{
			/* D1 /4	
			 * Multiply r/m32 by 2, once             
			 * SAL r/m32,1    
			 */
			uint32_t m32;
			MEM_DWORD_READ(c, i->modrm.ea, &m32);
			INSTR_CALC_AND_SET_FLAGS(32,
									 c,
									 m32,
									 1);
			MEM_DWORD_WRITE(c, i->modrm.ea, m32);       
		}
	}
	else
	{
		if ( i->prefixes & PREFIX_OPSIZE )
		{
			/* D1 /4	
			 * Multiply r/m16 by 2, once               
			 * SAL r/m16,1    
			 */
			INSTR_CALC_AND_SET_FLAGS(16,
									 c,
									 *c->reg16[i->modrm.rm],
									 1);

		}
		else
		{
			/* D1 /4	
			 * Multiply r/m32 by 2, once             
			 * SAL r/m32,1    
			 */
			INSTR_CALC_AND_SET_FLAGS(32,
									 c,
									 c->reg[i->modrm.rm],
									 1);

		}
	}

	return 0;
}

int32_t instr_group_2_d2_sal(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	if ( i->modrm.mod != 3 )
	{
		/* D2 /4   
		 * Multiply r/m8 by 2, CL times      
		 * SAL r/m8,CL    
		 */
		uint8_t m8;
		MEM_BYTE_READ(c, i->modrm.ea, &m8);
		INSTR_CALC_AND_SET_FLAGS(8,
								 c,
								 m8,
								 *c->reg8[cl]);

		MEM_BYTE_WRITE(c, i->modrm.ea, m8);     
	}
	else
	{
		/* D2 /4   
		 * Multiply r/m8 by 2, CL times      
		 * SAL r/m8,CL    
		 */
		INSTR_CALC_AND_SET_FLAGS(8,
								 c,
								 *c->reg8[i->modrm.rm],
								 *c->reg8[cl]);
	}

	return 0;
}


int32_t instr_group_2_d3_sal(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	if ( i->modrm.mod != 3 )
	{
		if ( i->prefixes & PREFIX_OPSIZE )
		{
			/* D3 /4   
			 * Multiply r/m16 by 2, CL times           
			 * SAL r/m16,CL   
			 */
			uint16_t m16;
			MEM_WORD_READ(c, i->modrm.ea, &m16);
			INSTR_CALC_AND_SET_FLAGS(16,
									 c,
									 m16,
									 *c->reg8[cl]);

			MEM_WORD_WRITE(c, i->modrm.ea, m16);        
		}
		else
		{
			/* D3 /4   
			 * Multiply r/m32 by 2, CL times         
			 * SAL r/m32,CL   
			 */
			uint32_t m32;
			MEM_DWORD_READ(c, i->modrm.ea, &m32);
			INSTR_CALC_AND_SET_FLAGS(32,
									 c,
									 m32,
									 *c->reg8[cl]);

			MEM_DWORD_WRITE(c, i->modrm.ea, m32);       
		}
	}
	else
	{
		if ( i->prefixes & PREFIX_OPSIZE )
		{
			/* D3 /4   
			 * Multiply r/m16 by 2, CL times           
			 * SAL r/m16,CL   
			 */
			INSTR_CALC_AND_SET_FLAGS(16,
									 c,
									 *c->reg16[i->modrm.rm],
									 *c->reg8[cl]);
		}
		else
		{
			/* D3 /4   
			 * Multiply r/m32 by 2, CL times         
			 * SAL r/m32,CL   
			 */
			INSTR_CALC_AND_SET_FLAGS(32,
									 c,
									 c->reg[i->modrm.rm],
									 *c->reg8[cl]);
		}
	}

	return 0;
}
