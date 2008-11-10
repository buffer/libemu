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
UINTOF(bits) operation_result = operand_a & operand_b;	


#include "emu/emu.h"
#include "emu/emu_cpu.h"
#include "emu/emu_cpu_data.h"

#include "emu/emu_cpu_stack.h"
#include "emu/emu_memory.h"

/*Intel Architecture Software Developer's Manual Volume 2: Instruction Set Reference (24319102.PDF) page 728*/

#define INSTR_CALC_AND_SET_FLAGS(bits, cpu, a, b)	\
INSTR_CALC(bits, a, b)					\
INSTR_SET_FLAG_ZF(cpu)											\
INSTR_SET_FLAG_PF(cpu)											\
INSTR_SET_FLAG_SF(cpu)											\
CPU_FLAG_UNSET(cpu,f_cf);										\
CPU_FLAG_UNSET(cpu,f_of);



int32_t instr_test_84(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	if ( i->modrm.mod != 3)
	{
		/* 84 /r    
		 * AND r8 with r/m8; set SF, ZF, PF according to result
		 * TEST r/m8,r8     
		 */
		uint8_t m8;
		MEM_BYTE_READ(c, i->modrm.ea, &m8);
		INSTR_CALC_AND_SET_FLAGS(8,
								 c,
								 *c->reg8[i->modrm.opc],
								 m8)
	}else
	{
		/* 84 /r    
		 * AND r8 with r/m8; set SF, ZF, PF according to result
		 * TEST r/m8,r8     
		 */
		INSTR_CALC_AND_SET_FLAGS(8,
								 c,
								 *c->reg8[i->modrm.opc],
								 *c->reg8[i->modrm.rm])
	}
	return 0;
}

int32_t instr_test_85(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	if ( i->modrm.mod != 3)
	{
		if ( i->prefixes & PREFIX_OPSIZE )
		{

			/* 85 /r    
			 * AND r16 with r/m16; set SF, ZF, PF according to result
			 * TEST r/m16,r16   
			 */
			uint16_t m16;
			MEM_WORD_READ(c, i->modrm.ea, &m16);
			INSTR_CALC_AND_SET_FLAGS(16,
									 c,
									 *c->reg16[i->modrm.opc],
									 m16)

		}
		else
		{

			/* 85 /r    
			 * AND r32 with r/m32; set SF, ZF, PF according to result
			 * TEST r/m32,r32   
			 */
			uint32_t m32;
			MEM_DWORD_READ(c, i->modrm.ea, &m32);
			INSTR_CALC_AND_SET_FLAGS(32,
									 c,
									 c->reg[i->modrm.opc],
									 m32)

		}
	}
	else
	{
		if ( i->prefixes & PREFIX_OPSIZE )
		{

			/* 85 /r    
			 * AND r16 with r/m16; set SF, ZF, PF according to result
			 * TEST r/m16,r16   
			 */
			INSTR_CALC_AND_SET_FLAGS(16,
									 c,
									 *c->reg16[i->modrm.opc],
									 *c->reg16[i->modrm.rm])

		}
		else
		{

			/* 85 /r    
			 * AND r32 with r/m32; set SF, ZF, PF according to result
			 * TEST r/m32,r32   
			 */
			INSTR_CALC_AND_SET_FLAGS(32,
									 c,
									 c->reg[i->modrm.opc],
									 c->reg[i->modrm.rm])
		}
	}
	return 0;
}

int32_t instr_test_a8(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	/* A8 ib    
	 * AND imm8 with AL; set SF, ZF, PF according to result      
	 * TEST AL,imm8     
	 */
	INSTR_CALC_AND_SET_FLAGS(8,
							 c,
							 *i->imm8,
							 *c->reg8[al])

	return 0;
}

int32_t instr_test_a9(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	if ( i->prefixes & PREFIX_OPSIZE )
	{
		/* A9 iw    
		 * AND imm16 with AX; set SF, ZF, PF according to result     
		 * TEST AX,imm16    
		 */
		INSTR_CALC_AND_SET_FLAGS(16,
								 c,
								 *i->imm16,
								 *c->reg16[ax])

	}
	else
	{
		/* A9 id    
		 * AND imm32 with EAX; set SF, ZF, PF according to result    
		 * TEST EAX,imm32   
		 */
		INSTR_CALC_AND_SET_FLAGS(32,
								 c,
								 i->imm,
								 c->reg[eax])

	}
	return 0;
}

int32_t instr_group_3_f6_test(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	if ( i->modrm.mod != 3 )
	{
		/* F6 /0 ib 
		 * AND imm8 with r/m8; set SF, ZF, PF according to result
		 * TEST r/m8,imm8   
		 */
		uint8_t m8;
		MEM_BYTE_READ(c, i->modrm.ea, &m8);
		INSTR_CALC_AND_SET_FLAGS(8,
								 c,
								 *i->imm8,
								 m8)

	}
	else
	{
		/* F6 /0 ib 
		 * AND imm8 with r/m8; set SF, ZF, PF according to result
		 * TEST r/m8,imm8   
		 */
		INSTR_CALC_AND_SET_FLAGS(8,
								 c,
								 *i->imm8,
								 *c->reg8[i->modrm.rm])

	}
	return 0;
}


int32_t instr_group_3_f7_test(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	if ( i->modrm.mod != 3 )
	{
		if ( i->prefixes & PREFIX_OPSIZE )
		{
			/* F7 /0 iw 
			 * AND imm16 with r/m16; set SF, ZF, PF according to result
			 * TEST r/m16,imm16 
			 */
			uint16_t m16;
			MEM_WORD_READ(c, i->modrm.ea, &m16);
			INSTR_CALC_AND_SET_FLAGS(16,
									 c,
									 *i->imm16,
									 m16)

		}
		else
		{
			/* F7 /0 id 
			 * AND imm32 with r/m32; set SF, ZF, PF according to result
			 * TEST r/m32,imm32 
			 */
			uint32_t m32;
			MEM_DWORD_READ(c, i->modrm.ea, &m32);
			INSTR_CALC_AND_SET_FLAGS(32,
									 c,
									 i->imm,
									 m32)

		}
	}
	else
	{
		if ( i->prefixes & PREFIX_OPSIZE )
		{
			/* F7 /0 iw 
			 * AND imm16 with r/m16; set SF, ZF, PF according to result
			 * TEST r/m16,imm16 
			 */
			INSTR_CALC_AND_SET_FLAGS(16,
									 c,
									 *i->imm16,
									 *c->reg16[i->modrm.rm])
		}
		else
		{
			/* F7 /0 id 
			 * AND imm32 with r/m32; set SF, ZF, PF according to result
			 * TEST r/m32,imm32 
			 */
			INSTR_CALC_AND_SET_FLAGS(32,
									 c,
									 i->imm,
									 c->reg[i->modrm.rm])
		}
	}
	return 0;
}

