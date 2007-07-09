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



#include <stdint.h>

#include "../src/libdasm.c"
#include "../src/libdasm.h"

const char scode[] = "\xFC\xBB\x1E\x88\xB8\x04\xEB\x0C";

void indent(int lev)
{
	int j;
	for ( j=0; j<lev; j++ )
		printf(" ");
}

void instrtree(uint8_t *data, uint8_t datasize, uint8_t offset, uint16_t level)
{
	static char str[256];

	int i;
	for ( i=1;i<15;i++ )
	{
		if (i > offset)
			break;

		INSTRUCTION inst;

		uint8_t isize = get_instruction(&inst, data+offset-i, MODE_32);

		if ( /*isize <= 0 ||*/ isize != i )
			continue;

		indent(level);
		printf("instrsize %i\n", i);


		get_instruction_string(&inst, FORMAT_INTEL, 0, str, sizeof(str));

		indent(level);
		printf("%s\n", str);

		if ( offset - isize >= 1 )
			instrtree(data, datasize, offset-isize, level+1);
//		else
//			return;
	}
}


int main()
{
    

//	int i;
	instrtree((uint8_t *)scode, 8, 8, 0); 
	return 0;
}
