/* @header@ */
#ifndef HAVE_EMU_LOG_H
#define HAVE_EMU_LOG_H

struct emu;

enum emu_log_level
{
	EMU_LOG_NONE,
	EMU_LOG_INFO,
	EMU_LOG_DEBUG
};


struct emu_logging *emu_log_new();
void emu_log_free(struct emu_logging *el);

void emu_log_level_set(struct emu_logging *el, enum emu_log_level level);

void emu_log(struct emu *e, enum emu_log_level level, const char *format, ...);


#define logInfo(e, format...) emu_log(e, EMU_LOG_INFO, format)
#define logDebug(e, format...) emu_log(e, EMU_LOG_DEBUG, format)

#endif // HAVE_EMU_LOG_H
