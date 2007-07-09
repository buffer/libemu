/********************************************************************************
 *                               libemu
 *
 *                    - x86 shellcode emulation -
 *
 *
 * Copyright (C) 2007  Paul Baecher & Markus Koetter
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 * 
 * 
 *             contact nepenthesdev@users.sourceforge.net  
 *
 *******************************************************************************/

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
