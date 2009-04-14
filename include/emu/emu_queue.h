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


#ifndef HAVE_EMU_QUEUE_H
#define HAVE_EMU_QUEUE_H


struct emu_queue_item
{
	struct emu_queue_item *next;
	void *data;
};

struct emu_queue_item *emu_queue_item_new(void);
void emu_queue_item_free(struct emu_queue_item *eqi);

struct emu_queue
{
	struct emu_queue_item *front;
	struct emu_queue_item *back;
};

/**
 * Create a new queue
 * 
 * @return on success: pointer to the new queue
 *         on failure: NULL
 */
struct emu_queue *emu_queue_new(void);
void emu_queue_free(struct emu_queue *eq);

/**
 * Retrieve the pointer to the first element
 * 
 * @param eq     the queue
 * 
 * @return returns the pointer to the first element
 */
void *emu_queue_front(struct emu_queue *eq);

/**
 * Enqueue data into the queue.
 * 
 * @param eq     the queue
 * @param data   the data to enqueue
 */
void  emu_queue_enqueue(struct emu_queue *eq, void *data);

/**
 * Dequeue the first element from the queue
 * 
 * @param eq     the queue
 * 
 * @return pointer to the data of the first element
 */
void *emu_queue_dequeue(struct emu_queue *eq);

/**
 * Check if the queue is empty
 * 
 * @param eq     the queue
 * 
 * @return true if the queue is empty, else false
 */
bool  emu_queue_empty(struct emu_queue *eq);

#endif
