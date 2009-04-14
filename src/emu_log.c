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

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "emu/emu.h"
#include "emu/emu_log.h"



struct emu_logging
{
	enum emu_log_level loglevel;

	emu_log_logcb logcb;
};


struct emu_logging *emu_log_new(void)
{
	struct emu_logging *el = (struct emu_logging *)malloc(sizeof(struct emu_logging));
	if( el == NULL )
	{
		return NULL;
	}
	memset(el, 0, sizeof(struct emu_logging));

	el->logcb = emu_log_default_logcb;

	return el;
}

void emu_log_free(struct emu_logging *el)
{
	free(el);
}

void emu_log_level_set(struct emu_logging *el, enum emu_log_level level)
{
	el->loglevel = level;
}

void emu_log(struct emu *e, enum emu_log_level level, const char *format, ...)
{
	struct emu_logging *el = emu_logging_get(e);

	if ( el->loglevel == EMU_LOG_NONE )
		return;

	if ( el->loglevel < level )
		return;


	va_list         ap;
	char            *message;

	va_start(ap, format);
	int va = vasprintf(&message, format, ap);
	va_end(ap);

	if (va == -1)
		message = strdup("failed to allocate memory in vasprintf\n");

	el->logcb(e, level, message);

	free(message);
}

void emu_log_set_logcb(struct emu_logging *el, emu_log_logcb logcb)
{
	el->logcb = logcb;
}

void emu_log_default_logcb(struct emu *e, enum emu_log_level level, const char *msg)
{
	const char *lev[] = {"none","\033[32;1minfo\033[0m","\033[31;1mdebug\033[0m"};
	fprintf(stdout,"[emu 0x%p %s ] ",(void *)e, lev[level]);
	fprintf(stdout,"%s", msg);
}
