/* @header@ */
#include <stdint.h>
#include <errno.h>

#include "emu/emu.h"
#include "emu/emu_cpu.h"
#include "emu/emu_cpu_data.h"
#include "emu/emu_cpu_functions.h"
#include "emu/emu_memory.h"

int32_t instr_aaa_37(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	/* 37 
	 * ASCII adjust AL after add
	 * AAA
	 */

	if ( ((*c->reg8[al] & 0x0f) > 9) || CPU_FLAG_ISSET(c,f_af))
	{
		*c->reg8[al] = *c->reg8[al] + 6;
		*c->reg8[ah] = *c->reg8[ah] + 1;
		CPU_FLAG_SET(c,f_af);
		CPU_FLAG_SET(c,f_cf);
	}else
	{
		CPU_FLAG_UNSET(c,f_af);
		CPU_FLAG_UNSET(c,f_cf);
	}
	*c->reg8[al] = (*c->reg8[al] & 0x0f);

	return 0;
}
