/* @header@ */
#include <stdint.h>
#include <errno.h>

#include "emu/emu.h"
#include "emu/emu_cpu.h"
#include "emu/emu_cpu_data.h"
#include "emu/emu_cpu_functions.h"
#include "emu/emu_cpu_stack.h"
#include "emu/emu_memory.h"

/*Intel Architecture Software Developer's Manual Volume 2: Instruction Set Reference (24319102.PDF) page 631*/

         

int32_t instr_group_2_c0_rol(struct emu_cpu *c, struct instruction *i)
{

	/* C0 /0 ib 
	 * Rotate eight bits r/m8 left imm8 times
	 * ROL r/m8,imm8  
	 */

	return 0;
}

int32_t instr_group_2_c1_rol(struct emu_cpu *c, struct instruction *i)
{

	/* C1 /0 ib 
	 * Rotate 16 bits r/m16 left imm8 times
	 * ROL r/m16,imm8 
	 */

	/* C1 /0 ib 
	 * Rotate 32 bits r/m32 left imm8 times
	 * ROL r/m32,imm8 
	 */

	return 0;
}

int32_t instr_group_2_d0_rol(struct emu_cpu *c, struct instruction *i)
{

	/* D0 /0	 
	 * Rotate eight bits r/m8 left once                        
	 * ROL r/m8,1     
	 */

	return 0;
}

int32_t instr_group_2_d1_rol(struct emu_cpu *c, struct instruction *i)
{
         
	/* D1 /0	 
	 * Rotate 16 bits r/m16 left once                      
	 * ROL r/m16,1    
	 */

	/* D1 /0	 
	 * Rotate 32 bits r/m32 left once         
	 * ROL r/m32,1    
	 */

	return 0;
}

int32_t instr_group_2_d2_rol(struct emu_cpu *c, struct instruction *i)
{
         


	/* D2 /0    
	 * Rotate eight bits r/m8 left CL times                    
	 * ROL r/m8,CL    
	 */

	return 0;
}

int32_t instr_group_2_d3_rol(struct emu_cpu *c, struct instruction *i)
{
																			
	/* D3 /0    
	 * Rotate 16 bits r/m16 left CL times                  
	 * ROL r/m16,CL   
	 */

															   
	/* D3 /0    
	 * Rotate 32 bits r/m32 left CL times     
	 * ROL r/m32,CL   
	 */

	return 0;
}

