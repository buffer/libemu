/* @header@ */
#include <stdint.h>

#include "emu/emu_cpu.h"
#include "emu/emu_cpu_data.h"
#include "emu/emu_cpu_functions.h"
#include "emu/emu_memory.h"


int32_t instr_group_2_c0(struct emu_cpu *c, struct instruction *i)
{
	static int32_t (*group_2_c0_fn[8])(struct emu_cpu *c, struct instruction *i) = {
		/* 0 */ instr_group_2_c0_rol,
		/* 1 */ instr_group_2_c0_ror,
		/* 2 */ instr_group_2_c0_rcl,
		/* 3 */ instr_group_2_c0_rcr,
		/* 4 */ instr_group_2_c0_sal,
		/* 5 */ instr_group_2_c0_shr,
		/* 6 */ 0,
		/* 7 */ instr_group_2_c0_sar,
	};

	return group_2_c0_fn[i->modrm.opc](c, i);
}


int32_t instr_group_2_c1(struct emu_cpu *c, struct instruction *i)
{
	static int32_t (*group_2_c1_fn[8])(struct emu_cpu *c, struct instruction *i) = {
		/* 0 */ instr_group_2_c1_rol,
		/* 1 */ instr_group_2_c1_ror,
		/* 2 */ instr_group_2_c1_rcl,
		/* 3 */ instr_group_2_c1_rcr,
		/* 4 */ instr_group_2_c1_sal,
		/* 5 */ instr_group_2_c1_shr,
		/* 6 */ 0,
		/* 7 */ instr_group_2_c1_sar,
	};

	return group_2_c1_fn[i->modrm.opc](c, i);
}


int32_t instr_group_2_d0(struct emu_cpu *c, struct instruction *i)
{
	static int32_t (*group_2_d0_fn[8])(struct emu_cpu *c, struct instruction *i) = {
		/* 0 */ instr_group_2_d0_rol,
		/* 1 */ instr_group_2_d0_ror,
		/* 2 */ instr_group_2_d0_rcl,
		/* 3 */ instr_group_2_d0_rcr,
		/* 4 */ instr_group_2_d0_sal,
		/* 5 */ instr_group_2_d0_shr,
		/* 6 */ 0,
		/* 7 */ instr_group_2_d0_sar,
	};

	return group_2_d0_fn[i->modrm.opc](c, i);
}

int32_t instr_group_2_d1(struct emu_cpu *c, struct instruction *i)
{
	static int32_t (*group_2_d1_fn[8])(struct emu_cpu *c, struct instruction *i) = {
		/* 0 */ instr_group_2_d1_rol,
		/* 1 */ instr_group_2_d1_ror,
		/* 2 */ instr_group_2_d1_rcl,
		/* 3 */ instr_group_2_d1_rcr,
		/* 4 */ instr_group_2_d1_sal,
		/* 5 */ instr_group_2_d1_shr,
		/* 6 */ 0,
		/* 7 */ instr_group_2_d1_sar,
	};

	return group_2_d1_fn[i->modrm.opc](c, i);
}


int32_t instr_group_2_d2(struct emu_cpu *c, struct instruction *i)
{
	static int32_t (*group_2_d2_fn[8])(struct emu_cpu *c, struct instruction *i) = {
		/* 0 */ instr_group_2_d2_rol,
		/* 1 */ instr_group_2_d2_ror,
		/* 2 */ instr_group_2_d2_rcl,
		/* 3 */ instr_group_2_d2_rcr,
		/* 4 */ instr_group_2_d2_sal,
		/* 5 */ instr_group_2_d2_shr,
		/* 6 */ 0,
		/* 7 */ instr_group_2_d2_sar,
	};

	return group_2_d2_fn[i->modrm.opc](c, i);
}


int32_t instr_group_2_d3(struct emu_cpu *c, struct instruction *i)
{
	static int32_t (*group_2_d3_fn[8])(struct emu_cpu *c, struct instruction *i) = {
		/* 0 */ instr_group_2_d3_rol,
		/* 1 */ instr_group_2_d3_ror,
		/* 2 */ instr_group_2_d3_rcl,
		/* 3 */ instr_group_2_d3_rcr,
		/* 4 */ instr_group_2_d3_sal,
		/* 5 */ instr_group_2_d3_shr,
		/* 6 */ 0,
		/* 7 */ instr_group_2_d3_sar,
	};

	return group_2_d3_fn[i->modrm.opc](c, i);
}

