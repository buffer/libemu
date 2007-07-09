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
#include <errno.h>

#include "emu/emu.h"
#include "emu/emu_cpu.h"
#include "emu/emu_cpu_data.h"

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
