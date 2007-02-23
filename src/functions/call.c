/* @header@ */
#include <stdint.h>
#include <errno.h>

#include "emu/emu.h"
#include "emu/emu_cpu.h"
#include "emu/emu_cpu_data.h"
#include "emu/emu_cpu_functions.h"
#include "emu/emu_cpu_stack.h"
#include "emu/emu_memory.h"


int32_t instr_call_9a(struct emu_cpu *c, struct instruction *i)
{

	/* 9A cd  
	 * CALL ptr16:16 
	 * Call far, absolute, address given in operand
	 */



	/* 9A cp  
	 * CALL ptr16:32 
	 * Call far, absolute, address given in operand
	 */

	return 0;
}


int32_t instr_call_e8(struct emu_cpu *c, struct instruction *i)
{

	/* E8 cd  
	 * CALL rel32    
	 * Call near, relative, displacement relative to next instruction
	 */


	/* E8 cw  
	 * CALL rel16    
	 * Call near, relative, displacement relative to next instruction
	 */

	PUSH_DWORD(c, c->eip);
	c->eip += i->disp;

	return 0;
}


