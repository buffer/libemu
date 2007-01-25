#ifndef HAVE_EMU_H
#define HAVE_EMU_H

#include <emu/log.h>

struct emu
{
	int i;

	struct emu_logging log;
	struct emu_memory *memory; 
};


struct emu *emu_new();
void emu_free(struct emu *e);

#endif // HAVE_EMU_H
