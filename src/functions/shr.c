/* @header@ */
#include <stdint.h>
#include <errno.h>

#include "emu/emu.h"
#include "emu/emu_cpu.h"
#include "emu/emu_cpu_data.h"
#include "emu/emu_cpu_functions.h"
#include "emu/emu_cpu_stack.h"
#include "emu/emu_memory.h"

/*Intel Architecture Software Developer's Manual Volume 2: Instruction Set Reference (24319102.PDF) page 662*/

int32_t instr_group_2_c0_shr(struct emu_cpu *c, struct instruction *i)
{

	/* C0 /5 ib 
	 * Unsigned divide r/m8 by 2, imm8 times
	 * SHR r/m8,imm8  
	 */
	return 0;
}

int32_t instr_group_2_c1_shr(struct emu_cpu *c, struct instruction *i)
{

	/* C1 /5 ib 
	 * Unsigned divide r/m16 by 2, imm8 times
	 * SHR r/m16,imm8 
	 */


	/* C1 /5 ib 
	 * Unsigned divide r/m32 by 2, imm8 times
	 * SHR r/m32,imm8 
	 */
	return 0;
}

int32_t instr_group_2_d0_shr(struct emu_cpu *c, struct instruction *i)
{

	/* D0 /5	
	 * Unsigned divide r/m8 by 2, once                      
	 * SHR r/m8,1     
	 */

	return 0;
}

int32_t instr_group_2_d1_shr(struct emu_cpu *c, struct instruction *i)
{


	/* D1 /5	
	 * Unsigned divide r/m16 by 2, once                      
	 * SHR r/m16,1    
	 */

	/* D1 /5	
	 * Unsigned divide r/m32 by 2, once                
	 * SHR r/m32,1    
	 */
	return 0;
}

int32_t instr_group_2_d2_shr(struct emu_cpu *c, struct instruction *i)
{

	/* D2 /5   
	 * Unsigned divide r/m8 by 2, CL times                  
	 * SHR r/m8,CL    
	 */
	return 0;
}

int32_t instr_group_2_d3_shr(struct emu_cpu *c, struct instruction *i)
{

	/* D3 /5   
	 * Unsigned divide r/m16 by 2, CL times                  
	 * SHR r/m16,CL   
	 */

	/* D3 /5   
	 * Unsigned divide r/m32 by 2, CL times            
	 * SHR r/m32,CL   
	 */

	return 0;
}

