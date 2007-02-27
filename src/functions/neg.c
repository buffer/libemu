/* @header@ */
#include <stdint.h>
#include <errno.h>

#include "emu/emu.h"
#include "emu/emu_cpu.h"
#include "emu/emu_cpu_data.h"
#include "emu/emu_cpu_functions.h"
#include "emu/emu_cpu_stack.h"
#include "emu/emu_memory.h"

/*Intel Architecture Software Developer's Manual Volume 2: Instruction Set Reference (24319102.PDF) page 494*/

int32_t instr_group_3_f6_neg(struct emu_cpu *c, struct instruction *i)
{
	/* F6 /3 
	 * Two's complement negate r/m8
	 * NEG r/m8  
	 */
	return 0;
}

int32_t instr_group_3_f7_neg(struct emu_cpu *c, struct instruction *i)
{
	/* F7 /3 
	 * Two's complement negate r/m16
	 * NEG r/m16 
	 */


	/* F7 /3 
	 * Two's complement negate r/m32
	 * NEG r/m32 
	 */
	return 0;
}




