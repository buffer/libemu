/* @header@ */
#include <stdint.h>

#include "emu/emu_cpu.h"
#include "emu/emu_cpu_data.h"
#include "emu/emu_cpu_functions.h"
#include "emu/emu_cpu_stack.h"
#include "emu/emu_memory.h"



int32_t instr_stos_aa(struct emu_cpu *c, struct instruction *i)
{

	/* AA
	 * Store AL at address ES:(E)DI
	 * STOS m8  
	 */


	/* AA 
	 * Store AL at address ES:(E)DI
	 * STOSB    
	 */

	return 0;
}

int32_t instr_stos_ab(struct emu_cpu *c, struct instruction *i)
{

	/* AB
	 * Store AX at address ES:(E)DI
	 * STOS m16 
	 */

	/* AB 
	 * Store AX at address ES:(E)DI
	 * STOSW    
	 */


	/* AB 
	 * Store EAX at address ES:(E)DI
	 * STOS m32 
	 */


	/* AB 
	 * Store EAX at address ES:(E)DI
	 * STOSD    
	 */

	return 0;
}

