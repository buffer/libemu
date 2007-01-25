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
	e->log = emu_log_new();
	e->memory = emu_memory_new(e);
	logDebug(e,"%s %x\n",__PRETTY_FUNCTION__,(unsigned int)e);
	return e;
}


void emu_free(struct emu *e)
{
	logDebug(e,"%s %x\n",__PRETTY_FUNCTION__,(unsigned int)e);
	emu_log_free(e->log);
	free(e);
}

inline struct emu_memory *emu_memory_get(struct emu *e)
{
	return e->memory;
}

inline struct emu_logging *emu_logging_get(struct emu *e)
{
	return e->log;
}
