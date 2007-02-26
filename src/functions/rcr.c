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


int32_t instr_group_2_c0_rcr(struct emu_cpu *c, struct instruction *i)
{
	
	/* C0 /3 ib 
	 * Rotate nine bits (CF,r/m8) right imm8 times
	 * RCR r/m8,imm8  
	 */
         
	return 0;
}

int32_t instr_group_2_c1_rcr(struct emu_cpu *c, struct instruction *i)
{

	/* C1 /3 ib 
	 * Rotate 17 bits (CF,r/m16) right imm8 times
	 * RCR r/m16,imm8 
	 */
	/* C1 /3 ib 
	 * Rotate 33 bits (CF,r/m32) right imm8 times
	 * RCR r/m32,imm8 
	 */

	return 0;
}

int32_t instr_group_2_d0_rcr(struct emu_cpu *c, struct instruction *i)
{

	/* D0 /3	 
	 * Rotate nine bits (CF,r/m8) right once                    
	 * RCR r/m8,1     
	 */
	return 0;
}

int32_t instr_group_2_d1_rcr(struct emu_cpu *c, struct instruction *i)
{

	/* D1 /3	 
	 * Rotate 17 bits (CF,r/m16) right once              
	 * RCR r/m16,1    
	 */
																		  
        
	/* D1 /3	 
	 * Rotate 33 bits (CF,r/m32) right once                     
	 * RCR r/m32,1    
	 */

	return 0;
}

int32_t instr_group_2_d2_rcr(struct emu_cpu *c, struct instruction *i)
{

	/* D2 /3    
	 * Rotate nine bits (CF,r/m8) right CL times                
	 * RCR r/m8,CL    
	 */
	return 0;
}

int32_t instr_group_2_d3_rcr(struct emu_cpu *c, struct instruction *i)
{

	/* D3 /3    
	 * Rotate 17 bits (CF,r/m16) right CL times          
	 * RCR r/m16,CL   
	 */
																				 
	/* D3 /3    
	 * Rotate 33 bits (CF,r/m32) right CL times                 
	 * RCR r/m32,CL   
	 */

	return 0;
}

