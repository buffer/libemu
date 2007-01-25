#ifndef HAVE_EMU_MEMORY_H
#define HAVE_EMU_MEMORY_H

#include <inttypes.h>

struct emu_memory;

struct emu_memory *emu_memory_new();
uint32_t emu_memory_read_word(struct emu_memory *e, void *p);
void emu_memory_write_word(struct emu_memory *e, void *p, uint32_t word);
void emu_memory_free(struct emu_memory *e);

#endif // HAVE_EMU_MEMORY_H
