/* @header@ */
#include <stdint.h>
#include <errno.h>

#include "emu/emu.h"
#include "emu/emu_cpu.h"
#include "emu/emu_cpu_data.h"
#include "emu/emu_cpu_functions.h"
#include "emu/emu_cpu_stack.h"
#include "emu/emu_memory.h"

/*Intel Architecture Software Developer's Manual Volume 2: Instruction Set Reference (24319102.PDF) page 332*/

int32_t instr_group_3_f6_idiv(struct emu_cpu *c, struct instruction *i)
{
	/* F6 /7 
	 * Signed divide AX (where AH must contain sign-extension of AL) by r/m byte. (Results: AL=Quotient,AH=Remainder)
	 * IDIV r/m8  
	 */
	return 0;
}


int32_t instr_group_3_f7_idiv(struct emu_cpu *c, struct instruction *i)
{
	/* F7 /7 
	 * Signed divide DX:AX (where DX must contain sign-extension of AX) by r/m word. (Results: AX=Quotient,DX=Remainder)
	 * IDIV r/m16 
	 */

	/* F7 /7 
	 * Signed divide EDX:EAX (where EDX must contain sign-extension of EAX) by r/m doubleword. (Results: EAX=Quotient, EDX=Remainder)
	 * IDIV r/m32 
	 */
	return 0;
}
