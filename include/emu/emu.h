#ifndef HAVE_EMU_H
#define HAVE_EMU_H


struct emu;
struct emu_logging;



struct emu *emu_new();
void emu_free(struct emu *e);

inline struct emu_memory *emu_memory_get(struct emu *e);
inline struct emu_logging *emu_logging_get(struct emu *e);

#endif // HAVE_EMU_H
