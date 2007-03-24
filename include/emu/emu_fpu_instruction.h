/* @header@ */
#ifndef HAVE_EMU_FPU_INSTRUCTION_H
#define HAVE_EMU_FPU_INSTRUCTION_H

#define FPU_MOD(fpu) (fpu[1] >> 6)
#define FPU_RM(fpu) (fpu[1] & 7)

struct emu_fpu_instruction
{
	uint16_t prefixes;
	
	uint8_t fpu_data[2]; /* TODO: split into correct fields */
};

#endif
