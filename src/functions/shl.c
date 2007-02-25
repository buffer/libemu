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

int32_t instr_group_2_c0_shl(struct emu_cpu *c, struct instruction *i)
{
	/* C0 /4 ib 
	 * Multiply r/m8 by 2, imm8 times
	 * SHL r/m8,imm8  
	 */
	return 0;
}

int32_t instr_group_2_c1_shl(struct emu_cpu *c, struct instruction *i)
{
	/* C1 /4 ib 
	 * Multiply r/m16 by 2, imm8 times
	 * SHL r/m16,imm8 
	 */

	/* C1 /4 ib 
	 * Multiply r/m32 by 2, imm8 times
	 * SHL r/m32,imm8 
	 */
	return 0;
}

int32_t instr_group_2_d0_shl(struct emu_cpu *c, struct instruction *i)
{

	/* D0 /4	
	 * Multiply r/m8 by 2, once                       
	 * SHL r/m8,1     
	 */
	return 0;
}

int32_t instr_group_2_d1_shl(struct emu_cpu *c, struct instruction *i)
{
	/* D1 /4	
	 * Multiply r/m16 by 2, once                       
	 * SHL r/m16,1    
	 */


	/* D1 /4	
	 * Multiply r/m32 by 2, once             
	 * SHL r/m32,1    
	 */
	return 0;
}

int32_t instr_group_2_d2_shl(struct emu_cpu *c, struct instruction *i)
{
	/* D2 /4   
	 * Multiply r/m8 by 2, CL times                   
	 * SHL r/m8,CL    
	 */
	return 0;
}

int32_t instr_group_2_d3_shl(struct emu_cpu *c, struct instruction *i)
{
	/* D3 /4   
	 * Multiply r/m16 by 2, CL times                   
	 * SHL r/m16,CL   
	 */

	/* D3 /4   
	 * Multiply r/m32 by 2, CL times         
	 * SHL r/m32,CL   
	 */

	return 0;
}

