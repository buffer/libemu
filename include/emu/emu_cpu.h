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

#ifndef HAVE_EMU_CPU_H
#define HAVE_EMU_CPU_H

#include <inttypes.h>

struct emu;
struct emu_cpu;

enum emu_reg32 {
	eax = 0, ecx, edx, ebx, esp, ebp, esi, edi
};


enum emu_reg16
{
	ax =  0,/* eax */
	cx, 	/* ecx */
	dx, 	/* edx */
	bx, 	/* ebx */
	sp, 	/* esp */
	bp, 	/* ebp */
	si, 	/* esp */
	di  	/* edi */

};


enum emu_reg8
{
	al=0,  	/* eax */
	cl,    	/* ecx */
	dl,    	/* edx */
	bl,    	/* ebx */
	ah,     /* eax */
	ch,     /* ecx */
	dh,     /* edx */
	bh      /* ebx */

};

struct emu_cpu *emu_cpu_new(struct emu *e);

uint32_t emu_cpu_reg32_get(struct emu_cpu *cpu_p, enum emu_reg32 reg);
void  emu_cpu_reg32_set(struct emu_cpu *cpu_p, enum emu_reg32 reg, uint32_t val);

uint16_t emu_cpu_reg16_get(struct emu_cpu *cpu_p, enum emu_reg16 reg);
void emu_cpu_reg16_set(struct emu_cpu *cpu_p, enum emu_reg16 reg, uint16_t val);

uint8_t emu_cpu_reg8_get(struct emu_cpu *cpu_p, enum emu_reg8 reg);
void emu_cpu_reg8_set(struct emu_cpu *cpu_p, enum emu_reg8 reg, uint8_t val);

uint32_t emu_cpu_eflags_get(struct emu_cpu *c);
void emu_cpu_eflags_set(struct emu_cpu *c, uint32_t val);

/**
 * Set the cpu's EIP
 * 
 * @param c      the cpu
 * @param eip    eip
 */
void emu_cpu_eip_set(struct emu_cpu *c, uint32_t eip);

/**
 * get the cpu's EIP
 * 
 * @param c      the cpu
 * 
 * @return EIP
 */
uint32_t emu_cpu_eip_get(struct emu_cpu *c);


/**
 * parse a instruction at EIP
 * 
 * @param c      the cpu
 * 
 * @return on success: 0
 *         on errror : -1, check emu_errno and emu_strerror
 */
int32_t emu_cpu_parse(struct emu_cpu *c);

/**
 * step the last instruction
 * 
 * @param c      the cpu
 * 
 * @return on success: 0
 *         on errror : -1, check emu_errno and emu_strerror
 */
int32_t emu_cpu_step(struct emu_cpu *c);

int32_t emu_cpu_run(struct emu_cpu *c);

void emu_cpu_free(struct emu_cpu *c);

void emu_cpu_debug_print(struct emu_cpu *c);

void emu_cpu_debugflag_set(struct emu_cpu *c, uint8_t flag);
void emu_cpu_debugflag_unset(struct emu_cpu *c, uint8_t flag);


#endif /* HAVEEMU_CPU_H */
