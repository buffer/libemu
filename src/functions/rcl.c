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

int32_t instr_group_2_c0_rcl(struct emu_cpu *c, struct instruction *i)
{
	/* C0 /2 ib 
	 * Rotate nine bits (CF,r/m8) left imm8 times
	 * RCL r/m8,imm8  
	 */
	return 0;
}

int32_t instr_group_2_c1_rcl(struct emu_cpu *c, struct instruction *i)
{
	/* C1 /2 ib 
	 * Rotate 17 bits (CF,r/m16) left imm8 times
	 * RCL r/m16,imm8
	 */

	/* C1 /2 ib 
	 * Rotate 33 bits (CF,r/m32) left imm8 times
	 * RCL r/m32,imm8 
	 */

	return 0;
}

int32_t instr_group_2_d0_rcl(struct emu_cpu *c, struct instruction *i)
{


	/* D0 /2	 
	 * Rotate nine bits (CF,r/m8) left once        
	 * RCL r/m8,1     
	 */
	return 0;
}

int32_t instr_group_2_d1_rcl(struct emu_cpu *c, struct instruction *i)
{

	/* D1 /2    
	 * Rotate 17 bits (CF,r/m16) left once
	 * RCL r/m16,1    
	 */

		 
	/* D1 /2    
	 * Rotate 33 bits (CF,r/m32) left once
	 * RCL r/m32,1    
	 */
	return 0;
}

int32_t instr_group_2_d2_rcl(struct emu_cpu *c, struct instruction *i)
{
		 
	/* D2 /2    
	 * Rotate nine bits (CF,r/m8) left CL times    
	 * RCL r/m8,CL    
	 */
	return 0;
}

int32_t instr_group_2_d3_rcl(struct emu_cpu *c, struct instruction *i)
{

	/* D3 /2    
	 * Rotate 17 bits (CF,r/m16) left CL times
	 * RCL r/m16,CL   
	 */


	/* D3 /2    
	 * Rotate 33 bits (CF,r/m32) left CL times
	 * RCL r/m32,CL
	 */


	return 0;
}

