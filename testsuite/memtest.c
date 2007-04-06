#include <stdio.h>
#include "emu/emu.h"
#include "emu/emu_memory.h"

void test_alloc(struct emu *e)
{
	const uint32_t len = 4711;
	uint32_t addr;
	struct emu_memory *m = emu_memory_get(e);
	
	emu_memory_alloc(m, &addr, len);
	
	printf("allocd at 0x%08x\n", addr);
	
	int i;
	
	for( i = 0; i < len; i++ )
	{
		uint8_t byte;
		if( emu_memory_read_byte(m, addr + i, &byte) )
		{
			printf("error reading allocated byte: %s\n", emu_strerror(e));
		}
	}
}


int main(int argc, char **argv)
{
	struct emu *e;
	
	e = emu_new();
	
	test_alloc(e);
	
	emu_free(e);
	
	return 0;
}
