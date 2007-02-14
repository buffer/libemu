/* @header@ */
#include <stdint.h>

#include "emu/emu_cpu.h"
#include "emu/emu_cpu_data.h"
#include "emu/emu_cpu_functions.h"
#include "emu/emu_memory.h"

int32_t instr_group_5_ff_call(struct emu_cpu *c, struct instruction *i)
{

	/* FF /2  
	 * CALL r/m16    
	 * Call near, absolute indirect, address given in r/m16   
	 */

	/* FF /2  
	 * CALL r/m32    
	 * Call near, absolute indirect, address given in r/m32   
	 */

	/* FF /3  
	 * CALL m16:16   
	 * Call far, absolute indirect, address given in m16:16  
	 */

	/* FF /3  
	 * CALL m16:32   
	 * Call far, absolute indirect, address given in m16:32  
	 */


	return 0;
}



int32_t instr_group_5_ff_jmp(struct emu_cpu *c, struct instruction *i)
{

	/* FF /4  
	 * Jump near, absolute indirect, address given in r/m16                   
	 * JMP r/m16    
	 */


	/* FF /4  
	 * Jump near, absolute indirect, address given in r/m32                   
	 * JMP r/m32    
	 */


	/* FF /5  
	 * Jump far, absolute indirect, address given in m16:16                   
	 * JMP m16:16   
	 */


	/* FF /5  
	 * Jump far, absolute indirect, address given in m16:32                   
	 * JMP m16:32   
	 */


	return 0;
}


int32_t instr_group_5_ff_push(struct emu_cpu *c, struct instruction *i)
{
	/* FF /6 
	 * Push r/m16    
	 * PUSH r/m16 
	 */

	/* FF /6 
	 * Push r/m32    
	 * PUSH r/m32 
	 */

	return 0;
}

