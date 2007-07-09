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


int32_t instr_jmp_e9(struct emu_cpu *c, struct emu_cpu_instruction *i)
{

	

	/* E9 cw  
	 * Jump near, relative, displacement relative to next instruction         
	 * JMP rel16    
	 */


	/* E9 cd  
	 * Jump near, relative, displacement relative to next instruction         
	 * JMP rel32    
	 */

	c->eip += i->disp;

	SOURCE_NORM_POS(c->instr, c->eip);

	return 0;
}


int32_t instr_jmp_ea(struct emu_cpu *c, struct emu_cpu_instruction *i)
{

	/* EA cd  
	 * Jump far, absolute, address given in operand                           
	 * JMP ptr16:16 
	 */


	/* EA cp  
	 * Jump far, absolute, address given in operand                           
	 * JMP ptr16:32 
	 */

	UNIMPLEMENTED(c, SST);

}

int32_t instr_jmp_eb(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	/* EB cb  
	 * Jump short, relative, displacement relative to next instruction        
	 * JMP rel8     
	 */

    c->eip += i->disp;

	SOURCE_NORM_POS(c->instr, c->eip);

	return 0;
}


int32_t instr_group_5_ff_jmp(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	if( i->modrm.opc == 4 )
	{
		if( i->modrm.mod != 3 )
		{
			if( i->prefixes & PREFIX_OPSIZE )
			{
				/* FF /4  
				 * Jump near, absolute indirect, address given in r/m16                   
				 * JMP r/m16    
				 */

				uint16_t disp;
				MEM_WORD_READ(c, i->modrm.ea, &disp);
				
				c->eip = disp;

				SOURCE_NORM_POS(c->instr, c->eip);
			}
			else
			{
				/* FF /4  
				 * Jump near, absolute indirect, address given in r/m32                   
				 * JMP r/m32    
				 */

				uint32_t disp;
				MEM_DWORD_READ(c, i->modrm.ea, &disp);
				
				c->eip = disp;

				SOURCE_NORM_POS(c->instr, c->eip);
			}
		}
		else
		{
			if( i->prefixes & PREFIX_OPSIZE )
			{
				/* FF /4  
				 * Jump near, absolute indirect, address given in r/m16                   
				 * JMP r/m16    
				 */

				c->eip = *c->reg16[i->modrm.rm];

				SOURCE_NORM_POS(c->instr, c->eip);
				TRACK_NEED_REG16(c->instr, i->modrm.rm);
			}
			else
			{
				/* FF /4  
				 * Jump near, absolute indirect, address given in r/m32                   
				 * JMP r/m32    
				 */

				c->eip = c->reg[i->modrm.rm];

				SOURCE_NORM_POS(c->instr, c->eip);
				TRACK_NEED_REG32(c->instr, i->modrm.rm);
			}
		}
	}
	else /* /5 */
	{
		/* unneeded */
		/* FF /5  
		 * Jump far, absolute indirect, address given in m16:16                   
		 * JMP m16:16   
		 */


		/* FF /5  
		 * Jump far, absolute indirect, address given in m16:32                   
		 * JMP m16:32   
		 */

		UNIMPLEMENTED(c, SST);

	}

	return 0;
}

