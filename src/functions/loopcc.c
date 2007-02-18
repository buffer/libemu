/* @header@ */
#include <stdint.h>

#include "emu/emu_cpu.h"
#include "emu/emu_cpu_data.h"
#include "emu/emu_cpu_functions.h"
#include "emu/emu_memory.h"


int32_t instr_loopcc_e0(struct emu_cpu *c, struct instruction *i)
{				   

	/* E0 cb  
	 * Decrement count; jump short if count != 0 and ZF=0
	 * LOOPNE rel8
	 * LOOPNZ rel8 
	 */



	return 0;
}

int32_t instr_loopcc_e1(struct emu_cpu *c, struct instruction *i)
{				   


	/* E1 cb  
	 * Decrement count; jump short if count != 0 and ZF=1
	 * LOOPE rel8  
	 * LOOPZ rel8  
	 */				   

	return 0;
}

int32_t instr_loop_e2(struct emu_cpu *c, struct instruction *i)
{				   
	/* E2 cb  
	 * Decrement count; jump short if count != 0
	 * LOOP rel8   
	 */



	return 0;
}
				   


