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

#ifndef HAVE_EMU_FPU_INSTRUCTION_H
#define HAVE_EMU_FPU_INSTRUCTION_H

#define FPU_MOD(fpu) (fpu[1] >> 6)
#define FPU_RM(fpu) (fpu[1] & 7)

#define FPU_MF(fpu) ((fpu[0] >> 1) & 3)

struct emu_fpu_instruction
{
	uint16_t prefixes;
	
	uint8_t fpu_data[2]; /* TODO: split into correct fields */
	uint32_t ea;
	
	uint32_t last_instr;

};

#endif
