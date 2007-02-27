/* @header@ */
#include <stdint.h>
#include <errno.h>

#include "emu/emu.h"
#include "emu/emu_cpu.h"
#include "emu/emu_cpu_data.h"
#include "emu/emu_cpu_functions.h"
#include "emu/emu_cpu_stack.h"
#include "emu/emu_memory.h"

/*Intel Architecture Software Developer's Manual Volume 2: Instruction Set Reference (24319102.PDF) page 188*/

int32_t instr_group_3_f6_div(struct emu_cpu *c, struct instruction *i)
{
	/* F6 /6 
	 * Unsigned divide AX by r/m8; AL <- Quotient, AH <- Remainder
	 * DIV r/m8  
	 */
	return 0;
}

int32_t instr_group_3_f7_div(struct emu_cpu *c, struct instruction *i)
{
	/* F7 /6 
	 * Unsigned divide DX:AX by r/m16; AX <- Quotient, DX <- Remainder
	 * DIV r/m16 
	 */

	/* F7 /6 
	 * Unsigned divide EDX:EAX by r/m32 doubleword; EAX <- Quotient, EDX <- Remainder
	 * DIV r/m32 
	 */
	return 0;
}

