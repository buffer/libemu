/* @header@ */
#ifndef HAVE_EMU_MEMORY_H
#define HAVE_EMU_MEMORY_H

#include <inttypes.h>
#include <sys/types.h>

enum emu_segment {
	s_cs = 0, s_ss, s_ds, s_es, s_fs, s_gs
};

struct emu;
struct emu_memory;

struct emu_memory *emu_memory_new(struct emu *e);
void emu_memory_free(struct emu_memory *em);

/* read access, these functions return -1 on error  */
int32_t emu_memory_read_byte(struct emu_memory *m, uint32_t addr, uint8_t *byte);
int32_t emu_memory_read_word(struct emu_memory *m, uint32_t addr, uint16_t *word);
int32_t emu_memory_read_dword(struct emu_memory *m, uint32_t addr, uint32_t *dword);
int32_t emu_memory_read_block(struct emu_memory *m, uint32_t addr, void *dest, size_t len);

/* write access */
int32_t emu_memory_write_byte(struct emu_memory *m, uint32_t addr, uint8_t byte);
int32_t emu_memory_write_word(struct emu_memory *m, uint32_t addr, uint16_t word);
int32_t emu_memory_write_dword(struct emu_memory *m, uint32_t addr, uint32_t dword);
int32_t emu_memory_write_block(struct emu_memory *m, uint32_t addr, void *src, size_t len);

/* segment selection */
void emu_memory_segment_select(struct emu_memory *m, enum emu_segment s);
enum emu_segment emu_memory_segment_get(struct emu_memory *m);

#endif // HAVE_EMU_MEMORY_H
