#include <string.h>
#include <stdlib.h>

#include <emu/emu.h>
#include <emu/log.h>


struct emu *emu_new()
{
	struct emu *e = (struct emu *)malloc(sizeof(struct emu));
	memset(e,0,sizeof(struct emu));
	e->log.loglevel = EMU_LOG_DEBUG;
	logDebug(e,"%s %x\n",__PRETTY_FUNCTION__,(unsigned int)e);
	return e;
}


void emu_free(struct emu *e)
{
	logDebug(e,"%s %x\n",__PRETTY_FUNCTION__,(unsigned int)e);
	free(e);
}
