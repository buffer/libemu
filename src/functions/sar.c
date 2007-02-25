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

int32_t instr_group_2_c0_sar(struct emu_cpu *c, struct instruction *i)
{
	/* C0 /7 ib 
	 * Signed divide* r/m8 by 2, imm8 times
	 * SAR r/m8,imm8  
	 */
	return 0;
}

int32_t instr_group_2_c1_sar(struct emu_cpu *c, struct instruction *i)
{

	/* C1 /7 ib 
	 * Signed divide* r/m16 by 2, imm8 times
	 * SAR r/m16,imm8 
	 */

	/* C1 /7 ib 
	 * Signed divide* r/m32 by 2, imm8 times
	 * SAR r/m32,imm8 
	 */
	return 0;
}

int32_t instr_group_2_d0_sar(struct emu_cpu *c, struct instruction *i)
{

	/* D0 /7	
	 * Signed divide* r/m8 by 2, once            
	 * SAR r/m8,1     
	 */
	return 0;
}

int32_t instr_group_2_d1_sar(struct emu_cpu *c, struct instruction *i)
{

	/* D1 /7	
	 * Signed divide* r/m16 by 2, once                      
	 * SAR r/m16,1    
	 */

	/* D1 /7	
	 * Signed divide* r/m32 by 2, once                 
	 * SAR r/m32,1    
	 */
	return 0;
}

int32_t instr_group_2_d2_sar(struct emu_cpu *c, struct instruction *i)
{


	/* D2 /7   
	 * Signed divide* r/m8 by 2, CL times        
	 * SAR r/m8,CL    
	 */
	return 0;
}

int32_t instr_group_2_d3_sar(struct emu_cpu *c, struct instruction *i)
{

	/* D3 /7   
	 * Signed divide* r/m16 by 2, CL times                  
	 * SAR r/m16,CL   
	 */

	/* D3 /7   
	 * Signed divide* r/m32 by 2, CL times             
	 * SAR r/m32,CL   
	 */

	return 0;
}

