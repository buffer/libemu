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
 
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include "emu/emu_breakpoint.h"
#include "emu/emu_memory.h"


struct emu_breakpoint
{
	struct emu_memory *mem;
	struct emu_breakpoint *next;
	uint32_t addr;
	uint8_t access;
	emu_bp_resp response;
	emu_bp_cond condition; 
};
 
 
struct emu_breakpoint *emu_breakpoint_alloc(struct emu_memory *mem)
{
	struct emu_breakpoint *bp = malloc(sizeof(struct emu_breakpoint));
	if(bp == NULL) {
		return NULL;
	}
	
	memset(bp, 0x00, sizeof(struct emu_breakpoint));
	
	bp->mem = mem;
	return bp;
}

void emu_breakpoint_free(struct emu_breakpoint *bp)
{
	/* TODO: Free all the bp_items */
	free(bp);
	
	return;
}
 
void emu_breakpoint_set(struct emu_memory *m, uint32_t addr, uint8_t access, emu_bp_resp response)
{
	emu_breakpoint_conditional_set(m, addr, access, response, NULL);
	
	return;
}

void emu_breakpoint_conditional_set(struct emu_memory *m, uint32_t addr, uint8_t access, emu_bp_resp response, emu_bp_cond condition)
{
	struct emu_breakpoint *item = emu_memory_get_breakpoint(m);
	struct emu_breakpoint *new_item = emu_breakpoint_alloc(m);
	if(new_item == NULL) {
		/* TODO: Something is wrong */
		return;
	}
	
	
	new_item->addr = addr;
	new_item->access = access;
	new_item->response = response;
	new_item->condition = condition;
	
	while(item->next != NULL) {
		item = item->next;
	}
	item->next = new_item;
	
	return;
}

struct emu_breakpoint *emu_breakpoint_get(struct emu_memory *m, uint32_t addr)
{
	struct emu_breakpoint *item = emu_memory_get_breakpoint(m);
		
	while(item != NULL) {
		if(item->addr == addr) {
			return item;
		}
		item = item->next;
	}
	return NULL;
}

void emu_breakpoint_check(struct emu_memory *m, uint32_t addr, uint8_t access)
{
	struct emu_breakpoint *bp = emu_breakpoint_get(m, addr);
 
 	if(bp == NULL) {
 		/* No Breakpoint at given addrs */
 		return;
	}
	
	if(bp->condition != NULL) {
		if(!(*bp->condition)(emu_memory_get_emu(m))) {
			return;
		}
	}
	
	if((bp->access & access) == access) {
		(*bp->response)(emu_memory_get_emu(m));
	}
	return;
}

void emu_breakpoint_remove(struct emu_memory *m, uint32_t addr)
{
	struct emu_breakpoint *item_current = emu_memory_get_breakpoint(m);
	struct emu_breakpoint *item_previous = NULL;
	
		
	while(item_current != NULL) {
		if(item_current->addr == addr) {
			if(item_previous == NULL) {
				item_previous = item_current;
				item_current = item_current->next;
				emu_breakpoint_free(item_previous);
			} else {
				item_previous->next = item_current->next;
				emu_breakpoint_free(item_current);
			}
			return;
		}
		item_previous = item_current;
		item_current = item_current->next;
	}
	return; 
}
