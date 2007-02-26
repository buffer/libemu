/* @header@ */
#include <stdint.h>

#include "emu/emu_cpu.h"
#include "emu/emu_cpu_data.h"
#include "emu/emu_cpu_functions.h"
#include "emu/emu_memory.h"

int32_t prefix_fn(struct emu_cpu *c, struct instruction *i)
{
	/* dummy */
	return 0;
}

int32_t instr_cld_fc(struct emu_cpu *c, struct instruction *i)
{
	/*Intel Architecture Software Developer's Manual Volume 2: Instruction Set Reference (24319102.PDF) page 107*/
	CPU_FLAG_UNSET(c,f_df);
	return 0;
}


int32_t instr_lea_8d(struct emu_cpu *c, struct instruction *i)
{
	/*Intel Architecture Software Developer's Manual Volume 2: Instruction Set Reference (24319102.PDF) page 393*/

	if ( i->prefixes & PREFIX_OPSIZE )
	{
		/* 8D /r 
		 * Store effective address for m in register r16
		 * LEA r16,m 
		 */
	}
	else
	{
		/* 8D /r 
		 * Store effective address for m in register r32
		 * LEA r32,m 
		 */
	}
	return 0;

}
