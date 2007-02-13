/* @header@ */
#include <stdint.h>

#define INSTR_CALC(inttype,a,b,operation)			\
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

#define INSTR_CALC_AND_SET_FLAGS(inttype,cpu,a,b,operation)	\
INSTR_CALC(inttype,a,b,operation)								\
INSTR_SET_FLAG_ZF(cpu)											\
INSTR_SET_FLAG_PF(cpu)											\
INSTR_SET_FLAG_SF(cpu)											\
INSTR_SET_FLAG_CF(cpu,operation)								\
INSTR_SET_FLAG_OF(cpu,operation)								


int32_t instr_cmp_38(struct emu_cpu *c, struct instruction *i)
{
	if ( i->modrm.mod != 3 )
	{
		uint8_t dst;
		MEM_BYTE_READ(c, i->modrm.ea, &dst);
//		dst += *c->reg8[i->modrm.opc];
		INSTR_CALC_AND_SET_FLAGS(uint8_t, 
								 c, 
								 dst, 
								 *c->reg8[i->modrm.opc], 
								 -)
		MEM_BYTE_WRITE(c, i->modrm.ea, dst);
	} else
	{
/*		*c->reg8[i->modrm.rm] += *c->reg8[i->modrm.opc]; */
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
			uint16_t dst;
			MEM_WORD_READ(c, i->modrm.ea, &dst);
//			dst += *c->reg16[i->modrm.opc];
			INSTR_CALC_AND_SET_FLAGS(uint16_t, 
									 c, 
									 dst, 
									 *c->reg16[i->modrm.opc], 
									 -)
			MEM_WORD_WRITE(c, i->modrm.ea, dst);
		} else
		{
			uint32_t dst;
			MEM_DWORD_READ(c, i->modrm.ea, &dst);
//			dst += c->reg[i->modrm.opc];
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
//			*c->reg16[i->modrm.rm] += *c->reg16[i->modrm.opc];
			INSTR_CALC_AND_SET_FLAGS(uint16_t, 
									 c, 
									 *c->reg16[i->modrm.rm], 
									 *c->reg16[i->modrm.opc], 
									 -)
		} else
		{
//			c->reg[i->modrm.rm] += c->reg[i->modrm.opc];
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
	if ( i->modrm.mod != 3 )
	{
		uint8_t op;
		MEM_BYTE_READ(c, i->modrm.ea, &op);

//		*c->reg8[i->modrm.opc] += op;
		INSTR_CALC_AND_SET_FLAGS(uint8_t, 
								 c, 
								 op, 
								 *c->reg8[i->modrm.opc], 
								 -)
	} else
	{
//		*c->reg8[i->modrm.opc] += *c->reg8[i->modrm.rm];
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
			uint16_t op;
			MEM_WORD_READ(c, i->modrm.ea, &op);

//			*c->reg16[i->modrm.opc] += op;
			INSTR_CALC_AND_SET_FLAGS(uint16_t, 
									 c, 
									 op,
									 *c->reg16[i->modrm.opc], 
									 -)
		} else
		{
			uint32_t op;
			MEM_DWORD_READ(c, i->modrm.ea, &op);
//			c->reg[i->modrm.opc] += op;
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
//			*c->reg16[i->modrm.opc] += *c->reg16[i->modrm.rm];
			INSTR_CALC_AND_SET_FLAGS(uint16_t, 
									 c, 
									 *c->reg16[i->modrm.rm], 
									 *c->reg16[i->modrm.opc], 
									 -)
		} else
		{
			c->reg[i->modrm.opc] += c->reg[i->modrm.rm];
		}
	}

	return 0;
}

int32_t instr_cmp_3c(struct emu_cpu *c, struct instruction *i)
{
//	*c->reg8[eax] += *i->imm8;
	INSTR_CALC_AND_SET_FLAGS(uint8_t, 
							 c, 
							 *c->reg8[eax], 
							 *i->imm8, 
							 -)
	return 0;
}

int32_t instr_cmp_3d(struct emu_cpu *c, struct instruction *i)
{
	if ( i->prefixes & PREFIX_OPSIZE )
	{
//		*c->reg16[eax] += *i->imm16;
		INSTR_CALC_AND_SET_FLAGS(uint16_t, 
								 c, 
								 *c->reg16[eax], 
								 *i->imm16, 
								 -)
	} else
	{
//		c->reg[eax] += i->imm;
		INSTR_CALC_AND_SET_FLAGS(uint32_t, 
								 c, 
								 c->reg[eax], 
								 i->imm, 
								 -)
	}

	return 0;
}
