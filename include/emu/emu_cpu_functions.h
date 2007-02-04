/* @header@ */
#ifndef HAVE_EMU_CPU_FUNCTIONS_H
#define HAVE_EMU_CPU_FUNCTIONS_H

#define MEM_BYTE_READ(cpu_p, addr, data_p) \
 { uint32_t ret = emu_memory_read_byte((cpu_p)->mem, addr, data_p); \
 if( ret != 0 ) \
  return ret; }

#define MEM_BYTE_WRITE(cpu_p, addr, data) \
 { uint32_t ret = emu_memory_write_byte((cpu_p)->mem, addr, data); \
 if( ret != 0 ) \
  return ret; }

#define MEM_WORD_READ(cpu_p, addr, data_p) \
 { uint32_t ret = emu_memory_read_word((cpu_p)->mem, addr, data_p); \
 if( ret != 0 ) \
  return ret; }

#define MEM_WORD_WRITE(cpu_p, addr, data) \
 { uint32_t ret = emu_memory_write_word((cpu_p)->mem, addr, data); \
 if( ret != 0 ) \
  return ret; }

#define MEM_DWORD_READ(cpu_p, addr, data_p) \
 { uint32_t ret = emu_memory_read_dword((cpu_p)->mem, addr, data_p); \
 if( ret != 0 ) \
  return ret; }

#define MEM_DWORD_WRITE(cpu_p, addr, data) \
 { uint32_t ret = emu_memory_write_dword((cpu_p)->mem, addr, data); \
 if( ret != 0 ) \
  return ret; }

int32_t prefix_fn(struct emu_cpu *c, struct instruction *i)
{
	/* dummy */
	return 0;
}

/* ADD (00, 01, 02, 03, 04, 05 */
#include "emu/functions/add.h"

#endif /*HAVE_EMU_CPU_FUNCTIONS_H*/
