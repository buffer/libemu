/* @header@ */
#include <stdint.h>

#define INSTR_CALC(inttype, a)						\
uint##inttype##_t operand_a = a;								\
uint##inttype##_t operation_result = operand_a+1;				\
a = operation_result;



#define INSTR_SET_FLAG_OF(cpu, inttype)									\
{																				\
	int64_t sz = (int##inttype##_t)operand_a;                                            \
																				\
	sz++;																	\
																				\
	if (sz < max_inttype_borders[sizeof(operation_result)][0][0] || sz > max_inttype_borders[sizeof(operation_result)][0][1] \
	|| sz != (int64_t)operation_result )									    \
	{                                                                           \
		CPU_FLAG_SET(cpu, f_of);                                                \
	}else                                                                       \
	{                                                                           \
		CPU_FLAG_UNSET(cpu, f_of);                                              \
	}                                                                           \
}


#include "emu/emu_cpu.h"
#include "emu/emu_cpu_data.h"
#include "emu/emu_cpu_functions.h"
#include "emu/emu_memory.h"



#ifdef INSTR_CALC_AND_SET_FLAGS
#undef INSTR_CALC_AND_SET_FLAGS
#endif // INSTR_CALC_AND_SET_FLAGS

#define INSTR_CALC_AND_SET_FLAGS(inttype, cpu, a)	\
INSTR_CALC(inttype, a)								\
INSTR_SET_FLAG_ZF(cpu)								\
INSTR_SET_FLAG_PF(cpu)								\
INSTR_SET_FLAG_SF(cpu)								\
INSTR_SET_FLAG_OF(cpu,inttype)								




int32_t instr_inc_4x(struct emu_cpu *c, struct instruction *i)
{
	if ( i->prefixes & PREFIX_OPSIZE )
	{
		/* 40+ rw 
		 * Increment word register by 1
		 * INC r16 
		 */
		INSTR_CALC_AND_SET_FLAGS(16, c, *c->reg16[i->opc & 7])
	}else
	{
		/* 40+ rd 
		 * Increment doubleword register by 1
		 * INC r32 
		 */
		INSTR_CALC_AND_SET_FLAGS(32, c, c->reg[i->opc & 7])
	}
	return 0;
}



