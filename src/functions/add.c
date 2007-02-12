/* @header@ */
#include <stdint.h>

#include "emu/emu_cpu.h"
#include "emu/emu_cpu_data.h"
#include "emu/emu_cpu_functions.h"
#include "emu/emu_memory.h"

int32_t instr_add_00(struct emu_cpu *c, struct instruction *i)
{
	if( i->modrm.mod != 3 )
	{
		uint8_t dst;
		MEM_BYTE_READ(c, i->modrm.ea, &dst);
//		dst += *c->reg8[i->modrm.opc];
		INSTR_CALC_AND_SET_FLAGS(uint8_t, 
										 c, 
										 dst, 
										 *c->reg8[i->modrm.opc], 
										 dst, 
										 +, 
										 EMU_INSTR_ADD)
		MEM_BYTE_WRITE(c, i->modrm.ea, dst);
	}
	else
	{
/*		*c->reg8[i->modrm.rm] += *c->reg8[i->modrm.opc]; */
		INSTR_CALC_AND_SET_FLAGS(uint8_t, 
										 c, 
										 *c->reg8[i->modrm.rm], 
										 *c->reg8[i->modrm.opc], 
										 *c->reg8[i->modrm.rm], 
										 +, 
										 EMU_INSTR_ADD)
	}
	
	return 0;
}

int32_t instr_add_01(struct emu_cpu *c, struct instruction *i)
{
	if( i->modrm.mod != 3 )
	{
		if( i->prefixes & PREFIX_OPSIZE )
		{
			uint16_t dst;
			MEM_WORD_READ(c, i->modrm.ea, &dst);
//			dst += *c->reg16[i->modrm.opc];
			INSTR_CALC_AND_SET_FLAGS(uint16_t, 
											 c, 
											 dst, 
											 *c->reg16[i->modrm.opc], 
											 dst, 
											 +, 
											 EMU_INSTR_ADD)
			MEM_WORD_WRITE(c, i->modrm.ea, dst);
		}
		else
		{
			uint32_t dst;
			MEM_DWORD_READ(c, i->modrm.ea, &dst);
//			dst += c->reg[i->modrm.opc];
			INSTR_CALC_AND_SET_FLAGS(uint32_t, 
											 c, 
											 dst, 
											 c->reg[i->modrm.opc], 
											 dst, 
											 +, 
											 EMU_INSTR_ADD)
			MEM_DWORD_WRITE(c, i->modrm.ea, dst);
		}
	}
	else
	{
		if( i->prefixes & PREFIX_OPSIZE )
		{
//			*c->reg16[i->modrm.rm] += *c->reg16[i->modrm.opc];
			INSTR_CALC_AND_SET_FLAGS(uint16_t, 
											 c, 
											 *c->reg16[i->modrm.rm], 
											 *c->reg16[i->modrm.opc], 
											 *c->reg16[i->modrm.rm], 
											 +, 
											 EMU_INSTR_ADD)
		}
		else
		{
//			c->reg[i->modrm.rm] += c->reg[i->modrm.opc];
			INSTR_CALC_AND_SET_FLAGS(uint32_t, 
											 c, 
											 c->reg[i->modrm.rm], 
											 c->reg[i->modrm.opc], 
											 c->reg[i->modrm.rm], 
											 +, 
											 EMU_INSTR_ADD)
		}
	}
	
	return 0;
}

int32_t instr_add_02(struct emu_cpu *c, struct instruction *i)
{
	if( i->modrm.mod != 3 )
	{
		uint8_t op;
		MEM_BYTE_READ(c, i->modrm.ea, &op);
		
//		*c->reg8[i->modrm.opc] += op;
		INSTR_CALC_AND_SET_FLAGS(uint8_t, 
										 c, 
										 op, 
										 *c->reg8[i->modrm.opc], 
										 *c->reg8[i->modrm.opc], 
										 +, 
										 EMU_INSTR_ADD)
	}
	else
	{
//		*c->reg8[i->modrm.opc] += *c->reg8[i->modrm.rm];
		INSTR_CALC_AND_SET_FLAGS(uint8_t, 
										 c, 
										 *c->reg8[i->modrm.opc], 
										 *c->reg8[i->modrm.rm], 
										 *c->reg8[i->modrm.opc], 
										 +, 
										 EMU_INSTR_ADD)
	}
	
	return 0;
}

int32_t instr_add_03(struct emu_cpu *c, struct instruction *i)
{
	if( i->modrm.mod != 3 )
	{
		if( i->prefixes & PREFIX_OPSIZE )
		{
			uint16_t op;
			MEM_WORD_READ(c, i->modrm.ea, &op);
			
//			*c->reg16[i->modrm.opc] += op;
			INSTR_CALC_AND_SET_FLAGS(uint16_t, 
											 c, 
											 op,
											 *c->reg16[i->modrm.opc], 
                                             *c->reg16[i->modrm.opc], 
											 +, 
											 EMU_INSTR_ADD)

		}
		else
		{
			uint32_t op;
			MEM_DWORD_READ(c, i->modrm.ea, &op);
//			c->reg[i->modrm.opc] += op;
			INSTR_CALC_AND_SET_FLAGS(uint32_t, 
											 c, 
											 op,
											 c->reg[i->modrm.opc], 
											 c->reg[i->modrm.opc], 
											 +, 
											 EMU_INSTR_ADD)
			

		}
	}
	else
	{
		if( i->prefixes & PREFIX_OPSIZE )
		{
//			*c->reg16[i->modrm.opc] += *c->reg16[i->modrm.rm];
			INSTR_CALC_AND_SET_FLAGS(uint16_t, 
											 c, 
											 *c->reg16[i->modrm.rm], 
											 *c->reg16[i->modrm.opc], 
											 *c->reg16[i->modrm.opc], 
											 +, 
											 EMU_INSTR_ADD)


		}
		else
		{
			c->reg[i->modrm.opc] += c->reg[i->modrm.rm];
		}
	}
	
	return 0;
}

int32_t instr_add_04(struct emu_cpu *c, struct instruction *i)
{
//	*c->reg8[eax] += *i->imm8;
	INSTR_CALC_AND_SET_FLAGS(uint8_t, 
									 c, 
									 *c->reg8[eax], 
									 *i->imm8, 
									 *c->reg8[eax], 
									 +, 
									 EMU_INSTR_ADD)

	return 0;
}

int32_t instr_add_05(struct emu_cpu *c, struct instruction *i)
{
	if( i->prefixes & PREFIX_OPSIZE )
	{
//		*c->reg16[eax] += *i->imm16;
		INSTR_CALC_AND_SET_FLAGS(uint16_t, 
										 c, 
										 *c->reg16[eax], 
										 *i->imm16, 
										 *c->reg16[eax], 
										 +, 
										 EMU_INSTR_ADD)
	}
	else
	{
//		c->reg[eax] += i->imm;
		INSTR_CALC_AND_SET_FLAGS(uint32_t, 
										 c, 
										 c->reg[eax], 
										 i->imm, 
										 c->reg[eax], 
										 +, 
										 EMU_INSTR_ADD)
	}
	
	return 0;
}
