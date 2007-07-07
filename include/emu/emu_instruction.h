/* @header@ */
#ifndef EMU_INSTRUCTION_H
#define EMU_INSTRUCTION_H

#include <emu/emu_cpu_instruction.h>
#include <emu/emu_fpu_instruction.h>
#include "emu/emu_track.h"



/**
 * The emu_instruction is the dummy struct for fpu/cpu instructions
 * The track & source information is used to 
 * create the static instruction graph and run the binary 
 * backwards traversal.
 * 
 * @see emu_tracking_info
 */
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

	struct 
	{
		struct emu_tracking_info init;
		struct emu_tracking_info need;		
	} track;

	struct 
	{
		uint8_t has_cond_pos : 1;
		uint32_t norm_pos;
		uint32_t cond_pos;
	} source;
};

#endif
