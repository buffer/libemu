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


#ifndef HAVE_EMU_TRACK_H
#define HAVE_EMU_TRACK_H

#include <stdint.h>
#include <stdbool.h>

struct emu;
struct emu_cpu;
struct emu_graph;
struct emu_instruction;



/**
 * The emu_tracking_info struct is used within the
 * binary backwards traversal.
 * The required informations about which registers
 * need to get initialized are stored in the struct together with the
 * eip value where we need to start searching.
 * 
 * @see emu_shellcode_run_and_track
 * @see emu_source_and_track_instr_info
 */
struct emu_tracking_info
{
	uint32_t eip;

	uint32_t eflags;
	uint32_t reg[8];

	uint8_t fpu:1; // used to store the last_instruction information required for fnstenv
};


/**
 * The emu_source_and_track_instr_info struct stores the register/fpu
 * tracking information as well as the source information 
 * for a instruction.
 * Additionally the disassembly of the instruction can be stored 
 * for debugging purposes.
 * 
 * @see emu_shellcode_run_and_track
 */
struct emu_source_and_track_instr_info
{
	uint32_t eip;
	char *instrstring;

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

struct emu_source_and_track_instr_info *emu_source_and_track_instr_info_new(struct emu_cpu *cpu, uint32_t eip_before_instruction);
void emu_source_and_track_instr_info_free(struct emu_source_and_track_instr_info *esantii);
void emu_source_and_track_instr_info_free_void(void *x);

bool emu_source_and_track_instr_info_cmp(void *a, void *b);
uint32_t emu_source_and_track_instr_info_hash(void *key);


struct emu_track_and_source
{
	struct emu_tracking_info track;

	struct emu_graph        *static_instr_graph;
	struct emu_hashtable    *static_instr_table;

	struct emu_graph        *run_instr_graph;
	struct emu_hashtable    *run_instr_table;

};


struct emu_track_and_source *emu_track_and_source_new(void);
void emu_track_and_source_free(struct emu_track_and_source *et);

int32_t emu_track_instruction_check(struct emu *e, struct emu_track_and_source *et);



struct emu_tracking_info *emu_tracking_info_new(void);
void emu_tracking_info_free(struct emu_tracking_info *eti);

void emu_tracking_info_clear(struct emu_tracking_info *eti);


/**
 * Calculate the logic difference between two instruction_infos 
 * and store the result.
 * 
 * @param a
 * @param b
 * @param result
 */
void emu_tracking_info_diff(struct emu_tracking_info *a, struct emu_tracking_info *b, struct emu_tracking_info *result);

void emu_tracking_info_copy(struct emu_tracking_info *from, struct emu_tracking_info *to);

/**
 * Check if a instruction can satisfy 
 * the requirements of another instruction.
 * 
 * @param a
 * @param b
 * 
 * @return returns true if a covers the requirements of b
 *         else false
 */
bool emu_tracking_info_covers(struct emu_tracking_info *a, struct emu_tracking_info *b);

void emu_tracking_info_debug_print(struct emu_tracking_info *a);



#endif
