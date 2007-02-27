/* @header@ */
#include <stdint.h>

#include "emu/emu_cpu.h"
#include "emu/emu_cpu_data.h"
#include "emu/emu_cpu_functions.h"
#include "emu/emu_memory.h"


int32_t instr_group_3_f6(struct emu_cpu *c, struct instruction *i)
{
//	static int32_t (*group_3_f6_fn[8])(struct emu_cpu *c, struct instruction *i) = {
//		/* 0 */ instr_group_3_f6_test,
//		/* 1 */ instr_group_3_f6_test,
//		/* 2 */ instr_group_3_f6_not,
//		/* 3 */ instr_group_3_f6_neg,
//		/* 4 */ instr_group_3_f6_mul,
//		/* 5 */ instr_group_3_f6_imul,
//		/* 6 */ instr_group_3_f6_div,
//		/* 7 */ instr_group_3_f6_idiv,
//	};
//
//	return group_3_f6_fn[i->modrm.opc](c, i);
	return 0;
}


int32_t instr_group_3_f7(struct emu_cpu *c, struct instruction *i)
{
//	static int32_t (*group_3_f7_fn[8])(struct emu_cpu *c, struct instruction *i) = {
//		/* 0 */ instr_group_3_f7_test,
//		/* 1 */ instr_group_3_f7_test,
//		/* 2 */ instr_group_3_f7_not,
//		/* 3 */ instr_group_3_f7_neg,
//		/* 4 */ instr_group_3_f7_mul,
//		/* 5 */ instr_group_3_f7_imul,
//		/* 6 */ instr_group_3_f7_div,
//		/* 7 */ instr_group_3_f7_idiv,
//	};
//
//	return group_3_f7_fn[i->modrm.opc](c, i);
	return 0;
}

