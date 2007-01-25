#include <stdio.h>
#include <stdlib.h>

#include <emu/emu.h>
#include <emu/emu_memory.h>
#include <emu/log.h>

int main()
{
	struct emu *e = emu_new();
	emu_log_level_set(emu_logging_get(e),EMU_LOG_DEBUG);


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
