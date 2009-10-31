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


#include "emu/emu.h"
#include "emu/emu_memory.h"
#include "emu/emu_cpu.h"
#include "emu/emu_cpu_data.h"
#include "emu/emu_getpc.h"
#include "emu/emu_cpu_instruction.h"


#define        MIN(a,b) (((a)<(b))?(a):(b))
#define        MAX(a,b) (((a)>(b))?(a):(b))


uint8_t emu_getpc_check(struct emu *e, uint8_t *data, uint32_t size, uint32_t offset)
{
	struct emu_cpu *c = emu_cpu_get(e);
	struct emu_memory *m = emu_memory_get(e);


//	uint32_t offset;
//	for (offset=0; offset<size;offset++)
//	{

	int reg;
	for (reg=0; reg<8; reg++)
	{
		emu_cpu_reg32_set(c, reg, 0);
	}

	emu_cpu_reg32_set(c, esp, 0x12000);


	switch (data[offset])
	{
	/* call */
	case 0xe8:
//		emu_memory_write_block(m, 0x1000, data+offset, MIN(size-offset, 6));
		emu_memory_write_block(m, 0x1000, data, size);
		emu_cpu_eip_set(c, 0x1000+offset);


		if ( emu_cpu_parse(c) != 0)
			break;
/*		
		printf("data %p size %x offset %x disp %x\n", data, size, offset, c->instr.cpu.disp);
		printf("data + offset + disp %p <-> %p data + size\n",  (data + offset + c->instr.cpu.disp), data+size);
		printf("data + offset + disp %p <-> %p data \n",  (data + offset + c->instr.cpu.disp), data);
*/
		
		if (abs(c->instr.cpu.disp) > 512)
		{
			break;
		}

		uint32_t espcopy = emu_cpu_reg32_get(c, esp);
		int j;
		for (j=0;j<64;j++)
		{
			int ret = emu_cpu_parse(emu_cpu_get(e));

			if (ret != -1)
			{
				ret = emu_cpu_step(emu_cpu_get(e));
			}

			if ( ret == -1 )
			{
//				printf("cpu error %s\n", emu_strerror(e));
				break;
			}

			if (emu_cpu_reg32_get(c, esp) == espcopy) // eip pushed by call is popped
				return 1;
		}

		return 1;
		break;

		/* fnstenv */
	case 0xd9:
		emu_memory_write_block(m, 0x1000, data+offset, MIN(size-offset, 64));
		emu_cpu_eip_set(c, 0x1000);

		if ( emu_cpu_parse(c) != 0 )
			break;

		if ( (c->instr.fpu.fpu_data[1] & 0x38) != 0x30 )
			break;

		if ( c->instr.fpu.ea == emu_cpu_reg32_get(c, esp) - 0xc )
		{
//			printf("found fnstenv with ea = esp - 0xc\n");
			return 1;
		}

		/* FIXME THE CODE HERE IS CRAP */
/*		espcopy = emu_cpu_reg32_get(c, esp);
		for (j=0;j<64;j++)
		{
			int ret = emu_cpu_parse(emu_cpu_get(e));

			if (ret != -1)
			{
				ret = emu_cpu_step(emu_cpu_get(e));
			}

			if ( ret == -1 )
			{
				printf("cpu error %s\n", emu_strerror(e));
				break;
			}

			for (reg=0; reg<8; reg++)
			{
				if (reg != 4 && emu_cpu_reg32_get(c, reg) == espcopy + 4) // eip written by fnstenv
					return 1;
			}
		}
*/
		break;
/*
	case 0x64: // fs: prefix
		if ( data[offset+1] == 0x8b )
		{
			emu_memory_write_block(m, 0x1000, data+offset, MIN(size-offset, 64));
			emu_cpu_eip_set(c, 0x1000);

			if ( emu_cpu_parse(c) != 0 )
				break;

			return 2;
		}
		break;
*/
	}
	return 0;
}

