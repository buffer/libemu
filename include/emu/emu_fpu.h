/* @header@ */
#ifndef HAVE_EMU_FPU_H
#define HAVE_EMU_FPU_H

#include "emu/emu_fpu_instruction.h"

struct emu;

struct emu_fpu
{
	// FIXME fpu registers

	struct emu *emu;

	struct emu_fpu_instruction fpu_instr;
	struct emu_fpu_instruction_info *fpu_instr_info;
};

struct emu_fpu *emu_fpu_new(struct emu *e);
void emu_fpu_free(struct emu_fpu *f);
int32_t emu_fpu_parse(struct emu_fpu *f);
int32_t emu_fpu_step(struct emu_fpu *f);


#endif
