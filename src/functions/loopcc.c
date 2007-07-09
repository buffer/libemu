/********************************************************************************
 *                               libemu
 *
 *                    - x86 shellcode emulation -
 *
 *
 * Copyright (C) 2007  Paul Baecher & Markus Koetter
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 * 
 * 
 *             contact nepenthesdev@users.sourceforge.net  
 *
 *******************************************************************************/

#include <stdint.h>

#include "emu/emu_cpu.h"
#include "emu/emu_cpu_data.h"

#include "emu/emu_memory.h"


int32_t instr_loopcc_e0(struct emu_cpu *c, struct emu_cpu_instruction *i)
{				   
	SOURCE_COND_POS(c->instr, c->eip + i->disp);

	/* E0 cb  
	 * Decrement count; jump short if count != 0 and ZF=0
	 * LOOPNE rel8
	 * LOOPNZ rel8 
	 */
	if ( i->prefixes & PREFIX_OPSIZE)
	{
		TRACK_NEED_REG16(c->instr, cx);
		TRACK_NEED_EFLAG(c->instr, f_zf);

		*c->reg16[cx] = *c->reg16[cx]-1;
		if (*c->reg16[cx] != 0 && !CPU_FLAG_ISSET(c,f_zf))
		{
			c->eip += i->disp;
		}
	}else
	{
		TRACK_NEED_REG32(c->instr, ecx);
		TRACK_NEED_EFLAG(c->instr, f_zf);

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
	SOURCE_COND_POS(c->instr, c->eip + i->disp);

	/* E1 cb  
	 * Decrement count; jump short if count != 0 and ZF=1
	 * LOOPE rel8  
	 * LOOPZ rel8  
	 */				   
	if ( i->prefixes & PREFIX_OPSIZE)
	{
		TRACK_NEED_REG16(c->instr, cx);
		TRACK_NEED_EFLAG(c->instr, f_zf);

		*c->reg16[cx] = *c->reg16[cx]-1;
		if (*c->reg16[cx] != 0 && CPU_FLAG_ISSET(c,f_zf))
		{
			c->eip += i->disp;
		}
	}else
	{
		TRACK_NEED_REG32(c->instr, ecx);
		TRACK_NEED_EFLAG(c->instr, f_zf);

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
	SOURCE_COND_POS(c->instr, c->eip + i->disp);

	/* E2 cb  
	 * Decrement count; jump short if count != 0
	 * LOOP rel8   
	 */

	if ( i->prefixes & PREFIX_OPSIZE)
	{
		TRACK_NEED_REG16(c->instr, cx);

		*c->reg16[cx] = *c->reg16[cx]-1;
		if (*c->reg16[cx] != 0 )
		{
			c->eip += i->disp;
		}
	}else
	{
		TRACK_NEED_REG32(c->instr, ecx);

		c->reg[ecx]--;
		if (c->reg[ecx] != 0)
		{
			c->eip += i->disp;
		}
	}


	return 0;
}
				   


