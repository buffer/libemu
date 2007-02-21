/* @header@ */
#include <stdint.h>

// FIXME replace the INSTR_CALC macro and verify the flags

#include "emu/emu_cpu.h"
#include "emu/emu_cpu_data.h"
#include "emu/emu_cpu_functions.h"
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
INSTR_SET_FLAG_OF(cpu, operation, bits)								


int32_t instr_sub_28(struct emu_cpu *c, struct instruction *i)
{
	/*
	 * 28 /r
	 * Subtract r8 from r/m8
	 * SUB r/m8,r8 
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
								 -)
		MEM_BYTE_WRITE(c, i->modrm.ea, dst);
	} else
	{
		INSTR_CALC_AND_SET_FLAGS(8, 
								 c, 
								 *c->reg8[i->modrm.rm], 
								 *c->reg8[i->modrm.opc], 
								 *c->reg8[i->modrm.rm], 
								 -)
	}

	return 0;
}

int32_t instr_sub_29(struct emu_cpu *c, struct instruction *i)
{      
	if ( i->modrm.mod != 3 )
	{
		if ( i->prefixes & PREFIX_OPSIZE )
		{
			/* 29 /r
			 * Subtract r16 from r/m16
			 * SUB r/m16,r16 
			 */

			uint16_t dst;
			MEM_WORD_READ(c, i->modrm.ea, &dst);
			INSTR_CALC_AND_SET_FLAGS(16, 
									 c, 
									 dst, 
									 *c->reg16[i->modrm.opc], 
									 dst, 
									 -)
			MEM_WORD_WRITE(c, i->modrm.ea, dst);
		} else
		{
			/* 29 /r
			 * Subtract r32 from r/m32
			 * SUB r/m32,r32 
			 */

			uint32_t dst;
			MEM_DWORD_READ(c, i->modrm.ea, &dst);
			INSTR_CALC_AND_SET_FLAGS(32, 
									 c, 
									 dst, 
									 c->reg[i->modrm.opc], 
									 dst, 
									 -)
			MEM_DWORD_WRITE(c, i->modrm.ea, dst);
		}
	} else
	{
		if ( i->prefixes & PREFIX_OPSIZE )
		{
			/* 29 /r
			 * Subtract r16 from r/m16
			 * SUB r/m16,r16 
			 */

			INSTR_CALC_AND_SET_FLAGS(16, 
									 c, 
									 *c->reg16[i->modrm.rm], 
									 *c->reg16[i->modrm.opc], 
									 *c->reg16[i->modrm.rm], 
									 -)
		} else
		{
			/* 29 /r
			 * Subtract r32 from r/m32
			 * SUB r/m32,r32 
			 */

			INSTR_CALC_AND_SET_FLAGS(32, 
									 c, 
									 c->reg[i->modrm.rm], 
									 c->reg[i->modrm.opc], 
									 c->reg[i->modrm.rm], 
									 -)
		}
	}

	return 0;
}

int32_t instr_sub_2a(struct emu_cpu *c, struct instruction *i)
{
	 /* 2A /r
	  * Subtract r/m8 from r8
	  * SUB r8,r/m8 
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
								 -)
	} else
	{
		INSTR_CALC_AND_SET_FLAGS(8, 
								 c, 
								 *c->reg8[i->modrm.opc], 
								 *c->reg8[i->modrm.rm], 
								 *c->reg8[i->modrm.opc], 
								 -)
	}

	return 0;
}

int32_t instr_sub_2b(struct emu_cpu *c, struct instruction *i)
{
	if ( i->modrm.mod != 3 )
	{
		if ( i->prefixes & PREFIX_OPSIZE )
		{
			/*
			* 2B /r
			* Subtract r/m16 from r16
			* SUB r16,r/m16
			*/

			uint16_t op;
			MEM_WORD_READ(c, i->modrm.ea, &op);

			INSTR_CALC_AND_SET_FLAGS(16, 
									 c, 
									 *c->reg16[i->modrm.opc], 
									 op,
									 *c->reg16[i->modrm.opc], 
									 -)
		} else
		{
			/*
			 * 2B /r
			 * Subtract r/m32 from r32
			 * SUB r32,r/m32 
			 */

			uint32_t op;
			MEM_DWORD_READ(c, i->modrm.ea, &op);
			INSTR_CALC_AND_SET_FLAGS(32, 
									 c, 
									 c->reg[i->modrm.opc], 
									 op,
									 c->reg[i->modrm.opc], 
									 -)
		}
	} else
	{
		if ( i->prefixes & PREFIX_OPSIZE )
		{

			/*
			* 2B /r
			* Subtract r/m16 from r16
			* SUB r16,r/m16
			*/

			INSTR_CALC_AND_SET_FLAGS(16, 
									 c, 
									 *c->reg16[i->modrm.opc], 
									 *c->reg16[i->modrm.rm], 
									 *c->reg16[i->modrm.opc], 
									 -)
		} else
		{
			/*
			 * 2B /r
			 * Subtract r/m32 from r32
			 * SUB r32,r/m32 
			 */

			INSTR_CALC_AND_SET_FLAGS(32, 
									 c, 
									 c->reg[i->modrm.opc], 
									 c->reg[i->modrm.rm], 
									 c->reg[i->modrm.opc], 
									 -)
		}
	}

	return 0;
}

int32_t instr_sub_2c(struct emu_cpu *c, struct instruction *i)
{
	/* 2C ib
	 * Subtract imm8 from AL
     * SUB AL,imm8
	 */

	INSTR_CALC_AND_SET_FLAGS(8, 
							 c, 
							 *c->reg8[al], 
							 *i->imm8, 
							 *c->reg8[al], 
							 -)
	return 0;
}

int32_t instr_sub_2d(struct emu_cpu *c, struct instruction *i)
{
	if ( i->prefixes & PREFIX_OPSIZE )
	{ 
		/* 2D iw
		 * Subtract imm16 from AX  
		 * SUB AX,imm16
		 */
	  
		INSTR_CALC_AND_SET_FLAGS(16, 
								 c, 
								 *c->reg16[ax], 
								 *i->imm16, 
								 *c->reg16[ax], 
								 -)
	} else
	{
		/* 2D id
		 * Subtract imm32 from EAX
		 * SUB EAX,imm32
		 */

		INSTR_CALC_AND_SET_FLAGS(32, 
								 c, 
								 c->reg[eax], 
								 i->imm, 
								 c->reg[eax], 
								 -)
	}

	return 0;
}

int32_t instr_group_1_80_sub(struct emu_cpu *cpu, uint8_t a, uint8_t b, uint8_t *result)
{
	INSTR_CALC_AND_SET_FLAGS(8, 
							 cpu, 
							 a, 
							 b, 
							 *result, 
							 -)
	return 0;
}

int32_t instr_group_1_81_sub(struct emu_cpu *cpu, uint8_t a, uint8_t b, uint8_t *result)
{
	/* 81 /5 iw 
	 * Subtract imm16 from r/m16
	 * SUB r/m16,imm16 
	 */

	/* 81 /5 id 
	 * Subtract imm32 from r/m32
	 * SUB r/m32,imm32 
	 */
	return 0;
}

int32_t instr_group_1_83_sub(struct emu_cpu *cpu, uint8_t a, uint8_t b, uint8_t *result)
{

	/* 83 /5 ib 
	 * Subtract sign-extended imm8 from r/m16
	 * SUB r/m16,imm8  
	 */

	/* 83 /5 ib 
	 * Subtract sign-extended imm8 from r/m32
	 * SUB r/m32,imm8
	 */
	return 0;
}

