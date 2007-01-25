#include <string.h>
#include <stdlib.h>

#include <emu/emu.h>
#include <emu/log.h>
#include <emu/emu_memory.h>

struct emu_memory
{
	int i;
};

struct emu_memory *emu_memory_new()
{
	struct emu_memory *em = (struct emu_memory *)malloc(sizeof(struct emu_memory));
	memset(em,0,sizeof(struct emu_memory));
	return em;
}

void emu_memory_free(struct emu_memory *em)
{
	free(em);
}

uint32_t emu_memory_read_dword(struct emu *e, struct emu_memory *em, void *p)
{
	return 0;
}

void emu_memory_write_dword(struct emu *e, struct emu_memory *em, void *p, uint32_t word)
{

}

