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


#ifdef INSTR_CALC_AND_SET_FLAGS
#undef INSTR_CALC_AND_SET_FLAGS
#endif // INSTR_CALC_AND_SET_FLAGS

#define INSTR_CALC_AND_SET_FLAGS(bits, cpu, a, b, c, operation)	\
INSTR_CALC(bits, a, b, c, operation)								\
INSTR_SET_FLAG_ZF(cpu)											\
INSTR_SET_FLAG_PF(cpu)											\
INSTR_SET_FLAG_SF(cpu)											\
INSTR_SET_FLAG_CF(cpu, operation)								\
INSTR_SET_FLAG_OF(cpu, operation,bits)								


#define TRACK_INIT_ALL_FLAGS(instruction_p) \
TRACK_INIT_EFLAG(instruction_p, f_zf); \
TRACK_INIT_EFLAG(instruction_p, f_pf); \
TRACK_INIT_EFLAG(instruction_p, f_sf); \
TRACK_INIT_EFLAG(instruction_p, f_cf); \
TRACK_INIT_EFLAG(instruction_p, f_of); 


int32_t instr_add_00(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	TRACK_INIT_ALL_FLAGS(c->instr);

	/* 00 /r
	 * Add r8 to r/m8
	 * ADD r/m8,r8     
	 */
	if ( i->modrm.mod != 3 )
	{
		uint8_t dst;
		MEM_BYTE_READ(c, i->modrm.ea, &dst);
		INSTR_CALC_AND_SET_FLAGS(8, 
								 c, 
								 dst, 
								 *c->reg8[i->modrm.opc], 
								 dst, 
								 +)
		MEM_BYTE_WRITE(c, i->modrm.ea, dst);
	}
	else
	{
		INSTR_CALC_AND_SET_FLAGS(8, 
								 c, 
								 *c->reg8[i->modrm.rm], 
								 *c->reg8[i->modrm.opc], 
								 *c->reg8[i->modrm.rm], 
								 +)
	}

	return 0;
}

int32_t instr_add_01(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	TRACK_INIT_ALL_FLAGS(c->instr);

	if ( i->modrm.mod != 3 )
	{
		if ( i->prefixes & PREFIX_OPSIZE )
		{
			uint16_t dst;
			MEM_WORD_READ(c, i->modrm.ea, &dst);
			/* 01 /r
			 * Add r16 to r/m16
			 * ADD r/m16,r16   
			 */
			INSTR_CALC_AND_SET_FLAGS(16, 
									 c, 
									 dst, 
									 *c->reg16[i->modrm.opc], 
									 dst, 
									 +)
			MEM_WORD_WRITE(c, i->modrm.ea, dst);
		}
		else
		{
			/* 01 /r
			 * Add r32 to r/m32
			 * ADD r/m32,r32   
			 */
			uint32_t dst;
			MEM_DWORD_READ(c, i->modrm.ea, &dst);
			INSTR_CALC_AND_SET_FLAGS(32, 
									 c, 
									 dst, 
									 c->reg[i->modrm.opc], 
									 dst, 
									 +)
			MEM_DWORD_WRITE(c, i->modrm.ea, dst);
		}
	}
	else
	{
		if ( i->prefixes & PREFIX_OPSIZE )
		{
			/* 01 /r
			 * Add r16 to r/m16
			 * ADD r/m16,r16   
			 */
			INSTR_CALC_AND_SET_FLAGS(16, 
									 c, 
									 *c->reg16[i->modrm.rm], 
									 *c->reg16[i->modrm.opc], 
									 *c->reg16[i->modrm.rm], 
									 +)
		}
		else
		{
			/* 01 /r
			 * Add r32 to r/m32
			 * ADD r/m32,r32   
			 */
			INSTR_CALC_AND_SET_FLAGS(32, 
									 c, 
									 c->reg[i->modrm.rm], 
									 c->reg[i->modrm.opc], 
									 c->reg[i->modrm.rm], 
									 +)
		}
	}

	return 0;
}

int32_t instr_add_02(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	TRACK_INIT_ALL_FLAGS(c->instr);

	/* 02 /r
	 * Add r/m8 to r8
	 * ADD r8,r/m8
	 */

	if ( i->modrm.mod != 3 )
	{
		uint8_t op;
		MEM_BYTE_READ(c, i->modrm.ea, &op);

		INSTR_CALC_AND_SET_FLAGS(8, 
								 c, 
								 op, 
								 *c->reg8[i->modrm.opc], 
								 *c->reg8[i->modrm.opc], 
								 +)
	}
	else
	{
//		*c->reg8[i->modrm.opc] += *c->reg8[i->modrm.rm];
		INSTR_CALC_AND_SET_FLAGS(8, 
								 c, 
								 *c->reg8[i->modrm.opc], 
								 *c->reg8[i->modrm.rm], 
								 *c->reg8[i->modrm.opc], 
								 +)
	}

	return 0;
}

int32_t instr_add_03(struct emu_cpu *c, struct emu_cpu_instruction *i)
{	
	TRACK_INIT_ALL_FLAGS(c->instr);

	if ( i->modrm.mod != 3 )
	{
		if ( i->prefixes & PREFIX_OPSIZE )
		{
			/* 03 /r
			 * Add r/m16 to r16
			 * ADD r16,r/m16   
			 */
			uint16_t op;
			MEM_WORD_READ(c, i->modrm.ea, &op);

			INSTR_CALC_AND_SET_FLAGS(16, 
									 c, 
									 op,
									 *c->reg16[i->modrm.opc], 
									 *c->reg16[i->modrm.opc], 
									 +)
		}
		else
		{
			/* 03 /r
			 * Add r/m32 to r32
			 * ADD r32,r/m32   
			 */
			uint32_t op;
			MEM_DWORD_READ(c, i->modrm.ea, &op);
			INSTR_CALC_AND_SET_FLAGS(32, 
									 c, 
									 op,
									 c->reg[i->modrm.opc], 
									 c->reg[i->modrm.opc], 
									 +)
		}
	}
	else
	{
		if ( i->prefixes & PREFIX_OPSIZE )
		{
			/* 03 /r
			 * Add r/m16 to r16
			 * ADD r16,r/m16   
			 */
			INSTR_CALC_AND_SET_FLAGS(16, 
									 c, 
									 *c->reg16[i->modrm.rm], 
									 *c->reg16[i->modrm.opc], 
									 *c->reg16[i->modrm.opc], 
									 +)
		}
		else
		{
			/* 03 /r
			 * Add r/m32 to r32
			 * ADD r32,r/m32   
			 */
			INSTR_CALC_AND_SET_FLAGS(32, 
									 c, 
									 c->reg[i->modrm.rm], 
									 c->reg[i->modrm.opc], 
									 c->reg[i->modrm.opc], 
									 +)
		}
	}

	return 0;
}

int32_t instr_add_04(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	TRACK_INIT_ALL_FLAGS(c->instr);

	/* 04 ib                    
	 * Add imm8 to AL
	 * ADD AL,imm8
	 */
	INSTR_CALC_AND_SET_FLAGS(8, 
							 c, 
							 *c->reg8[al], 
							 *i->imm8, 
							 *c->reg8[al], 
							 +)
	return 0;
}

int32_t instr_add_05(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	TRACK_INIT_ALL_FLAGS(c->instr);

	if ( i->prefixes & PREFIX_OPSIZE )
	{
		/* 05 iw
		 * Add imm16 to AX
		 * ADD AX,imm16
		 */
		INSTR_CALC_AND_SET_FLAGS(16, 
								 c, 
								 *c->reg16[ax], 
								 *i->imm16, 
								 *c->reg16[ax], 
								 +)
	}
	else
	{
		/* 05 id
		 * ADD EAX,imm32
		 * Add imm32 to EAX
		 */
		INSTR_CALC_AND_SET_FLAGS(32, 
								 c, 
								 c->reg[eax], 
								 i->imm, 
								 c->reg[eax], 
								 +)
	}

	return 0;
}


int32_t instr_group_1_80_add(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	TRACK_INIT_ALL_FLAGS(c->instr);

	if( i->modrm.mod != 3 )
	{
		uint8_t dst;
		MEM_BYTE_READ(c, i->modrm.ea, &dst);
		/* dst <-- dst <OPC> imm8 */

		INSTR_CALC_AND_SET_FLAGS(8, 
								 c, 
								 dst, 
								 *i->imm8, 
								 dst, 
								 +)

		MEM_BYTE_WRITE(c, i->modrm.ea, dst);
	}
	else
	{
		/* reg8[rm] <-- reg8[rm] <OPC> imm8 */
		INSTR_CALC_AND_SET_FLAGS(8, 
								 c, 
								 *c->reg8[i->modrm.rm], 
								 *i->imm8, 
								 *c->reg8[i->modrm.rm],
								 +)
	}

	return 0;
}






int32_t instr_group_1_81_add(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	TRACK_INIT_ALL_FLAGS(c->instr);

	if ( i->modrm.mod != 3 )
	{
		if ( i->prefixes & PREFIX_OPSIZE )
		{

			/* 81 /0 iw 
			 * Add imm16 to r/m16
			 * ADD r/m16,imm16 
			 */
			uint16_t dst;
			MEM_WORD_READ(c, i->modrm.ea, &dst);


			INSTR_CALC_AND_SET_FLAGS(16, 
									 c, 
									 dst, 
									 *i->imm16, 
									 dst, 
									 +)

			MEM_WORD_WRITE(c, i->modrm.ea, dst);
		}
		else
		{

			/* 81 /0 id 
			 * Add imm32 to r/m32
			 * ADD r/m32,imm32 
			 */
			uint32_t dst;
			MEM_DWORD_READ(c, i->modrm.ea, &dst);


			INSTR_CALC_AND_SET_FLAGS(32, 
									 c, 
									 dst, 
									 i->imm, 
									 dst, 
									 +)

			MEM_DWORD_WRITE(c, i->modrm.ea, dst);
		}
	}
	else
	{
		if ( i->prefixes & PREFIX_OPSIZE )
		{

			/* 81 /0 iw 
			 * Add imm16 to r/m16
			 * ADD r/m16,imm16 
			 */
			INSTR_CALC_AND_SET_FLAGS(16, 
									 c, 
									 *c->reg16[i->modrm.rm], 
									 *i->imm16, 
									 *c->reg16[i->modrm.rm], 
									 +)

		}
		else
		{

			/* 81 /0 id 
			 * Add imm32 to r/m32
			 * ADD r/m32,imm32 
			 */
			INSTR_CALC_AND_SET_FLAGS(32, 
									 c, 
									 c->reg[i->modrm.rm], 
									 i->imm, 
									 c->reg[i->modrm.rm], 
									 +)

		}

	}
	return 0;
}


int32_t instr_group_1_83_add(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	TRACK_INIT_ALL_FLAGS(c->instr);

	if ( i->modrm.mod != 3 )
	{
		if ( i->prefixes & PREFIX_OPSIZE )
		{

			/* 83 /0 ib 
			 * Add sign-extended imm8 to r/m16
			 * ADD r/m16,imm8  
			 */
			int16_t sexd = (int8_t)*i->imm8;

			uint16_t dst;
			MEM_WORD_READ(c, i->modrm.ea, &dst);

			INSTR_CALC_AND_SET_FLAGS(16, 
									 c, 
									 dst,
									 sexd, 
									 dst, 
									 +)

			MEM_WORD_WRITE(c, i->modrm.ea, dst);

		}
		else
		{


			/* 83 /0 ib 
			 * Add sign-extended imm8 to r/m32
			 * ADD r/m32,imm8  
			 */
			int32_t sexd = (int8_t)*i->imm8;

			uint32_t dst;
			MEM_DWORD_READ(c, i->modrm.ea, &dst);

			INSTR_CALC_AND_SET_FLAGS(32, 
									 c, 
									 dst,
									 sexd, 
									 dst, 
									 +)
			MEM_DWORD_WRITE(c, i->modrm.ea, dst);

		}
	}
	else
	{
		if ( i->prefixes & PREFIX_OPSIZE )
		{

			/* 83 /0 ib 
			 * Add sign-extended imm8 to r/m16
			 * ADD r/m16,imm8  
			 */
			int16_t sexd = (int8_t)*i->imm8;
			INSTR_CALC_AND_SET_FLAGS(16, 
									 c, 
									 *c->reg16[i->modrm.rm], 
									 sexd, 
									 *c->reg16[i->modrm.rm], 
									 +)

		}
		else
		{


			/* 83 /0 ib 
			 * Add sign-extended imm8 to r/m32
			 * ADD r/m32,imm8  
			 */
			int32_t sexd = (int8_t)*i->imm8;
			INSTR_CALC_AND_SET_FLAGS(32, 
									 c, 
									 c->reg[i->modrm.rm], 
									 sexd, 
									 c->reg[i->modrm.rm], 
									 +)

		}

	}
	return 0;
}
