/* @header@ */
#include <stdint.h>
#include <stdio.h>

#define INSTR_CALC(inttype, a, b, c, operation, cpu)			\
uint##inttype##_t operand_a = a;								\
uint##inttype##_t operand_b = b;								\
uint##inttype##_t operation_result = operand_a operation operand_b operation ((cpu->eflags & (1 << f_cf))?1:0);	\
c = operation_result;

#define INSTR_SET_FLAG_OF(cpu, operand,inttype)											\
{																				\
	int64_t sx = (int##inttype##_t)operand_a;                                            \
	int64_t sy = (int##inttype##_t)operand_b;                                            \
	int64_t sz = 0;                                                             \
																				\
	sz = sx operand sy operand ((cpu->eflags & (1 << f_cf))?1:0);						\
	/*printf("of: sx %lli + sy %lli + cf %i = sz %lli \n", sx, sy, (cpu->eflags & (1 << f_cf))?1:0, sz);*/\
																			\
	if (sz < max_inttype_borders[sizeof(operation_result)][0][0] || sz > max_inttype_borders[sizeof(operation_result)][0][1] \
	|| sz != (int64_t)operation_result )									    \
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
#include "emu/emu_cpu_functions.h"
#include "emu/emu_memory.h"


#ifdef INSTR_CALC_AND_SET_FLAGS
#undef INSTR_CALC_AND_SET_FLAGS
#endif // INSTR_CALC_AND_SET_FLAGS

#define INSTR_CALC_AND_SET_FLAGS(inttype, cpu, a, b, c, operation)	\
INSTR_CALC(inttype, a, b, c, operation, cpu)									\
INSTR_SET_FLAG_ZF(cpu)											\
INSTR_SET_FLAG_PF(cpu)											\
INSTR_SET_FLAG_SF(cpu)											\
INSTR_SET_FLAG_OF(cpu, operation, inttype)						\
INSTR_SET_FLAG_CF(cpu, operation)




int32_t instr_adc_10(struct emu_cpu *c, struct instruction *i)
{
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
	} else
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

int32_t instr_adc_11(struct emu_cpu *c, struct instruction *i)
{


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
		} else
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
	} else
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
		} else
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

int32_t instr_adc_12(struct emu_cpu *c, struct instruction *i)
{
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
	} else
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

int32_t instr_adc_13(struct emu_cpu *c, struct instruction *i)
{


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
		} else
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
	} else
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
		} else
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

int32_t instr_adc_14(struct emu_cpu *c, struct instruction *i)
{
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

int32_t instr_adc_15(struct emu_cpu *c, struct instruction *i)
{

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
	} else
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


int32_t instr_group_1_80_adc(struct emu_cpu *cpu, uint8_t a, uint8_t b, uint8_t *result)
{
	INSTR_CALC_AND_SET_FLAGS(8, 
							 cpu, 
							 a, 
							 b, 
							 *result, 
							 +)
	return 0;
}
