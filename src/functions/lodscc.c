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

#include "emu/emu_cpu_stack.h"
#include "emu/emu_memory.h"


int32_t instr_lods_ac(struct emu_cpu *c, struct emu_cpu_instruction *i)
{


	/* AC 
	 * Load byte at address DS:(E)SI into AL
	 * LODS m8  
	 */

	/* AC
	 * Load byte at address DS:(E)SI into AL
	 * LODSB    
	 */

	if ( i->prefixes & PREFIX_ADSIZE )
	{
//    	MEM_BYTE_READ(c, c->reg16[si], c->reg8[al]);
		UNIMPLEMENTED(c, SST);
	}
	else
	{
		MEM_BYTE_READ(c, c->reg[esi], c->reg8[al]);

		if ( CPU_FLAG_ISSET(c,f_df) )
		{ /* decrement */
			c->reg[esi] -= 1;
		}
		else
		{ /* increment */
			c->reg[esi] += 1;
		}

		TRACK_INIT_REG8(c->instr, al);
	}


	return 0;
}

int32_t instr_lods_ad(struct emu_cpu *c, struct emu_cpu_instruction *i)
{

	if ( i->prefixes & PREFIX_OPSIZE )
	{

		/* AD 
		 * Load word at address DS:(E)SI into AX
		 * LODS m16 
		 */

		/* AD 
		 * Load word at address DS:(E)SI into AX
		 * LODSW    
		 */
		if ( i->prefixes & PREFIX_ADSIZE )
		{
//        	MEM_WORD_READ(c, c->reg16[si], c->reg16[ax]);
			UNIMPLEMENTED(c, SST);
		}
		else
		{
			MEM_WORD_READ(c, c->reg[esi], c->reg16[ax]);
			if (CPU_FLAG_ISSET(c,f_df))
			{ /* decrement */
				c->reg[esi] -= 2;
			}else
			{ /* increment */
				c->reg[esi] += 2;
			}
		}

	}
	else
	{

		/* AD 
		 * Load doubleword at address DS:(E)SI into EAX
		 * LODS m32 
		 */


		/* AD
		 * Load doubleword at address DS:(E)SI into EAX
		 * LODSD    
		 */
		if ( i->prefixes & PREFIX_ADSIZE )
		{
//			MEM_DWORD_READ(c, c->reg16[si], &c->reg[eax]);
			UNIMPLEMENTED(c, SST);

		}
		else
		{
			MEM_DWORD_READ(c, c->reg[esi], &c->reg[eax]);
			if (CPU_FLAG_ISSET(c,f_df))
			{ /* decrement */
				c->reg[esi] -= 4;
			}else
			{ /* increment */
				c->reg[esi] += 4;
			}

			TRACK_INIT_REG32(c->instr, eax);
		}
	}

	return 0;
}

