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

#ifndef HAVE_EMU_H
#define HAVE_EMU_H

#include <stdio.h>
#include <stdint.h>

#ifndef MIN
	#define        MIN(a,b) (((a)<(b))?(a):(b))
#endif

#ifndef MAX
	#define        MAX(a,b) (((a)>(b))?(a):(b))
#endif


struct emu;
struct emu_logging;
struct emu_cpu;
struct emu_fpu;


/**
 * Create a new emu.
 * 
 * @return on success: the new emu
 *         on failure: NULL
 */
struct emu *emu_new(void);

/**
 * Free the emu
 * 
 * @param e      the emu to free
 */
void emu_free(struct emu *e);

/**
 * Retrieve a pointer to the emu's emu_memory.
 * 
 * @param e      the emu
 * 
 * @return The pointer to the emu's emu_memory.
 */
struct emu_memory *emu_memory_get(struct emu *e);

/**
 * Retrieve a pointer to the emu's logging facility.
 * 
 * @param e      the emu
 * 
 * @return pointer to the emu's emu_logging.
 */
struct emu_logging *emu_logging_get(struct emu *e);

/**
 * Retrieve a pointer to the emu's emu_cpu
 * 
 * @param e      the emu
 * 
 * @return pointer to the emu's emu_cpu.
 */
struct emu_cpu *emu_cpu_get(struct emu *e);

/**
 * Set the emu's internal errno
 * 
 * @param e      the emu
 * @param err
 */
void emu_errno_set(struct emu *e, int err);

/**
 * Retrieve the emu's errno
 * 
 * @param c      the emu
 * 
 * @return the emu's errno
 */
int emu_errno(struct emu *c);

/**
 * Set the emu's strerror message.
 * 
 * @param e      the emu
 * @param format the errormessage format
 */
void emu_strerror_set(struct emu *e, const char *format, ...);

/**
 * Retrieve the emu's strerror
 * 
 * @param e      the emu
 * 
 * @return the strerror
 */
const char *emu_strerror(struct emu *e);


/*int32_t emu_parse(struct emu *e);
int32_t emu_step(struct emu *e);*/
#endif // HAVE_EMU_H
