/********************************************************************************
 *                               libemu
 *
 *                    - x86 shellcode emulation -
 *
 *
 * Copyright (C) 2008  Paul Baecher & Markus Koetter
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


#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "emu/emu_stack.h"

struct emu_stack *emu_stack_new(void)
{
	struct emu_stack *es = malloc(sizeof(struct emu_stack));
	memset(es, 0, sizeof(struct emu_stack));
	return es;
}

void emu_stack_free(struct emu_stack *es)
{
	free(es);
}

void *emu_stack_front(struct emu_stack *es)
{
	return es->front->data;
}

void  emu_stack_push(struct emu_stack *es, void *data)
{
	struct emu_stack_item *item = malloc(sizeof(struct emu_stack_item));
	memset(item, 0, sizeof(struct emu_stack_item));
	item->data = data;
	item->next = es->front;
	es->front = item;
}

void *emu_stack_pop(struct emu_stack *es)
{
	if (emu_stack_empty(es) == true)
		return NULL;

	struct emu_stack_item *item = es->front;
	void *data = es->front->data;
	es->front = es->front->next;
	free(item);
	return data;
}

bool  emu_stack_empty(struct emu_stack *es)
{
	if (es->front == NULL)
		return true;
	return false;
}

