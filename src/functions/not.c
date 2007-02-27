/* @header@ */
#include <stdint.h>
#include <errno.h>

#include "emu/emu.h"
#include "emu/emu_cpu.h"
#include "emu/emu_cpu_data.h"
#include "emu/emu_cpu_functions.h"
#include "emu/emu_cpu_stack.h"
#include "emu/emu_memory.h"

/*Intel Architecture Software Developer's Manual Volume 2: Instruction Set Reference (24319102.PDF) page 497*/

int32_t instr_group_3_f6_not(struct emu_cpu *c, struct instruction *i)
{
	/* F6 /2  
	 * Reverse each bit of r/m8
	 * NOT r/m8    
	 */
	return 0;
}

int32_t instr_group_3_f7_not(struct emu_cpu *c, struct instruction *i)
{
	/* F7 /2  
	 * Reverse each bit of r/m16
	 * NOT r/m16   
	 */	  

	/* F7 /2  
	 * Reverse each bit of r/m32
	 * NOT r/m32   
	 */
	return 0;
}
