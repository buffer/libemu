/* @header@ */

#ifndef HAVE_EMU_SHELLCODE_H
#define HAVE_EMU_SHELLCODE_H

#include <stdint.h>

struct emu;

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




struct emu_stats *emu_stats_new();
void emu_stats_free(struct emu_stats *es);


#endif

