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


#ifndef HAVE_EMU_SHELLCODE_H
#define HAVE_EMU_SHELLCODE_H

#include <stdint.h>

struct emu;

/**
 * Tests a given buffer for possible shellcodes
 * 
 * @param e      the emu
 * @param data   the buffer to test
 * @param size   the size of the buffer
 * 
 * @return on success, the offset within the buffer where the shellcode is suspected
 *         on failure (no shellcode detected), -1
 */
int32_t emu_shellcode_test(struct emu *e, uint8_t *data, uint16_t size);


struct emu_stats
{
	uint32_t eip;

	struct 
	{
		uint32_t read_access;
		uint32_t write_access;
	} memory;

	struct 
	{
		uint32_t steps;
	}cpu;
};




struct emu_stats *emu_stats_new(void);
void emu_stats_free(struct emu_stats *es);


#endif

