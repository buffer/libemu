/* @header@ */
#ifndef HAVE_EMU_CPU_H
#define HAVE_EMU_CPU_H

#include <inttypes.h>

struct emu;
struct emu_cpu;

enum emu_reg {
	eax = 0, ecx, edx, ebx, esp, ebp, esi, edi
};

struct emu_cpu *emu_cpu_new(struct emu *e);

inline uint32_t emu_cpu_reg32_get(struct emu_cpu *cpu_p, enum emu_reg reg);
inline void  emu_cpu_reg32_set(struct emu_cpu *cpu_p, enum emu_reg reg, uint32_t val);

inline uint16_t emu_cpu_reg16_get(struct emu_cpu *cpu_p, enum emu_reg reg);
inline void emu_cpu_reg16_set(struct emu_cpu *cpu_p, enum emu_reg reg, uint16_t val);

inline uint8_t emu_cpu_reg8_get(struct emu_cpu *cpu_p, enum emu_reg reg);
inline void emu_cpu_reg8_set(struct emu_cpu *cpu_p, enum emu_reg reg, uint8_t val);


void emu_cpu_eip_set(struct emu_cpu *c, uint32_t eip);
uint32_t emu_cpu_eip_get(struct emu_cpu *c);

int32_t emu_cpu_step(struct emu_cpu *c);
int32_t emu_cpu_run(struct emu_cpu *c);

void emu_cpu_free(struct emu_cpu *c);



#endif /* HAVEEMU_CPU_H */
