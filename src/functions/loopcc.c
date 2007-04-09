/* @header@ */
#include <stdint.h>

#include "emu/emu_cpu.h"
#include "emu/emu_cpu_data.h"

#include "emu/emu_memory.h"


int32_t instr_loopcc_e0(struct emu_cpu *c, struct emu_cpu_instruction *i)
{				   
	SOURCE_COND_POS( i, c->eip + i->disp);

	/* E0 cb  
	 * Decrement count; jump short if count != 0 and ZF=0
	 * LOOPNE rel8
	 * LOOPNZ rel8 
	 */
	if ( i->prefixes & PREFIX_OPSIZE)
	{
		TRACK_NEED_REG16( i, cx);
		TRACK_NEED_EFLAG( i, f_zf);

		*c->reg16[cx] = *c->reg16[cx]-1;
		if (*c->reg16[cx] != 0 && !CPU_FLAG_ISSET(c,f_zf))
		{
			c->eip += i->disp;
		}
	}else
	{
		TRACK_NEED_REG32( i, ecx);
		TRACK_NEED_EFLAG( i, f_zf);

		c->reg[ecx]--;
		if (c->reg[ecx] != 0 && !CPU_FLAG_ISSET(c,f_zf))
		{
			c->eip += i->disp;
		}
	}


	return 0;
}

int32_t instr_loopcc_e1(struct emu_cpu *c, struct emu_cpu_instruction *i)
{				   
	SOURCE_COND_POS( i, c->eip + i->disp);

	/* E1 cb  
	 * Decrement count; jump short if count != 0 and ZF=1
	 * LOOPE rel8  
	 * LOOPZ rel8  
	 */				   
	if ( i->prefixes & PREFIX_OPSIZE)
	{
		TRACK_NEED_REG16( i, cx);
		TRACK_NEED_EFLAG( i, f_zf);

		*c->reg16[cx] = *c->reg16[cx]-1;
		if (*c->reg16[cx] != 0 && CPU_FLAG_ISSET(c,f_zf))
		{
			c->eip += i->disp;
		}
	}else
	{
		TRACK_NEED_REG32( i, ecx);
		TRACK_NEED_EFLAG( i, f_zf);

		c->reg[ecx]--;
		if (c->reg[ecx] != 0 && CPU_FLAG_ISSET(c,f_zf))
		{
			c->eip += i->disp;
		}
	}


	return 0;
}

int32_t instr_loop_e2(struct emu_cpu *c, struct emu_cpu_instruction *i)
{				   
	SOURCE_COND_POS( i, c->eip + i->disp);

	/* E2 cb  
	 * Decrement count; jump short if count != 0
	 * LOOP rel8   
	 */

	if ( i->prefixes & PREFIX_OPSIZE)
	{
		TRACK_NEED_REG16( i, cx);

		*c->reg16[cx] = *c->reg16[cx]-1;
		if (*c->reg16[cx] != 0 )
		{
			c->eip += i->disp;
		}
	}else
	{
		TRACK_NEED_REG32( i, ecx);

		c->reg[ecx]--;
		if (c->reg[ecx] != 0)
		{
			c->eip += i->disp;
		}
	}


	return 0;
}
				   


