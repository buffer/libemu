/* @header@ */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include "emu/emu.h"
#include "emu/emu_log.h"
#include "emu/emu_memory.h"
#include "emu/emu_cpu.h"

struct emu
{
	struct emu_logging *log;
	struct emu_memory *memory; 
	struct emu_cpu *cpu;

	int 	errno;
	char 	*errorstr;
};


struct emu *emu_new()
{
	struct emu *e = (struct emu *)malloc(sizeof(struct emu));
	if( e == NULL )
	{
		return NULL;
	}
	memset(e, 0, sizeof(struct emu));
	e->log = emu_log_new();
	e->memory = emu_memory_new(e);
	if( e->memory == NULL )
	{
		return NULL;
	}
	e->cpu = emu_cpu_new(e);
	logDebug(e,"%s %x\n", __PRETTY_FUNCTION__,(unsigned int)e);
	return e;
}


void emu_free(struct emu *e)
{
	logDebug(e,"%s %x\n", __PRETTY_FUNCTION__,(unsigned int)e);
	emu_cpu_free(e->cpu);
	emu_memory_free(e->memory);
	emu_log_free(e->log);
	if (e->errorstr != NULL)
		free(e->errorstr);

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

inline struct emu_cpu *emu_cpu_get(struct emu *e)
{
	return e->cpu;
}



void emu_errno_set(struct emu *e, int err)
{
	e->errno = err;
}

int emu_errno(struct emu *c)
{
	return c->errno;
}

void emu_strerror_set(struct emu *e, const char *format, ...)
{
	if (e->errorstr != NULL)
    	free(e->errorstr);

	va_list         ap;
	char            *message;
	va_start(ap, format);
	vasprintf(&message, format, ap);
	va_end(ap);

	e->errorstr = message;
}

const char *emu_strerror(struct emu *e)
{
	return e->errorstr;
}




/*int32_t emu_parse(struct emu *e)
{
	return emu_cpu_parse(e);
	struct emu_memory *m = e->memory;
	struct emu_cpu *c = e->cpu;
	struct emu_fpu *f = e->fpu;

	uint8_t instrbyte;
	int32_t ret =0;

	if( (ret = emu_memory_read_byte(m, emu_cpu_eip_get(c), &instrbyte)) != 0)
		return ret;



	if (instrbyte >= 0xd8 && instrbyte <= 0xdf)
	{ // fpu instr
		if ((ret = emu_fpu_parse(f)) != 0 )
        	return ret;
		else
			return 1; // fpu instruction parsed
	}


	if ( (ret = emu_cpu_parse(c)) != 0 )
		return ret;
	else
        return 0; // cpu instruction parsed
}*/

/*int32_t emu_step(struct emu *e)
{
	return emu_cpu_step(emu_cpu_get(e));
	int32_t ret = emu_parse(e);
	if (ret == 0)
	{
		return emu_cpu_step(emu_cpu_get(e));
	}
	else
	if ( ret == 1 )
	{
		return emu_fpu_step(emu_fpu_get(e));
	}
	else
	{
		printf("cpu error %s\n", emu_strerror(e));
		return -1;
	}
}*/
