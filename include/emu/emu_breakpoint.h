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
 
#ifndef HAVE_EMU_BREAKPOINT_H
#define HAVE_EMU_BREAKPOINT_H

#include <inttypes.h>
#include <stdbool.h>

struct emu;
struct emu_memory;
struct emu_breakpoint;

#define EMU_ACCESS_READ	(4)
#define EMU_ACCESS_WRITE   (2)
/* NOTYET SUPPORTED */
#define EMU_ACCESS_EXECUTE (1) 

/* Argument Function Pointers */
typedef void (*emu_bp_resp)(struct emu *e);
typedef bool (*emu_bp_cond)(struct emu *e);

/* Memory Management */
struct emu_breakpoint *emu_breakpoint_alloc(struct emu_memory *mem);
void emu_breakpoint_free(struct emu_breakpoint *bp);

/* Set / Get */
void emu_breakpoint_set(struct emu_memory *m, uint32_t addr, uint8_t access, emu_bp_resp response);
void emu_breakpoint_conditional_set(struct emu_memory *m, uint32_t addr, uint8_t access, emu_bp_resp response, emu_bp_cond condition);
struct emu_breakpoint *emu_breakpoint_get(struct emu_memory *m, uint32_t addr);
void emu_breakpoint_check(struct emu_memory *m, uint32_t addr, uint8_t access);

void emu_breakpoint_remove(struct emu_memory *m, uint32_t addr);

#endif /* HAVE_EMU_BREAKPOINT_H */
