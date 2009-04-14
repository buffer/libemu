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


#ifndef HAVE_EMU_STACK_H
#define HAVE_EMU_STACK_H

struct emu_stack_item
{
	struct emu_stack_item *next;
	void *data;
};

struct emu_stack
{
	struct emu_stack_item *front;
	struct emu_stack_item *back;
};

struct emu_stack *emu_stack_new(void);
void emu_stack_free(struct emu_stack *es);
void *emu_stack_front(struct emu_stack *es);
void  emu_stack_push(struct emu_stack *es, void *data);
void *emu_stack_pop(struct emu_stack *es);
bool  emu_stack_empty(struct emu_stack *es);

#endif
