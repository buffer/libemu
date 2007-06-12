/* @header@ */
#ifndef HAVE_EMU_H
#define HAVE_EMU_H

#include <stdio.h>
#include <stdint.h>

#define        MIN(a,b) (((a)<(b))?(a):(b))
#define        MAX(a,b) (((a)>(b))?(a):(b))


struct emu;
struct emu_logging;
struct emu_cpu;
struct emu_fpu;


struct emu *emu_new();
void emu_free(struct emu *e);

/* inline */ struct emu_memory *emu_memory_get(struct emu *e);
/* inline */ struct emu_logging *emu_logging_get(struct emu *e);
/* inline */ struct emu_cpu *emu_cpu_get(struct emu *e);
/* inline */ struct emu_fpu *emu_fpu_get(struct emu *e);

void emu_errno_set(struct emu *e, int err);
int emu_errno(struct emu *c);
void emu_strerror_set(struct emu *e, const char *format, ...);
const char *emu_strerror(struct emu *e);


/*int32_t emu_parse(struct emu *e);
int32_t emu_step(struct emu *e);*/
#endif // HAVE_EMU_H
