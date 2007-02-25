/* @header@ */
#include <stdint.h>
#include <errno.h>

#include "emu/emu.h"
#include "emu/emu_cpu.h"
#include "emu/emu_cpu_data.h"
#include "emu/emu_cpu_functions.h"
#include "emu/emu_cpu_stack.h"
#include "emu/emu_memory.h"

/*Intel Architecture Software Developer's Manual Volume 2: Instruction Set Reference (24319102.PDF) page 621*/


int32_t instr_group_2_c0_ror(struct emu_cpu *c, struct instruction *i)
{
	/* C0 /1 ib 
	 * Rotate eight bits r/m16 right imm8 times
	 * ROR r/m8,imm8  
	 */
	return 0;
}

int32_t instr_group_2_c1_ror(struct emu_cpu *c, struct instruction *i)
{
	/* C1 /1 ib 
	 * Rotate 16 bits r/m16 right imm8 times
	 * ROR r/m16,imm8 
	 */
	/* C1 /1 ib 
	 * Rotate 32 bits r/m32 right imm8 times
	 * ROR r/m32,imm8 
	 */
	return 0;
}

int32_t instr_group_2_d0_ror(struct emu_cpu *c, struct instruction *i)
{

	/* D0 /1	 
	 * Rotate eight bits r/m8 right once              
	 * ROR r/m8,1     
	 */
	return 0;
}

int32_t instr_group_2_d1_ror(struct emu_cpu *c, struct instruction *i)
{
	/* D1 /1	 
	 * Rotate 16 bits r/m16 right once                 
	 * ROR r/m16,1    
	 */																		
	/* D1 /1	 
	 * Rotate 32 bits r/m32 right once                
	 * ROR r/m32,1    
	 */																	   
	return 0;
}

int32_t instr_group_2_d2_ror(struct emu_cpu *c, struct instruction *i)
{

	/* D2 /1    
	 * Rotate eight bits r/m8 right CL times          
	 * ROR r/m8,CL    
	 */
	return 0;
}

int32_t instr_group_2_d3_ror(struct emu_cpu *c, struct instruction *i)
{
         

	/* D3 /1    
	 * Rotate 16 bits r/m16 right CL times             
	 * ROR r/m16,CL   
	 */

         
	/* D3 /1    
	 * Rotate 32 bits r/m32 right CL times            
	 * ROR r/m32,CL   
	 */

	return 0;
}

