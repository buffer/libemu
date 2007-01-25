#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include <emu/emu.h>
#include <emu/log.h>

struct emu_logging
{
	enum emu_log_level loglevel;
	void (*emu_log_fn)(struct emu *e, enum emu_log_level level, const char *msg);
};


struct emu_logging *log_new()
{
	struct emu_logging *el = (struct emu_logging *)malloc(sizeof(struct emu_logging));
	memset(el,0,sizeof(struct emu_logging));
	el->loglevel = EMU_LOG_DEBUG;
	return el;
}

void log_free(struct emu_logging *el)
{
	free(el);
}


void emu_log(struct emu *e, enum emu_log_level level, const char *format, ...)
{
	struct emu_logging *el = emu_logging_get(e);

	if ( el->loglevel == EMU_LOG_NOT )
		return;

	if ( el->loglevel < level )
		return;


	va_list         ap;
	char            *message;

	va_start(ap, format);
	vasprintf(&message, format, ap);
	va_end(ap);

	if ( el->emu_log_fn == NULL )
	{
		fprintf(stderr,"[emu 0x%08x] ",(unsigned int)e);
		fprintf(stderr,"%s",message);
	} else
		el->emu_log_fn(e,level,message);

	free(message);
}
