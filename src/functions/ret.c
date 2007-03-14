/* @header@ */

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
	
	c->reg[esp] += *i->imm16;
	
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
	return -1;
}

int32_t instr_ret_cb(struct emu_cpu *c, struct emu_cpu_instruction *i)
{ 
	/* CB
	 * Far return to calling procedure
	 * RET
	 */
	return -1;
}

