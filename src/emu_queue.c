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


#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "emu/emu_queue.h"

struct emu_queue *emu_queue_new(void)
{
	struct emu_queue *eq = (struct emu_queue *)malloc(sizeof(struct emu_queue));
	if( eq == NULL )
	{
		return NULL;
	}
	memset(eq, 0, sizeof(struct emu_queue));
	return eq;
}

void emu_queue_free(struct emu_queue *eq)
{
	while ( emu_queue_empty(eq) == false )
	{
		emu_queue_dequeue(eq);
	}
	free(eq);
}

void *emu_queue_front(struct emu_queue *eq)
{
	return eq->front->data;
}

void  emu_queue_enqueue(struct emu_queue *eq, void *data)
{
	struct emu_queue_item *eqi = emu_queue_item_new();
	eqi->data = data;

	if (emu_queue_empty(eq) == true)
	{
		eq->front = eqi;
		eq->back = eqi;
	}else
	{
		eq->back->next = eqi;
		eq->back = eqi;
	}
}

void *emu_queue_dequeue(struct emu_queue *eq)
{
	if (emu_queue_empty(eq) == true)
		return NULL;

	struct emu_queue_item *eqi = eq->front;

	// last element
	if ( eq->front == eq->back )
		eq->front = eq->back = NULL;
	else
		eq->front = eq->front->next;

	void *data = eqi->data;
	emu_queue_item_free(eqi);
	return data;
}


bool  emu_queue_empty(struct emu_queue *eq)
{
	if (eq->front == NULL)
		return true;
	else
		return false;
}


struct emu_queue_item *emu_queue_item_new(void)
{
	struct emu_queue_item *eqi = (struct emu_queue_item *)malloc(sizeof(struct emu_queue_item));
	if( eqi == NULL )
	{
		return NULL;
	}
	memset(eqi, 0, sizeof(struct emu_queue_item));
	return eqi;

}

void emu_queue_item_free(struct emu_queue_item *eqi)
{
	free(eqi);
}

