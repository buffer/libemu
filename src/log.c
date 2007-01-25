#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#include <emu/emu.h>
#include <emu/log.h>

void emu_log(struct emu *e, enum emu_log_level level, const char *format, ...)
{
	if ( e->log.loglevel == EMU_LOG_NOT )
		return;

	if ( e->log.loglevel < level )
		return;


	va_list         ap;
	char            *message;

	va_start(ap, format);
	vasprintf(&message, format, ap);
	va_end(ap);

	if ( e->log.emu_log_fn == NULL )
	{
		fprintf(stderr,"[emu 0x%08x] ",(unsigned int)e);
		fprintf(stderr,"%s",message);
	} else
		e->log.emu_log_fn(e,level,message);

	free(message);
}
