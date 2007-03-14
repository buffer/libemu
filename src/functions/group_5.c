/* @header@ */
#include <stdint.h>
#include <errno.h>

#include "emu/emu.h"
#include "emu/emu_cpu.h"
#include "emu/emu_cpu_data.h"
#include "emu/emu_cpu_functions.h"
#include "emu/emu_cpu_stack.h"
#include "emu/emu_memory.h"




int32_t instr_group_5_ff(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	static int32_t (*group_5_fn[8])(struct emu_cpu *c, struct emu_cpu_instruction *i) = {
		/* 0 */ instr_group_5_ff_inc,
		/* 1 */ instr_group_5_ff_dec,
		/* 2 */ instr_group_5_ff_call,
		/* 3 */ instr_group_5_ff_call,
		/* 4 */ instr_group_5_ff_jmp,
		/* 5 */ instr_group_5_ff_jmp,
		/* 6 */ instr_group_5_ff_push,
		/* 7 */ 0,
	};

	return group_5_fn[i->modrm.opc](c, i);
}
