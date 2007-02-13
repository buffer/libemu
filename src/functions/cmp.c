/* @header@ */
#include <stdint.h>

#define INSTR_CALC(inttype, a, b, operation)			\
inttype operand_a = a;										\
inttype operand_b = b;										\
inttype operation_result = operand_a operation operand_b;	



#include "emu/emu_cpu.h"
#include "emu/emu_cpu_data.h"
#include "emu/emu_cpu_functions.h"
#include "emu/emu_memory.h"


#ifdef INSTR_CALC_AND_SET_FLAGS
#undef INSTR_CALC_AND_SET_FLAGS
#endif // INSTR_CALC_AND_SET_FLAGS

#define INSTR_CALC_AND_SET_FLAGS(inttype, cpu, a, b, operation)	\
INSTR_CALC(inttype, a, b, operation)								\
INSTR_SET_FLAG_ZF(cpu)											\
INSTR_SET_FLAG_PF(cpu)											\
INSTR_SET_FLAG_SF(cpu)											\
INSTR_SET_FLAG_CF(cpu, operation)								\
INSTR_SET_FLAG_OF(cpu, operation)								


int32_t instr_cmp_38(struct emu_cpu *c, struct instruction *i)
{
	/* 38 /r
	 * Compare r8 with r/m8
	 * CMP r/m8,r8 
	 */

	if ( i->modrm.mod != 3 )
	{
		uint8_t dst;
		MEM_BYTE_READ(c, i->modrm.ea, &dst);
		INSTR_CALC_AND_SET_FLAGS(uint8_t, 
								 c, 
								 dst, 
								 *c->reg8[i->modrm.opc], 
								 -)
		MEM_BYTE_WRITE(c, i->modrm.ea, dst);
	} else
	{
		INSTR_CALC_AND_SET_FLAGS(uint8_t, 
								 c, 
								 *c->reg8[i->modrm.rm], 
								 *c->reg8[i->modrm.opc], 
								 -)
	}
	return 0;
}

int32_t instr_cmp_39(struct emu_cpu *c, struct instruction *i)
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
			INSTR_CALC_AND_SET_FLAGS(uint16_t, 
									 c, 
									 dst, 
									 *c->reg16[i->modrm.opc], 
									 -)
			MEM_WORD_WRITE(c, i->modrm.ea, dst);
		} else
		{

			/* 39 /r
			 * Compare r32 with r/m32
			 * CMP r/m32,r32 
			 */

			uint32_t dst;
			MEM_DWORD_READ(c, i->modrm.ea, &dst);
			INSTR_CALC_AND_SET_FLAGS(uint32_t, 
									 c, 
									 dst, 
									 c->reg[i->modrm.opc], 
									 -)
			MEM_DWORD_WRITE(c, i->modrm.ea, dst);
		}
	} else
	{
		if ( i->prefixes & PREFIX_OPSIZE )
		{
			/* 39 /r
			 * Compare r16 with r/m16
			 * CMP r/m16,r16 
			 */
			INSTR_CALC_AND_SET_FLAGS(uint16_t, 
									 c, 
									 *c->reg16[i->modrm.rm], 
									 *c->reg16[i->modrm.opc], 
									 -)
		} else
		{
			/* 39 /r
			 * Compare r32 with r/m32
			 * CMP r/m32,r32 
			 */

			INSTR_CALC_AND_SET_FLAGS(uint32_t, 
									 c, 
									 c->reg[i->modrm.rm], 
									 c->reg[i->modrm.opc], 
									 -)
		}
	}

	return 0;
}

int32_t instr_cmp_3a(struct emu_cpu *c, struct instruction *i)
{      
	/* 3A /r
	 * Compare r/m8 with r8
	 * CMP r8,r/m8 
	 */


	if ( i->modrm.mod != 3 )
	{
		uint8_t op;
		MEM_BYTE_READ(c, i->modrm.ea, &op);

		INSTR_CALC_AND_SET_FLAGS(uint8_t, 
								 c, 
								 op, 
								 *c->reg8[i->modrm.opc], 
								 -)
	} else
	{
		INSTR_CALC_AND_SET_FLAGS(uint8_t, 
								 c, 
								 *c->reg8[i->modrm.opc], 
								 *c->reg8[i->modrm.rm], 
								 -)
	}

	return 0;
}

int32_t instr_cmp_3b(struct emu_cpu *c, struct instruction *i)
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
			INSTR_CALC_AND_SET_FLAGS(uint16_t, 
									 c, 
									 op,
									 *c->reg16[i->modrm.opc], 
									 -)
		} else
		{
			/* 3B /r
			 * Compare r/m32 with r32
			 * CMP r32,r/m32 
			 */
			uint32_t op;
			MEM_DWORD_READ(c, i->modrm.ea, &op);
			INSTR_CALC_AND_SET_FLAGS(uint32_t, 
									 c, 
									 op,
									 c->reg[i->modrm.opc], 
									 -)
		}
	} else
	{
		if ( i->prefixes & PREFIX_OPSIZE )
		{
			/* 3B /r
			 * Compare r/m16 with r16
			 * CMP r16,r/m16 
			 */
			INSTR_CALC_AND_SET_FLAGS(uint16_t, 
									 c, 
									 *c->reg16[i->modrm.rm], 
									 *c->reg16[i->modrm.opc], 
									 -)
		} else
		{
			/* 3B /r
			 * Compare r/m32 with r32
			 * CMP r32,r/m32 
			 */
			INSTR_CALC_AND_SET_FLAGS(uint32_t, 
									 c, 
									 c->reg[i->modrm.rm], 
									 c->reg[i->modrm.opc], 
									 -)
		}
	}

	return 0;
}

int32_t instr_cmp_3c(struct emu_cpu *c, struct instruction *i)
{
	/* 3C ib 
	 * Compare imm8 with AL
	 * CMP AL,imm8 
	 */
	INSTR_CALC_AND_SET_FLAGS(uint8_t, 
							 c, 
							 *c->reg8[al], 
							 *i->imm8, 
							 -)
	return 0;
}

int32_t instr_cmp_3d(struct emu_cpu *c, struct instruction *i)
{
	if ( i->prefixes & PREFIX_OPSIZE )
	{
		/* 3D iw 
		 * Compare imm16 with AX
		 * CMP AX,imm16  
		 */
		INSTR_CALC_AND_SET_FLAGS(uint16_t, 
								 c, 
								 *c->reg16[ax], 
								 *i->imm16, 
								 -)
	} else
	{
		/* 3D id 
		 * Compare imm32 with EAX
		 * CMP EAX,imm32 
		 */

		INSTR_CALC_AND_SET_FLAGS(uint32_t, 
								 c, 
								 c->reg[eax], 
								 i->imm, 
								 -)
	}



	return 0;
}

int32_t instr_group_1_80_cmp(struct emu_cpu *cpu, uint8_t a, uint8_t b)
{
	INSTR_CALC_AND_SET_FLAGS(uint8_t, 
							 cpu, 
							 a, 
							 b, 
							 -)
	return 0;
}

