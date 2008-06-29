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


#include <inttypes.h>

#include "emu/emu_cpu.h"
#include "emu/emu_cpu_data.h"
#include "emu/emu_cpu_stack.h"
#include "emu/emu_memory.h"

int32_t instr_ret_c2(struct emu_cpu *c, struct emu_cpu_instruction *i)
{ 
	/* C2 
	 * Near return to calling procedure and pop imm16 bytes from stack
	 * iw RET imm16  
	 */
	POP_DWORD(c, &c->eip);

#if BYTE_ORDER == BIG_ENDIAN
	uint16_t val;
	bcopy(i->imm16, &val, 2);
	c->reg[esp] += val;
#else
	c->reg[esp] += *i->imm16;
#endif	
	return 0;
}

int32_t instr_ret_c3(struct emu_cpu *c, struct emu_cpu_instruction *i)
{ 
	/* C3
	 * Near return to calling procedure
	 * RET        
	 */
	POP_DWORD(c, &c->eip);
	
	return 0;
}

int32_t instr_ret_ca(struct emu_cpu *c, struct emu_cpu_instruction *i)
{ 	
	/* CA iw 
	 * Far return to calling procedure and pop imm16 bytes from stack
	 * RET imm16  
	 */
	UNIMPLEMENTED(c, NNY);
	return -1;
}

int32_t instr_ret_cb(struct emu_cpu *c, struct emu_cpu_instruction *i)
{ 
	/* CB
	 * Far return to calling procedure
	 * RET
	 */
	UNIMPLEMENTED(c, NNY);
	return -1;
}

