/* @header@ */
#include <stdint.h>

#include "emu/emu_cpu.h"
#include "emu/emu_cpu_data.h"
#include "emu/emu_cpu_functions.h"
#include "emu/emu_cpu_stack.h"
#include "emu/emu_memory.h"


int32_t instr_lods_ac(struct emu_cpu *c, struct instruction *i)
{


	/* AC 
	 * Load byte at address DS:(E)SI into AL
	 * LODS m8  
	 */

	/* AC
	 * Load byte at address DS:(E)SI into AL
	 * LODSB    
	 */

	return 0;
}

int32_t instr_lods_ad(struct emu_cpu *c, struct instruction *i)
{

	/* AD 
	 * Load word at address DS:(E)SI into AX
	 * LODS m16 
	 */

	/* AD 
	 * Load doubleword at address DS:(E)SI into EAX
	 * LODS m32 
	 */

	/* AD 
	 * Load word at address DS:(E)SI into AX
	 * LODSW    
	 */

	/* AD
	 * Load doubleword at address DS:(E)SI into EAX
	 * LODSD    
	 */


	return 0;
}

