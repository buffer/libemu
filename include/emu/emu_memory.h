#ifndef HAVE_EMU_MEMORY_H
#define HAVE_EMU_MEMORY_H

#include <inttypes.h>

struct emu;
struct emu_memory;

struct emu_memory *emu_memory_new();
void emu_memory_free(struct emu_memory *em);

uint32_t emu_memory_read_dword(struct emu *e, struct emu_memory *em, void *p, uint32_t *word);
void emu_memory_write_dword(struct emu *e, struct emu_memory *em, void *p, uint32_t dword);


#endif // HAVE_EMU_MEMORY_H
