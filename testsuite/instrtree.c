/* @header@ */


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
