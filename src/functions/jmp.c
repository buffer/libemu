/* @header@ */
#include <stdint.h>

#include "emu/emu_cpu.h"
#include "emu/emu_cpu_data.h"
#include "emu/emu_cpu_functions.h"
#include "emu/emu_memory.h"


int32_t instr_jmp_e9(struct emu_cpu *c, struct instruction *i)
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

	return 0;
}


int32_t instr_jmp_ea(struct emu_cpu *c, struct instruction *i)
{

	/* EA cd  
	 * Jump far, absolute, address given in operand                           
	 * JMP ptr16:16 
	 */


	/* EA cp  
	 * Jump far, absolute, address given in operand                           
	 * JMP ptr16:32 
	 */


	return 0;
}

int32_t instr_jmp_eb(struct emu_cpu *c, struct instruction *i)
{
	/* EB cb  
	 * Jump short, relative, displacement relative to next instruction        
	 * JMP rel8     
	 */

	c->eip += i->disp;

	return 0;
}

