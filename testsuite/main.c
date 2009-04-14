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


#include <stdio.h>
#include <stdlib.h>

#include "emu/emu.h"
#include "emu/emu_memory.h"
#include "emu/emu_log.h"

int main(void)
{
	struct emu *e = emu_new();
	emu_log_level_set(emu_logging_get(e), EMU_LOG_DEBUG);


	/* memory test */
	struct emu_memory *mem = emu_memory_get(e);
	int n = 0xff;
	uint8_t testbytes[n];
	uint32_t testaddresses[n];
	int i;
	
	for( i = 0; i < n; i++ )
	{
		testbytes[i] = rand() % 0xff;
		testaddresses[i] = rand() % 0xff;
		testaddresses[i] <<= 8; 
		testaddresses[i] |= rand() % 0xff;
		testaddresses[i] <<= 8; 
		testaddresses[i] |= rand() % 0xff; 
		testaddresses[i] <<= 8; 
		testaddresses[i] |= rand() % 0xff; 
		
		emu_memory_write_byte(mem, testaddresses[i], testbytes[i]);
	}
	
	
	for( i = 0; i < n; i++ )
	{
		uint8_t byte;
		
		emu_memory_read_byte(mem, testaddresses[i], &byte);
		if( byte != testbytes[i] )
		{
			printf("!!! memtest failed, expected %02x got %02x\n", testbytes[i], byte);
			return -1;
		}
	}

	emu_free(e);

	return 0;
}
