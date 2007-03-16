/* @header@ */
#ifndef HAVE_EMU_FPU__INSTRUCTION_H
	#define HAVE_EMU_FPU_INSTRUCTION_H

struct emu_fpu;

struct emu_fpu_instruction
{

};


struct emu_fpu_instruction_info
{
	int32_t (*function)(struct emu_fpu *, struct emu_fpu_instruction *);
	const char *name;

	struct
	{
	} format;

};


#endif
