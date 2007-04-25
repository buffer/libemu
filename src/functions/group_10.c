/* @header@ */
#include <stdint.h>

#include "emu/emu_cpu.h"
#include "emu/emu_cpu_data.h"
#include "emu/emu_cpu_functions.h"
#include "emu/emu_cpu_stack.h"
#include "emu/emu_memory.h"

int32_t instr_group_10_8f_pop(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	if (i->prefixes & PREFIX_OPSIZE)
	{
    /* 8F /0  
	 * Pop top of stack into m16; increment stack pointer  
	 * POP m16 
	 */
	}else
	{
    /* 8F /0  
	 * Pop top of stack into m32; increment stack pointer  
	 * POP m32 
	 */	
		uint32_t m32;
		POP_DWORD(c, &m32);
		MEM_DWORD_WRITE(c, i->modrm.ea, m32);
	}

	return 0;
}


int32_t instr_group_10_8f(struct emu_cpu *c, struct emu_cpu_instruction *i)
{

	static int32_t (*group_10_8f_fn[8])(struct emu_cpu *c, struct emu_cpu_instruction *i) = {
		/* 0 */ instr_group_10_8f_pop,
		/* 1 */ NULL,
		/* 2 */ NULL,
		/* 3 */ NULL,
		/* 4 */ NULL,
		/* 5 */ NULL,
		/* 6 */ NULL,
		/* 7 */ NULL,
	};

	if ( group_10_8f_fn[i->modrm.opc] != NULL )
		return group_10_8f_fn[i->modrm.opc](c, i);
	else
		return 0;
}

