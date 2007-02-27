/* @header@ */
#include <stdint.h>
#include <errno.h>

#include "emu/emu.h"
#include "emu/emu_cpu.h"
#include "emu/emu_cpu_data.h"
#include "emu/emu_cpu_functions.h"
#include "emu/emu_cpu_stack.h"
#include "emu/emu_memory.h"

/*Intel Architecture Software Developer's Manual Volume 2: Instruction Set Reference (24319102.PDF) page 488*/

int32_t instr_group_3_f6_mul(struct emu_cpu *c, struct instruction *i)
{
	/* F6 /4 
	 * Unsigned multiply (AX <- AL * r/m8)             
	 * MUL r/m8  
	 */
	return 0;
}

int32_t instr_group_3_f7_mul(struct emu_cpu *c, struct instruction *i)
{
	/* F7 /4 
	 * Unsigned multiply (DX:AX <- AX * r/m16)         
	 * MUL r/m16 
	 */																		 
																		 

	/* F7 /4 
	 * Unsigned multiply (EDX:EAX <- EAX * r/m32)      
	 * MUL r/m32 
	 */
	return 0;
}
