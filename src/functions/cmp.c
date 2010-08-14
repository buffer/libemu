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

#include <sys/types.h>
#include <sys/param.h>
#include <stdint.h>

#if BYTE_ORDER == BIG_ENDIAN 
#define INSTR_CALC(bits, a, b, operation)			\
UINTOF(bits) operand_a; \
UINTOF(bits) operand_b; \
bcopy(&(a), &operand_a, bits/8); \
bcopy(&(b), &operand_b, bits/8); \
UINTOF(bits) operation_result = operand_a operation operand_b;
#else // ENDIAN
#define INSTR_CALC(bits, a, b, operation)			\
UINTOF(bits) operand_a = a;								\
UINTOF(bits) operand_b = b;								\
UINTOF(bits) operation_result = operand_a operation operand_b;	
#endif // ENDIAN


#include "emu/emu_cpu.h"
#include "emu/emu_cpu_data.h"

#include "emu/emu_memory.h"

#ifdef INSTR_CALC_AND_SET_FLAGS
#undef INSTR_CALC_AND_SET_FLAGS
#endif // INSTR_CALC_AND_SET_FLAGS

#define INSTR_CALC_AND_SET_FLAGS(bits, cpu, a, b, operation)	\
INSTR_CALC(bits, a, b, operation)								\
INSTR_SET_FLAG_ZF(cpu)											\
INSTR_SET_FLAG_PF(cpu)											\
INSTR_SET_FLAG_SF(cpu)											\
INSTR_SET_FLAG_CF(cpu, operation)								\
INSTR_SET_FLAG_OF(cpu, operation, bits)								


int32_t instr_cmp_38(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	/* 38 /r
	 * Compare r8 with r/m8
	 * CMP r/m8,r8 
	 */

	if ( i->modrm.mod != 3 )
	{
		uint8_t dst;
		MEM_BYTE_READ(c, i->modrm.ea, &dst);
		INSTR_CALC_AND_SET_FLAGS(8, 
								 c, 
								 dst, 
								 *c->reg8[i->modrm.opc], 
								 -)
//		MEM_BYTE_WRITE(c, i->modrm.ea, dst);
	}
	else
	{
		INSTR_CALC_AND_SET_FLAGS(8, 
								 c, 
								 *c->reg8[i->modrm.rm], 
								 *c->reg8[i->modrm.opc], 
								 -)
	}
	return 0;
}

int32_t instr_cmp_39(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	if ( i->modrm.mod != 3 )
	{
		if ( i->prefixes & PREFIX_OPSIZE )
		{
			/* 39 /r
			 * Compare r16 with r/m16
			 * CMP r/m16,r16 
			 */

			uint16_t dst;
			MEM_WORD_READ(c, i->modrm.ea, &dst);
			INSTR_CALC_AND_SET_FLAGS(16, 
									 c, 
									 dst, 
									 *c->reg16[i->modrm.opc], 
									 -)
//			MEM_WORD_WRITE(c, i->modrm.ea, dst);
		}
		else
		{

			/* 39 /r
			 * Compare r32 with r/m32
			 * CMP r/m32,r32 
			 */

			uint32_t dst;
			MEM_DWORD_READ(c, i->modrm.ea, &dst);
			INSTR_CALC_AND_SET_FLAGS(32, 
									 c, 
									 dst, 
									 c->reg[i->modrm.opc], 
									 -)
//			MEM_DWORD_WRITE(c, i->modrm.ea, dst);
		}
	}
	else
	{
		if ( i->prefixes & PREFIX_OPSIZE )
		{
			/* 39 /r
			 * Compare r16 with r/m16
			 * CMP r/m16,r16 
			 */
			INSTR_CALC_AND_SET_FLAGS(16, 
									 c, 
									 *c->reg16[i->modrm.rm], 
									 *c->reg16[i->modrm.opc], 
									 -)
		}
		else
		{
			/* 39 /r
			 * Compare r32 with r/m32
			 * CMP r/m32,r32 
			 */

			INSTR_CALC_AND_SET_FLAGS(32, 
									 c, 
									 c->reg[i->modrm.rm], 
									 c->reg[i->modrm.opc], 
									 -)
		}
	}

	return 0;
}

int32_t instr_cmp_3a(struct emu_cpu *c, struct emu_cpu_instruction *i)
{      
	/* 3A /r
	 * Compare r/m8 with r8
	 * CMP r8,r/m8 
	 */


	if ( i->modrm.mod != 3 )
	{
		uint8_t op;
		MEM_BYTE_READ(c, i->modrm.ea, &op);

		INSTR_CALC_AND_SET_FLAGS(8, 
								 c, 
                                 *c->reg8[i->modrm.opc], 
								 op, 
								 -)
	}
	else
	{
		INSTR_CALC_AND_SET_FLAGS(8, 
								 c, 
								 *c->reg8[i->modrm.opc], 
								 *c->reg8[i->modrm.rm], 
								 -)
	}

	return 0;
}

int32_t instr_cmp_3b(struct emu_cpu *c, struct emu_cpu_instruction *i)
{      

	if ( i->modrm.mod != 3 )
	{
		if ( i->prefixes & PREFIX_OPSIZE )
		{
			/* 3B /r
			 * Compare r/m16 with r16
			 * CMP r16,r/m16 
			 */
			uint16_t op;
			MEM_WORD_READ(c, i->modrm.ea, &op);
			INSTR_CALC_AND_SET_FLAGS(16, 
									 c, 
									 *c->reg16[i->modrm.opc], 
									 op,
									 -)
		}
		else
		{
			/* 3B /r
			 * Compare r/m32 with r32
			 * CMP r32,r/m32 
			 */
			uint32_t op;
			MEM_DWORD_READ(c, i->modrm.ea, &op);
			INSTR_CALC_AND_SET_FLAGS(32, 
									 c, 
									 c->reg[i->modrm.opc], 
									 op,
                                     -)
		}
	}
	else
	{
		if ( i->prefixes & PREFIX_OPSIZE )
		{
			/* 3B /r
			 * Compare r/m16 with r16
			 * CMP r16,r/m16 
			 */
			INSTR_CALC_AND_SET_FLAGS(16, 
									 c, 
									 *c->reg16[i->modrm.opc], 
									 *c->reg16[i->modrm.rm], 
                                     -)
		}
		else
		{
			/* 3B /r
			 * Compare r/m32 with r32
			 * CMP r32,r/m32 
			 */
			INSTR_CALC_AND_SET_FLAGS(32, 
									 c, 
									 c->reg[i->modrm.opc], 
									 c->reg[i->modrm.rm], 
									 -)
		}
	}

	return 0;
}

int32_t instr_cmp_3c(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	/* 3C ib 
	 * Compare imm8 with AL
	 * CMP AL,imm8 
	 */
	INSTR_CALC_AND_SET_FLAGS(8, 
							 c, 
							 *c->reg8[al], 
							 *i->imm8, 
							 -)
	return 0;
}

int32_t instr_cmp_3d(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	if ( i->prefixes & PREFIX_OPSIZE )
	{
		/* 3D iw 
		 * Compare imm16 with AX
		 * CMP AX,imm16  
		 */
		INSTR_CALC_AND_SET_FLAGS(16, 
								 c, 
								 *c->reg16[ax], 
								 *i->imm16, 
								 -)
	}
	else
	{
		/* 3D id 
		 * Compare imm32 with EAX
		 * CMP EAX,imm32 
		 */

		INSTR_CALC_AND_SET_FLAGS(32, 
								 c, 
								 c->reg[eax], 
								 i->imm, 
								 -)
	}



	return 0;
}

int32_t instr_group_1_80_cmp(struct emu_cpu *c, struct emu_cpu_instruction *i)
{

	if( i->modrm.mod != 3 )
	{
		uint8_t dst;
		MEM_BYTE_READ(c, i->modrm.ea, &dst);
		/* dst <-- dst <OPC> imm8 */

		INSTR_CALC_AND_SET_FLAGS(8, 
								 c, 
								 dst, 
								 *i->imm8, 
								 -)

		MEM_BYTE_WRITE(c, i->modrm.ea, dst);
	}
	else
	{
		/* reg8[rm] <-- reg8[rm] <OPC> imm8 */
		INSTR_CALC_AND_SET_FLAGS(8, 
								 c, 
								 *c->reg8[i->modrm.rm], 
								 *i->imm8, 
								 -)
	}

	return 0;
}


int32_t instr_group_1_81_cmp(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	if ( i->modrm.mod != 3 )
	{
		if ( i->prefixes & PREFIX_OPSIZE )
		{
			/* 81 /7 iw 
			 * Compare imm16 with r/m16
			 * CMP r/m16, imm16 
			 */

			uint16_t dst;
			MEM_WORD_READ(c, i->modrm.ea, &dst);


			INSTR_CALC_AND_SET_FLAGS(16, 
									 c, 
									 dst, 
									 *i->imm16, 
									 -)
		}
		else
		{
			/* 81 /7 id 
			 * Compare imm32 with r/m32
			 * CMP r/m32,imm32  
			 */
			uint32_t dst;
			MEM_DWORD_READ(c, i->modrm.ea, &dst);

			INSTR_CALC_AND_SET_FLAGS(32, 
									 c, 
									 dst, 
									 i->imm, 
									 -)
		}
	}
	else
	{
		if ( i->prefixes & PREFIX_OPSIZE )
		{
			/* 81 /7 iw 
			 * Compare imm16 with r/m16
			 * CMP r/m16, imm16 
			 */
			INSTR_CALC_AND_SET_FLAGS(16, 
									 c, 
									 *c->reg16[i->modrm.rm], 
									 *i->imm16, 
									 -)

		}
		else
		{
			/* 81 /7 id 
			 * Compare imm32 with r/m32
			 * CMP r/m32,imm32  
			 */
			INSTR_CALC_AND_SET_FLAGS(32, 
									 c, 
									 c->reg[i->modrm.rm], 
									 i->imm, 
									 -)

		}

	}
	return 0;
}


int32_t instr_group_1_83_cmp(struct emu_cpu *c, struct emu_cpu_instruction *i)
{

/* As the INSTR_CALC for big endian uses bcopy of the operands size, we have to create an operand of the size 
 * and use it, the replacement aligned equal size operand is called imm 
 */
#if BYTE_ORDER == BIG_ENDIAN
	 uint8_t imm8;
	 bcopy(i->imm8, &imm8, 1);
#endif

	if ( i->modrm.mod != 3 )
	{
		if ( i->prefixes & PREFIX_OPSIZE )
		{

			/* 83 /7 ib 
			 * Compare imm8 with r/m16
			 * CMP r/m16,imm8   
			 */

			 uint16_t dst;
			 MEM_WORD_READ(c, i->modrm.ea, &dst);

#if BYTE_ORDER == BIG_ENDIAN
			int16_t sexd = (int8_t) imm8;
#else
			int16_t sexd = (int8_t) *i->imm8;
#endif

			 INSTR_CALC_AND_SET_FLAGS(16, 
									  c, 
									  dst,
									  sexd,
									  -)
		}
		else
		{


			/* 83 /7 ib 
			 * Compare imm8 with r/m32
			 * CMP r/m32,imm8   
			 */

			uint32_t dst;
			MEM_DWORD_READ(c, i->modrm.ea, &dst);

#if BYTE_ORDER == BIG_ENDIAN
			int32_t sexd = (int8_t) imm8;
#else
			int32_t sexd = (int8_t) *i->imm8;
#endif

			INSTR_CALC_AND_SET_FLAGS(32, 
									 c, 
									 dst,
									 sexd,
									 -)
		}
	}
	else
	{
		if ( i->prefixes & PREFIX_OPSIZE )
		{
			/* 83 /7 ib 
			 * Compare imm8 with r/m16
			 * CMP r/m16,imm8   
			 */
			
#if BYTE_ORDER == BIG_ENDIAN
			int16_t sexd = (int8_t) imm8;
#else
			int16_t sexd = (int8_t) *i->imm8;
#endif
						
			INSTR_CALC_AND_SET_FLAGS(16, 
									 c, 
									 *c->reg16[i->modrm.rm],
									 sexd,
									 -)
		}
		else
		{
			/* 83 /7 ib 
			 * Compare imm8 with r/m32
			 * CMP r/m32,imm8   
			 */

#if BYTE_ORDER == BIG_ENDIAN
			int32_t sexd = (int8_t) imm8;
#else
			int32_t sexd = (int8_t) *i->imm8;
#endif

            INSTR_CALC_AND_SET_FLAGS(32, 
									 c, 
									 c->reg[i->modrm.rm], 
									 sexd,
									 -)
		}
	}
	return 0;
}
