#include <string.h>
#include <stdlib.h>

#include <emu/emu.h>
#include <emu/log.h>
#include <emu/emu_memory.h>


struct emu
{
	struct emu_logging *log;
	struct emu_memory *memory; 
};


struct emu *emu_new()
{
	struct emu *e = (struct emu *)malloc(sizeof(struct emu));
	memset(e,0,sizeof(struct emu));
	e->log = log_new();
	e->memory = emu_memory_new();
	logDebug(e,"%s %x\n",__PRETTY_FUNCTION__,(unsigned int)e);
	return e;
}


void emu_free(struct emu *e)
{
	logDebug(e,"%s %x\n",__PRETTY_FUNCTION__,(unsigned int)e);
	log_free(e->log);
	free(e);
}

struct emu_logging* emu_logging_get(struct emu *e)
{
	return e->log;
}
