#ifndef HAVE_LOG_H
#define HAVE_LOG_H

struct emu;

enum emu_log_level
{
	EMU_LOG_NOT,
	EMU_LOG_INFO,
	EMU_LOG_DEBUG
};

struct emu_logging
{
	enum emu_log_level loglevel;
	void (*emu_log_fn)(struct emu *e, enum emu_log_level level, const char *msg);
};

void emu_log(struct emu *e, enum emu_log_level level, const char *format, ...);


#define logInfo(e,format,...) emu_log(e,EMU_LOG_INFO,format,__VA_ARGS__)
#define logDebug(e,format,...) emu_log(e,EMU_LOG_DEBUG,format,__VA_ARGS__)

#endif // HAVE_LOG_H
