/* @header@ */

#include <inttypes.h>

#include "emu/emu_cpu.h"
#include "emu/emu_cpu_data.h"
#include "emu/emu_cpu_stack.h"
#include "emu/emu_memory.h"

int32_t instr_ret_c2(struct emu_cpu *c, struct instruction *i)
{ /* C2 iw RET imm16 Near return to calling procedure and pop imm16 bytes from stack */
	POP_DWORD(c, &c->eip);
	
	c->reg[esp] += *i->imm16;
	
	return 0;
}

int32_t instr_ret_c3(struct emu_cpu *c, struct instruction *i)
{ /* C3    RET       Near return to calling procedure */
	POP_DWORD(c, &c->eip);
	
	return 0;
}

int32_t instr_ret_ca(struct emu_cpu *c, struct instruction *i)
{ /* CA iw RET imm16 Far return to calling procedure and pop imm16 bytes from stack */
	return -1;
}

int32_t instr_ret_cb(struct emu_cpu *c, struct instruction *i)
{ /* CB    RET       Far return to calling procedure */
	return -1;
}

