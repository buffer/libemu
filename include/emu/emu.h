#ifndef HAVE_EMU_H
#define HAVE_EMU_H


struct emu;
struct emu_logging;



struct emu *emu_new();
void emu_free(struct emu *e);

struct emu_logging* emu_logging_get(struct emu *e);

#endif // HAVE_EMU_H
