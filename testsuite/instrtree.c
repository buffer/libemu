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
/* JMPCall 
const char scode[] = "\xfc\xbb\xbf\x05\xeb\xd0\xeb\x0c\x5e\x56\x31\x1e\xad\x01\xc3"
"\x85\xc0\x75\xf7\xc3\xe8\xef\xff\xff\xff\x43\x6f\x00\x97\x53";
*/

const char scode[] = 		"\x33\xc9\x83\xe9\xb0\xe8\xff\xff\xff\xff\xc0\x5e\x81\x76\x0e\x47"
		"\x13\x2b\xc0\x83\xee\xfc\xe2\xf4\xbb\x79\xc0\x8d\xaf\xea\xd4\x3f";


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


int main(void)
{
    

//	int i;
//
// 	instrtree((uint8_t *)scode, 16*7, 16*7 0); 

	unsigned int i;
	for ( i=0;i<sizeof(scode);i++ )
	{
		static char str[256];
		INSTRUCTION inst;

		int32_t isize = get_instruction(&inst, (uint8_t *)scode+i, MODE_32);

		if (isize <= 0)
			continue;

		get_instruction_string(&inst, FORMAT_INTEL, 0, str, sizeof(str));

		printf("         { rank = same; \"%02X\"; \"%08X %s\" };\n",i,i,str);
	}

	for ( i=0;i<sizeof(scode);i++ )
	{
		uint32_t off1;

		static char str1[256];
		INSTRUCTION inst1;

		uint32_t off2;
		static char str2[256];
		INSTRUCTION inst2;

		off1 = i;
		int32_t isize = get_instruction(&inst1, (uint8_t *)scode+i, MODE_32);

		if (isize <= 0)
			continue;

		off2=i+isize;

		isize = get_instruction(&inst2, (uint8_t *)scode+i+isize, MODE_32);
		if (isize <= 0)
			continue;

		

		get_instruction_string(&inst1, FORMAT_INTEL, 0, str1, sizeof(str1));
		get_instruction_string(&inst2, FORMAT_INTEL, 0, str2, sizeof(str2));

		printf("         \"%08X %s\" -> \"%08X %s\";\n",off1,str1,off2,str2);
	}



	return 0;
}

