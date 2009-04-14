/********************************************************************************
 *                               libemu
 *
 *                    - x86 shellcode emulation -
 *
 *
 * Copyright (C) 2007  Paul Baecher & Markus Koetter
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 * 
 * 
 *             contact nepenthesdev@users.sourceforge.net  
 *
 *******************************************************************************/

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


struct emu *emu_new(void)
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
	int va = vasprintf(&message, format, ap);
	va_end(ap);

	if (va == -1)
		return;

	e->errorstr = message;
}

const char *emu_strerror(struct emu *e)
{
	return e->errorstr;
}

