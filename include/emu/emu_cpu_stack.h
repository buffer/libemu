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

#ifndef EMU_CPU_STACK_H_
#define EMU_CPU_STACK_H_

#define PUSH_DWORD(cpu, arg)							\
{														\
	uint32_t pushme;									\
	bcopy(&(arg),  &pushme, 4);							\
	if (cpu->reg[esp] < 4)								\
	{													\
		emu_errno_set((cpu)->emu, ENOMEM);				\
		emu_strerror_set((cpu)->emu,					\
		"ran out of stack space writing a dword\n");	\
		return -1;										\
	}													\
	cpu->reg[esp]-=4;									\
	{																			\
		int32_t memret = emu_memory_write_dword(cpu->mem, cpu->reg[esp], pushme);	\
		if (memret != 0)														\
			return memret;														\
	}																			\
}


#define PUSH_WORD(cpu, arg)								\
{														\
	uint16_t pushme;									\
	bcopy(&(arg),  &pushme, 2);							\
	if (cpu->reg[esp] < 2)								\
	{													\
		emu_errno_set((cpu)->emu, ENOMEM);				\
		emu_strerror_set((cpu)->emu,					\
		"ran out of stack space writing a word\n");		\
		return -1;										\
	}													\
	cpu->reg[esp]-=2;									\
	{																			\
		int32_t memret = emu_memory_write_word(cpu->mem, cpu->reg[esp], pushme);\
		if (memret != 0)														\
			return memret;														\
	}																			\
}



#define PUSH_BYTE(cpu, arg)								\
{														\
	uint8_t pushme = arg;								\
	if (cpu->reg[esp] < 1)								\
	{													\
		emu_errno_set((cpu)->emu, ENOMEM);				\
		emu_strerror_set((cpu)->emu,					\
		"ran out of stack space writing a byte\n");		\
		return -1;										\
	}													\
	cpu->reg[esp]-=1;									\
	{																				\
		int32_t memret = emu_memory_write_byte(cpu->mem, cpu->reg[esp], pushme);	\
		if (memret != 0)															\
			return memret;															\
	}																				\
}


#define POP_DWORD(cpu, dst_p) \
{ int32_t ret = emu_memory_read_dword(cpu->mem, cpu->reg[esp], dst_p); \
if( ret != 0 ) \
	return ret; \
else \
  if ( dst_p != &cpu->reg[esp] ) \
  	cpu->reg[esp] += 4; }

#define POP_WORD(cpu, dst_p) \
{ int32_t ret = emu_memory_read_word(cpu->mem, cpu->reg[esp], dst_p); \
if( ret != 0 ) \
	return ret; \
else \
	cpu->reg[esp] += 2; }

#define POP_BYTE(cpu, dst_p) \
{ int32_t ret = emu_memory_read_byte(cpu->mem, cpu->reg[esp], dst_p); \
if( ret != 0 ) \
	return ret; \
else \
	cpu->reg[esp] += 1; }



#endif /*EMU_CPU_STACK_H_*/
