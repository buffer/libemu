/* @header@ */
#include <stdint.h>

#include "emu/emu_cpu.h"
#include "emu/emu_cpu_data.h"
#include "emu/emu_cpu_functions.h"
#include "emu/emu_memory.h"

int32_t instr_group_1_80(struct emu_cpu *c, struct instruction *i)
{
	if( i->modrm.mod != 3 )
	{
		uint8_t dst;
		MEM_BYTE_READ(c, i->modrm.ea, &dst);
		
		/* dst <-- dst <OPC> imm8 */
		switch ( i->modrm.opc )
		{
		case 0:	/* add */
			instr_group_1_80_add(c, dst, *i->imm8, &dst);
			break;
		case 1:	/* or */
			instr_group_1_80_or(c, dst, *i->imm8, &dst);
			break;
		case 2:	/* adc */
			instr_group_1_80_adc(c, dst, *i->imm8, &dst);
			break;
		case 3:	/* sbb */
			instr_group_1_80_sbb(c, dst, *i->imm8, &dst);
			break;
		case 4:	/* and */
			instr_group_1_80_and(c, dst, *i->imm8, &dst);
			break;
		case 5:	/* sub */
			instr_group_1_80_sub(c, dst, *i->imm8, &dst);
			break;
		case 6:	/* xor */
			instr_group_1_80_xor(c, dst, *i->imm8, &dst);
			break;
		case 7:	/* cmp */
			instr_group_1_80_cmp(c, dst, *i->imm8);
			break;
		}

		MEM_BYTE_WRITE(c, i->modrm.ea, dst);
	}
	else
	{
		/* reg8[rm] <-- reg8[rm] <OPC> imm8 */
		switch ( i->modrm.opc )
		{
		case 0:	/* add */
			instr_group_1_80_add(c, *c->reg8[i->modrm.rm], *i->imm8, c->reg8[i->modrm.rm]);
			break;
		case 1:	/* or */
			instr_group_1_80_or(c, *c->reg8[i->modrm.rm], *i->imm8, c->reg8[i->modrm.rm]);
			break;
		case 2:	/* adc */
			instr_group_1_80_adc(c, *c->reg8[i->modrm.rm], *i->imm8, c->reg8[i->modrm.rm]);
			break;
		case 3:	/* sbb */
			instr_group_1_80_sbb(c, *c->reg8[i->modrm.rm], *i->imm8, c->reg8[i->modrm.rm]);
			break;
		case 4:	/* and */
			instr_group_1_80_and(c, *c->reg8[i->modrm.rm], *i->imm8, c->reg8[i->modrm.rm]);
			break;
		case 5:	/* sub */
			instr_group_1_80_sub(c, *c->reg8[i->modrm.rm], *i->imm8, c->reg8[i->modrm.rm]);
			break;
		case 6:	/* xor */
			instr_group_1_80_xor(c, *c->reg8[i->modrm.rm], *i->imm8, c->reg8[i->modrm.rm]);
			break;
		case 7:	/* cmp */
			instr_group_1_80_cmp(c, *c->reg8[i->modrm.rm], *i->imm8);
			break;
		}
	}
	
	return 0;
}

int32_t instr_group_1_81(struct emu_cpu *c, struct instruction *i)
{

	static int32_t (*group_1_81_fn[8])(struct emu_cpu *c, struct instruction *i) = {
		/* 0 */ instr_group_1_81_add,
		/* 1 */ instr_group_1_81_or,
		/* 2 */ instr_group_1_81_adc,
		/* 3 */ instr_group_1_81_sbb,
		/* 4 */ instr_group_1_81_and,
		/* 5 */ instr_group_1_81_sub,
		/* 6 */ instr_group_1_81_xor,
		/* 7 */ instr_group_1_81_cmp,
	};

	return group_1_81_fn[i->modrm.opc](c, i);
}

int32_t instr_group_1_82(struct emu_cpu *c, struct instruction *i)
{
	return 0;
}

int32_t instr_group_1_83(struct emu_cpu *c, struct instruction *i)
{
	static int32_t (*group_1_83_fn[8])(struct emu_cpu *c, struct instruction *i) = {
		/* 0 */ instr_group_1_83_add,
		/* 1 */ instr_group_1_83_or,
		/* 2 */ instr_group_1_83_adc,
		/* 3 */ instr_group_1_83_sbb,
		/* 4 */ instr_group_1_83_and,
		/* 5 */ instr_group_1_83_sub,
		/* 6 */ instr_group_1_83_xor,
		/* 7 */ instr_group_1_83_cmp,
	};

	return group_1_83_fn[i->modrm.opc](c, i);
}


