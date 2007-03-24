/* @header@ */
#ifndef HAVE_EMU_FPU_H
#define HAVE_EMU_FPU_H

#include "emu/emu_fpu_instruction.h"

#define FPU_MOD(x) (x[1] >> 6)
#define FPU_RM(x) (x[1] & 7)

struct emu;
struct emu_memory;
struct emu_cpu;

struct emu_fpu
{
	// FIXME fpu registers

	struct emu *emu;
	struct emu_memory *mem;
	struct emu_cpu *cpu;

	struct emu_fpu_instruction fpu_instr;
	struct emu_fpu_instruction_info *fpu_instr_info;
};

struct emu_fpu *emu_fpu_new(struct emu_cpu *e);
void emu_fpu_free(struct emu_fpu *f);
int32_t emu_fpu_parse(struct emu_fpu *f);
int32_t emu_fpu_step(struct emu_fpu *f);


#endif
