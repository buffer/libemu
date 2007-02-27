/* @header@ */
#include <stdint.h>
#include <errno.h>

#include "emu/emu.h"
#include "emu/emu_cpu.h"
#include "emu/emu_cpu_data.h"
#include "emu/emu_cpu_functions.h"
#include "emu/emu_cpu_stack.h"
#include "emu/emu_memory.h"

/*Intel Architecture Software Developer's Manual Volume 2: Instruction Set Reference (24319102.PDF) page 669*/

int32_t instr_scas_ae(struct emu_cpu *c, struct instruction *i)
{
	/* AE 
     * Compare AL with byte at ES:(E)DI and set status flags
	 * SCAS m8  
	 * Compare AL with byte at ES:(E)DI and set status flags
	 * SCASB    
	 */
	return 0;
}


int32_t instr_scas_af(struct emu_cpu *c, struct instruction *i)
{
	/* AF 
	 * Compare AX with word at ES:(E)DI and set status flags
	 * SCAS m16 
	 * Compare AX with word at ES:(E)DI and set status flags
	 * SCASW    
	 */

	/* AF 
	 * Compare EAX with doubleword at ES(E)DI and set status flags
	 * SCAS m32 
	 * Compare EAX with doubleword at ES:(E)DI and set status flags
	 * SCASD    
	 */

	return 0;
}
