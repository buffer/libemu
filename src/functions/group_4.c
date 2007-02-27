/* @header@ */
#include <stdint.h>

#include "emu/emu_cpu.h"
#include "emu/emu_cpu_data.h"
#include "emu/emu_cpu_functions.h"
#include "emu/emu_memory.h"


int32_t instr_group_4_fe(struct emu_cpu *c, struct instruction *i)
{
	static int32_t (*group_4_fe_fn[8])(struct emu_cpu *c, struct instruction *i) = {
		/* 0 */ instr_group_4_fe_inc,
		/* 1 */ instr_group_4_fe_dec,
		/* 2 */ 0,
		/* 3 */ 0,
		/* 4 */ 0,
		/* 5 */ 0,
		/* 6 */ 0,
		/* 7 */ 0,
	};

	return group_4_fe_fn[i->modrm.opc](c, i);
}
