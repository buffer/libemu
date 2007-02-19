/* @header@ */
#include <stdint.h>

#include "emu/emu_cpu.h"
#include "emu/emu_cpu_data.h"
#include "emu/emu_cpu_functions.h"
#include "emu/emu_memory.h"


int32_t instr_loopcc_e0(struct emu_cpu *c, struct instruction *i)
{				   

	/* E0 cb  
	 * Decrement count; jump short if count != 0 and ZF=0
	 * LOOPNE rel8
	 * LOOPNZ rel8 
	 */
	if ( i->prefixes & PREFIX_OPSIZE)
	{
		*c->reg16[cx] = *c->reg16[cx]-1;
		if (*c->reg16[cx] != 0 && !CPU_FLAG_ISSET(c,f_zf))
		{
			c->eip += *i->imm8;
		}
	}else
	{
		c->reg[ecx]--;
		if (c->reg[ecx] != 0 && !CPU_FLAG_ISSET(c,f_zf))
		{
			c->eip += *i->imm8;
		}
	}


	return 0;
}

int32_t instr_loopcc_e1(struct emu_cpu *c, struct instruction *i)
{				   


	/* E1 cb  
	 * Decrement count; jump short if count != 0 and ZF=1
	 * LOOPE rel8  
	 * LOOPZ rel8  
	 */				   
	if ( i->prefixes & PREFIX_OPSIZE)
	{
		*c->reg16[cx] = *c->reg16[cx]-1;
		if (*c->reg16[cx] != 0 && CPU_FLAG_ISSET(c,f_zf))
		{
			c->eip += *i->imm8;
		}
	}else
	{
		c->reg[ecx]--;
		if (c->reg[ecx] != 0 && CPU_FLAG_ISSET(c,f_zf))
		{
			c->eip += *i->imm8;
		}
	}


	return 0;
}

int32_t instr_loop_e2(struct emu_cpu *c, struct instruction *i)
{				   
	/* E2 cb  
	 * Decrement count; jump short if count != 0
	 * LOOP rel8   
	 */

	if ( i->prefixes & PREFIX_OPSIZE)
	{
		*c->reg16[cx] = *c->reg16[cx]-1;
		if (*c->reg16[cx] != 0 )
		{
			c->eip += *i->imm8;
		}
	}else
	{
		c->reg[ecx]--;
		if (c->reg[ecx] != 0)
		{
			c->eip += *i->imm8;
		}
	}


	return 0;
}
				   


