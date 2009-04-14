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
#include <stdarg.h>


#include "emu/emu_string.h"


struct emu_string *emu_string_new(void)
{
    struct emu_string *s = (struct emu_string *)malloc(sizeof(struct emu_string));
    if( s == NULL )
    {
    	return NULL;
    }
    memset(s, 0, sizeof(struct emu_string));
    return s;
}

void emu_string_free(struct emu_string *s)
{
    free(s->data);
    free(s);
}

char *emu_string_char(struct emu_string *s)
{
    return (char *)s->data;
}


#include <stdio.h>
void emu_string_append_char(struct emu_string *s, const char *data)
{
//	printf("before %i %i|%s|\n", s->size, strlen(data), (char *)s->data);
	s->data = realloc(s->data, s->size + strlen(data) + 1);
	memcpy((unsigned char *)s->data + s->size, data, strlen(data));
	*(unsigned char *)(s->data + s->size + strlen(data)) = 0;
	s->size += strlen(data);
//	printf("after %i |%s|\n", s->size, (char *)s->data);
}

void emu_string_append_format(struct emu_string *s, const char *format, ...)
{
	va_list         ap;
	char            *message;

	va_start(ap, format);
	int va = vasprintf(&message, format, ap);
	va_end(ap);

	if (va == -1)
		exit(-1);

	emu_string_append_char(s, message);
	free(message);
}
