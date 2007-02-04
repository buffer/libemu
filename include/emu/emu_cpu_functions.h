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

/* add */
int32_t instr_add_00(struct emu_cpu *c, struct instruction *i);
int32_t instr_add_01(struct emu_cpu *c, struct instruction *i);
int32_t instr_add_02(struct emu_cpu *c, struct instruction *i);
int32_t instr_add_03(struct emu_cpu *c, struct instruction *i);
int32_t instr_add_04(struct emu_cpu *c, struct instruction *i);
int32_t instr_add_05(struct emu_cpu *c, struct instruction *i);

#endif /*HAVE_EMU_CPU_FUNCTIONS_H*/
