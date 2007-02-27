/* @header@ */
#include <stdint.h>
#include <errno.h>

#include "emu/emu.h"
#include "emu/emu_cpu.h"
#include "emu/emu_cpu_data.h"
#include "emu/emu_cpu_functions.h"
#include "emu/emu_cpu_stack.h"
#include "emu/emu_memory.h"

/*Intel Architecture Software Developer's Manual Volume 2: Instruction Set Reference (24319102.PDF) page 127*/

int32_t instr_cmps_a6(struct emu_cpu *c, struct instruction *i)
{

	/* A6 
	 * Compares byte at address DS:(E)SI with byte at address ES:(E)DI and sets the status flags accordingly
	 * CMPSB         
	 * Compares byte at address DS:(E)SI with byte at address ES:(E)DI and sets the status flags accordingly
	 * CMPS m8, m8   
	 */
	return 0;
}


int32_t instr_cmps_a7(struct emu_cpu *c, struct instruction *i)
{
	/* A7 
	 * Compares word at address DS:(E)SI with word at address ES:(E)DI and sets the status flags accordingly
	 * CMPS m16, m16 
	 * Compares word at address DS:(E)SI with word at address ES:(E)DI and sets the status flags accordingly
	 * CMPSW         
	 */

	/* A7 
	 * Compares doubleword at address DS:(E)SI with doubleword at address ES:(E)DI and sets the status flags accordingly
	 * CMPS m32, m32 
	 * CMPSD         
	 * Compares doubleword at address DS:(E)SI with doubleword at address ES:(E)DI and sets the status flags accordingly
	 */
	return 0;
}
