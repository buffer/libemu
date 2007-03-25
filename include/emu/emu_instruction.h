/* @header@ */
#ifndef EMU_INSTRUCTiON_H
#define EMU_INSTRUCTION_H

#include <emu/emu_cpu_instruction.h>
#include <emu/emu_fpu_instruction.h>

struct emu_instruction
{
	uint16_t prefixes;
	uint8_t opc;
	uint8_t is_fpu : 1;
	
	union
	{
		struct emu_cpu_instruction cpu;
		struct emu_fpu_instruction fpu;
	};
};

#endif
