/* @header@ */
#include <stdint.h>

#include "emu/emu_cpu.h"
#include "emu/emu_cpu_data.h"

#include "emu/emu_cpu_stack.h"
#include "emu/emu_memory.h"

/*Intel Architecture Software Developer's Manual Volume 2: Instruction Set Reference (24319102.PDF) page 571*/

int32_t instr_pop_07(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	/* 07     
	 * Pop top of stack into ES; increment stack pointer   
	 * POP ES  
	 */
	UNIMPLEMENTED(c, NNY);

	return 0;
}

int32_t instr_pop_17(struct emu_cpu *c, struct emu_cpu_instruction *i)
{

	/* 17     
	 * Pop top of stack into SS; increment stack pointer   
	 * POP SS  
	 */
	UNIMPLEMENTED(c, NNY);

	return 0;
}

int32_t instr_pop_1f(struct emu_cpu *c, struct emu_cpu_instruction *i)
{

	/* 1F     
	 * Pop top of stack into DS; increment stack pointer   
	 * POP DS  
	 */
	UNIMPLEMENTED(c, NNY);
	
	return 0;
}

int32_t instr_pop_5x(struct emu_cpu *c, struct emu_cpu_instruction *i)
{

	if (i->prefixes & PREFIX_OPSIZE)
	{
		/* 58+ rw 
		 * Pop top of stack into r16; increment stack pointer  
		 * POP r16 
		 */
		POP_WORD(c, c->reg16[i->opc & 7 ]);
		TRACK_INIT_REG16(i, i->opc & 7);
	}else
	{
		/* 58+ rd 
		 * Pop top of stack into r32; increment stack pointer  
		 * POP r32 
		 */
		POP_DWORD(c, &c->reg[i->opc & 7]);
		TRACK_INIT_REG32(i, i->opc & 7);
	}
	return 0;
}

int32_t instr_pop_0fa1(struct emu_cpu *c, struct emu_cpu_instruction *i)
{

	/* 0F A1  
	 * Pop top of stack into FS; increment stack pointer   
	 * POP FS  
	 */
	UNIMPLEMENTED(c, NNY);

	return 0;
}

int32_t instr_pop_0fa9(struct emu_cpu *c, struct emu_cpu_instruction *i)
{

	/* 0F A9  
	 * Pop top of stack into GS; increment stack pointer   
	 * POP GS  
	 */
	UNIMPLEMENTED(c, NNY);

	return 0;
}

int32_t instr_popad_61(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	uint32_t j;
	
	if( i->prefixes & PREFIX_OPSIZE )
	{
		for( j = 7; j < 8; j-- )
		{
			if( j != 4 )
			{
				POP_WORD(c, c->reg16[j])
			}
			else
			{
				c->reg[esp] += 2;
			}
		}
	}
	else
	{
		for( j = 7; j < 8; j-- )
		{
			if( j != 4 )
			{
				POP_DWORD(c, &c->reg[j]);
			}
			else
			{
				c->reg[esp] += 4;
			}
		}
	}
	
	return 0;
}

