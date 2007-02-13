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
			instr_group_1_80_add8(c,dst,i->imm8,&dst);
			break;
		case 1:	/* or */
			break;
		case 2:	/* adc */
			break;
		case 3:	/* sbb */
			break;
		case 4:	/* and */
			break;
		case 5:	/* sub */
			break;
		case 6:	/* xor */
			break;
		case 7:	/* cmp */
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
			instr_group_1_80_add8(c,*c->reg8[i->modrm],i->imm8,*c->reg8[i->modrm]);
			break;
		case 1:	/* or */
			break;
		case 2:	/* adc */
			break;
		case 3:	/* sbb */
			break;
		case 4:	/* and */
			break;
		case 5:	/* sub */
			break;
		case 6:	/* xor */
			break;
		case 7:	/* cmp */
			break;
		}
	}
	
	return 0;
}

int32_t instr_group_1_81(struct emu_cpu *c, struct instruction *i)
{
	return 0;
}

int32_t instr_group_1_82(struct emu_cpu *c, struct instruction *i)
{
	return 0;
}

int32_t instr_group_1_83(struct emu_cpu *c, struct instruction *i)
{
	return 0;
}

