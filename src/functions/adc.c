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
#include <stdio.h>


#if BYTE_ORDER == BIG_ENDIAN 
#define INSTR_CALC(bits, a, b, c, operation, cpu)			\
UINTOF(bits) operand_a; \
UINTOF(bits) operand_b; \
bcopy(&(a), &operand_a, bits/8); \
bcopy(&(b), &operand_b, bits/8); \
UINTOF(bits) operation_result = operand_a operation operand_b operation ((cpu->eflags & (1 << f_cf))?1:0);	\
bcopy(&operation_result, &(c), bits/8); 
#else // ENDIAN
#define INSTR_CALC(bits, a, b, c, operation, cpu)			\
UINTOF(bits) operand_a = a;								\
UINTOF(bits) operand_b = b;								\
UINTOF(bits) operation_result = operand_a operation operand_b operation ((cpu->eflags & (1 << f_cf))?1:0);	\
c = operation_result;
#endif // ENDIAN


#define INSTR_SET_FLAG_OF(cpu, operand,bits)											\
{																				\
	int64_t sx = (INTOF(bits))operand_a;                                            \
	int64_t sy = (INTOF(bits))operand_b;                                            \
	int64_t sz = 0;                                                             \
																				\
	sz = sx operand sy operand ((cpu->eflags & (1 << f_cf))?1:0);						\
	/*printf("of: sx %lli + sy %lli + cf %i = sz %lli \n", sx, sy, (cpu->eflags & (1 << f_cf))?1:0, sz);*/\
																			\
	if (sz < max_inttype_borders[sizeof(operation_result)][0][0] || sz > max_inttype_borders[sizeof(operation_result)][0][1] \
	|| sz != (INTOF(bits))operation_result )									    \
	{                                                                           \
		CPU_FLAG_SET(cpu, f_of);                                                 \
	}else                                                                       \
	{                                                                           \
		CPU_FLAG_UNSET(cpu, f_of);                                               \
	}                                                                           \
}

#define INSTR_SET_FLAG_CF(cpu, operand)											\
{																				\
	uint64_t ux = (uint64_t)operand_a;                                          \
	uint64_t uy = (uint64_t)operand_b;                                          \
	uint64_t uz = 0;                                                            \
	uz = ux operand uy operand ((cpu->eflags & (1 << f_cf))?1:0);					\
	/*printf("cf: ux %lli + uy %lli + cf %i = uz %lli \n", ux, uy, (cpu->eflags & (1 << f_cf))?1:0, uz);*/\
																				\
	if (uz < max_inttype_borders[sizeof(operation_result)][1][0] || uz > max_inttype_borders[sizeof(operation_result)][1][1] \
	|| uz != (uint64_t)operation_result )									    \
	{                                                                           \
		CPU_FLAG_SET(cpu, f_cf);                                                 \
	}else                                                                       \
	{                                                                           \
		CPU_FLAG_UNSET(cpu, f_cf);                                               \
	}                                                                           \
}



#include "emu/emu_cpu.h"
#include "emu/emu_cpu_data.h"

#include "emu/emu_memory.h"


#ifdef INSTR_CALC_AND_SET_FLAGS
#undef INSTR_CALC_AND_SET_FLAGS
#endif // INSTR_CALC_AND_SET_FLAGS

#define INSTR_CALC_AND_SET_FLAGS(bits, cpu, a, b, c, operation)	\
INSTR_CALC(bits, a, b, c, operation, cpu)									\
INSTR_SET_FLAG_ZF(cpu)											\
INSTR_SET_FLAG_PF(cpu)											\
INSTR_SET_FLAG_SF(cpu)											\
INSTR_SET_FLAG_OF(cpu, operation, bits)						\
INSTR_SET_FLAG_CF(cpu, operation)



#define TRACK_INIT_ALL_FLAGS(instruction_p) \
TRACK_INIT_EFLAG(instruction_p, f_zf); \
TRACK_INIT_EFLAG(instruction_p, f_pf); \
TRACK_INIT_EFLAG(instruction_p, f_sf); \
TRACK_INIT_EFLAG(instruction_p, f_of); \
TRACK_INIT_EFLAG(instruction_p, f_cf); 


int32_t instr_adc_10(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	TRACK_INIT_ALL_FLAGS(c->instr);

	/* 10 /r
	 * Add with carry byte register to r/m8
	 * ADC r/m8,r8     
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

int32_t instr_adc_11(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	TRACK_INIT_ALL_FLAGS(c->instr);

	if ( i->modrm.mod != 3 )
	{
		if ( i->prefixes & PREFIX_OPSIZE )
		{
			/* 11 /r
			 * Add with carry r16 to r/m16
			 * ADC r/m16,r16   
			 */
			uint16_t dst;
			MEM_WORD_READ(c, i->modrm.ea, &dst);
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
			/* 11 /r
			 * Add with CF r32 to r/m32
			 * ADC r/m32,r32   
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
			/* 11 /r
			 * Add with carry r16 to r/m16
			 * ADC r/m16,r16   
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
			/* 11 /r
			 * Add with CF r32 to r/m32
			 * ADC r/m32,r32   
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

int32_t instr_adc_12(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	TRACK_INIT_ALL_FLAGS(c->instr);

	/* 12 /r
	 * Add with carry r/m8 to byte register
	 * ADC r8,r/m8     
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
		INSTR_CALC_AND_SET_FLAGS(8, 
								 c, 
								 *c->reg8[i->modrm.opc], 
								 *c->reg8[i->modrm.rm], 
								 *c->reg8[i->modrm.opc], 
								 +)
	}

	return 0;
}

int32_t instr_adc_13(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	TRACK_INIT_ALL_FLAGS(c->instr);

	if ( i->modrm.mod != 3 )
	{
		if ( i->prefixes & PREFIX_OPSIZE )
		{
			/* 13 /r
			 * Add with carry r/m16 to r16
			 * ADC r16,r/m16   
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
			/* 13 /r
			 * Add with CF r/m32 to r32
			 * ADC r32,r/m32   
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
			/* 13 /r
			 * Add with carry r/m16 to r16
			 * ADC r16,r/m16   
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
			/* 13 /r
			 * Add with CF r/m32 to r32
			 * ADC r32,r/m32   
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

int32_t instr_adc_14(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	TRACK_INIT_ALL_FLAGS(c->instr);

	/* 14 ib 
	 * Add with carry imm8 to AL
	 * ADC AL,imm8
	 */
	INSTR_CALC_AND_SET_FLAGS(8, 
							 c, 
							 *c->reg8[al], 
							 *i->imm8, 
							 *c->reg8[al], 
							 +)
	return 0;
}

int32_t instr_adc_15(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	TRACK_INIT_ALL_FLAGS(c->instr);

	if ( i->prefixes & PREFIX_OPSIZE )
	{
		/* 15 iw
		 * Add with carry imm16 to AX
		 * ADC AX,imm16
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
		/* 15 id 
		 * Add with carry imm32 to EAX
		 * ADC EAX,imm32
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


int32_t instr_group_1_80_adc(struct emu_cpu *c, struct emu_cpu_instruction *i)
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



int32_t instr_group_1_81_adc(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	TRACK_INIT_ALL_FLAGS(c->instr);

	if ( i->modrm.mod != 3 )
	{
		if ( i->prefixes & PREFIX_OPSIZE )
		{

			/* 81 /2 iw 
			 * Add with carry imm16 to r/m16
			 * ADC r/m16,imm16 
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

			/* 81 /2 id 
			 * Add with CF imm32 to r/m32
			 * ADC r/m32,imm32 
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

			/* 81 /2 iw 
			 * Add with carry imm16 to r/m16
			 * ADC r/m16,imm16 
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

			/* 81 /2 id 
			 * Add with CF imm32 to r/m32
			 * ADC r/m32,imm32 
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

int32_t instr_group_1_83_adc(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	TRACK_INIT_ALL_FLAGS(c->instr);

	if ( i->modrm.mod != 3 )
	{
		if ( i->prefixes & PREFIX_OPSIZE )
		{


			/* 83 /2 ib 
			 * Add with CF sign-extended imm8 to r/m16
			 * ADC r/m16,imm8  
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
			/* 83 /2 ib 
			 * Add with CF sign-extended imm8 into r/m32
			 * ADC r/m32,imm8  
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


			/* 83 /2 ib 
			 * Add with CF sign-extended imm8 to r/m16
			 * ADC r/m16,imm8  
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
			/* 83 /2 ib 
			 * Add with CF sign-extended imm8 into r/m32
			 * ADC r/m32,imm8  
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
