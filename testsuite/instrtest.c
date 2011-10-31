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


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/select.h>
#include <unistd.h>
#include <stdio.h>

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <getopt.h>


#include "emu/emu.h"
#include "emu/emu_memory.h"
#include "emu/emu_cpu.h"
#include "emu/emu_log.h"
#include "emu/emu_cpu_data.h"

#define CODE_OFFSET 0x417001

#define FAILED "\033[31;1mfailed\033[0m"
#define SUCCESS "\033[32;1msuccess\033[0m"

#define F(x) (1 << (x))


static struct run_time_options
{
	int verbose;
	int nasm_force;
	uint32_t steps;
	int testnumber;
} opts;

static const char *regm[] = {
	"eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi"
};


	                         /* 0     1     2     3      4       5       6     7 */
static const char *flags[] = { "CF", "  ", "PF", "  " , "AF"  , "    ", "ZF", "SF", 
	                           "TF", "IF", "DF", "OF" , "IOPL", "IOPL", "NT", "  ",
	                           "RF", "VM", "AC", "VIF", "RIP" , "ID"  , "  ", "  ",
	                           "  ", "  ", "  ", "   ", "    ", "    ", "  ", "  "};


struct instr_test
{
	const char *instr;

	char  *code;
	uint16_t codesize;

	struct 
	{
		uint32_t reg[8];
		uint32_t		mem_state[2];
		uint32_t	eflags;
	} in_state;

	struct 
	{
		uint32_t reg[8];
		uint32_t		mem_state[2];
		uint32_t	eflags;
		uint32_t eip;
	} out_state;
};

#define FLAG(fl) (1 << (fl))

struct instr_test tests[] = 
{

/*  {
        .instr = "instr",
        .in_state.reg  = {0,0,0,0,0,0,0,0 },
        .in_state.mem_state = {0, 0},
        .out_state.reg  = {0,0,0,0,0,0,0,0 },
        .out_state.mem_state = {0, 0},
    },*/

	/*{
		.instr = "",
		.code =  
		.codesize = 344,
		.in_state.reg  = {0,0xfffffe6c,0,0,0x12fe98,0x12ff74,0x12fe9c,0x12ff74}, // ollydbg
		.in_state.mem_state = {0, 0},
	},*/


	/* 00 */
	{
		.instr = "add ah,al",
		.code = "\x00\xc4",
		.codesize = 2,
		.in_state.reg  = {0xff01,0,0,0,0,0,0,0},
		.in_state.mem_state = {0, 0},
		.out_state.reg  = {0x01,0,0,0,0,0,0,0},
		.out_state.mem_state = {0, 0},
		.out_state.eflags = FLAG(f_cf) | FLAG(f_pf) | FLAG(f_zf),
	},
	{
		.instr = "add ch,dl",
		.code = "\x00\xd5",
		.codesize = 2,
		.in_state.reg  = {0,0x1000,0x20,0,0,0,0,0},
		.in_state.mem_state = {0, 0},
		.out_state.reg  = {0,0x3000,0x20,0,0,0,0,0},
		.out_state.mem_state = {0, 0},
		.out_state.eflags =  FLAG(f_pf),
	},
	{
		.instr = "add [ecx],al",
		.code = "\x00\x01",
		.codesize = 2,
		.in_state.reg  = {0x10,0x40000,0,0,0,0,0,0},
		.in_state.mem_state = {0x40000, 0x10101010},
		.out_state.reg  = {0x10,0x40000,0,0,0,0,0,0},
		.out_state.mem_state = {0x40000, 0x10101020},
	},
	/* 01 */
	{
		.instr = "add ax,cx",
		.code = "\x66\x01\xc8",
		.codesize = 3,
		.in_state.reg  = {0xffff1111,0xffff2222,0,0,0,0,0,0},
		.in_state.mem_state = {0, 0},
		.out_state.reg  = {0xffff3333,0xffff2222,0,0,0,0,0,0},
		.out_state.mem_state = {0, 0},
		.out_state.eflags =  FLAG(f_pf), 
	},
	{
		.instr = "add [ecx],ax",
		.code = "\x66\x01\x01",
		.codesize = 3,
		.in_state.reg  = {0xffff1111,0x40000,0,0,0,0,0,0},
		.in_state.mem_state = {0x40000, 0x22224444},
		.out_state.reg  = {0xffff1111,0x40000,0,0,0,0,0,0},
		.out_state.mem_state = {0x40000, 0x22225555},
		.out_state.eflags =  FLAG(f_pf), 
	},
	{
		.instr = "add eax,ecx",
		.code = "\x01\xc8",
		.codesize = 2,
		.in_state.reg  = {0x11112222,0x22221111,0,0,0,0,0,0},
		.in_state.mem_state = {0, 0},
		.out_state.reg  = {0x33333333,0x22221111,0,0,0,0,0,0},
		.out_state.mem_state = {0, 0},
		.out_state.eflags =  FLAG(f_pf), 
	},
	{
		.instr = "add [ecx],eax",
		.code = "\x01\x01",
		.codesize = 2,
		.in_state.reg  = {0x22221111,0x40000,0,0,0,0,0,0},
		.in_state.mem_state = {0x40000, 0x22224444},
		.out_state.reg  = {0x22221111,0x40000,0,0,0,0,0,0},
		.out_state.mem_state = {0x40000, 0x44445555},
		.out_state.eflags =  FLAG(f_pf), 
	},
	/* 02 */
	{
		.instr = "add cl,bh",
		.code = "\x02\xcf",	/* add cl,bh */
		.codesize = 2,
		.in_state.reg  = {0,0xff,0,0x100,0,0,0,0},
		.in_state.mem_state = {0, 0},
		.out_state.reg  = {0,0,0,0x100,0,0,0,0},
		.out_state.mem_state = {0, 0},
		.out_state.eflags =  FLAG(f_cf) | FLAG(f_pf) | FLAG(f_zf), 
	},
	{
		.instr = "add al,[ecx]",
		.code = "\x02\x01",
		.codesize = 2,
		.in_state.reg  = {0x3,0x40000,0,0,0,0,0,0},
		.in_state.mem_state = {0x40000, 0x30303030},
		.out_state.reg  = {0x33,0x40000,0,0,0,0,0,0},
		.out_state.mem_state = {0x40000, 0x30303030},
		.out_state.eflags =  FLAG(f_pf),
	},
	/* 03 */
	{
		.instr = "add cx,di",
		.code = "\x66\x03\xcf",	/* add cx,di */
		.codesize = 3,
		.in_state.reg  = {0,0x10101010,0,0,0,0,0,0x02020202},
		.in_state.mem_state = {0, 0},
		.out_state.reg  = {0,0x10101212,0,0,0,0,0,0x02020202},
		.out_state.mem_state = {0, 0},
		.out_state.eflags =  FLAG(f_pf),
	},
	{
		.instr = "add ax,[ecx]",
		.code = "\x66\x03\x01",
		.codesize = 3,
		.in_state.reg  = {0x11112222,0x40000,0,0,0,0,0,0},
		.in_state.mem_state = {0x40000, 0x44443333},
		.out_state.reg  = {0x11115555,0x40000,0,0,0,0,0,0},
		.out_state.mem_state = {0x40000, 0x44443333},
		.out_state.eflags =  FLAG(f_pf),
	},
	{
		.instr = "add ecx,edi",
		.code = "\x03\xcf",	/* add ecx,edi */
		.codesize = 2,
		.in_state.reg  = {0,0x10101010,0,0,0,0,0,0x02020202},
		.in_state.mem_state = {0, 0},
		.out_state.reg  = {0,0x12121212,0,0,0,0,0,0x02020202},
		.out_state.mem_state = {0, 0},
		.out_state.eflags =  FLAG(f_pf),
	},
	{
		.instr = "add eax,[ecx]",
		.code = "\x03\x01",
		.codesize = 2,
		.in_state.reg  = {0x11112222,0x40000,0,0,0,0,0,0},
		.in_state.mem_state = {0x40000, 0x44443333},
		.out_state.reg  = {0x55555555,0x40000,0,0,0,0,0,0},
		.out_state.mem_state = {0x40000, 0x44443333},
		.out_state.eflags =  FLAG(f_pf),
	},
	{
		.instr = "add ecx,[ebx+eax*4+0xdeadbeef]",
		.code = "\x03\x8c\x83\xef\xbe\xad\xde",
		.codesize = 7,
		.in_state.reg  = {0x2,0x1,0,0x1,0,0,0,0},
		.in_state.mem_state = {0xdeadbef8, 0x44443333},
		.out_state.reg  = {0x2,0x44443334,0,0x1,0,0,0,0},
		.out_state.mem_state = {0xdeadbef8, 0x44443333},
	},
	/* 04 */
	{
		.instr = "add al,0x11",
		.code = "\x04\x11",
		.codesize = 2,
		.in_state.reg  = {0x22222222,0,0,0,0,0,0,0},
		.in_state.mem_state = {0, 0},
		.out_state.reg  = {0x22222233,0,0,0,0,0,0,0},
		.out_state.mem_state = {0, 0},
		.out_state.eflags =  FLAG(f_pf),
	},
	/* 05 */
	{
		.instr = "add ax,0x1111",
		.code = "\x66\x05\x11\x11",
		.codesize = 4,
		.in_state.reg  = {0x22222222,0,0,0,0,0,0,0},
		.in_state.mem_state = {0, 0},
		.out_state.reg  = {0x22223333,0,0,0,0,0,0,0},
		.out_state.mem_state = {0, 0},
		.out_state.eflags =  FLAG(f_pf),
	},
	{
		.instr = "add eax,0x11111111",
		.code = "\x05\x11\x11\x11\x11",
		.codesize = 5,
		.in_state.reg  = {0x22222222,0,0,0,0,0,0,0},
		.in_state.mem_state = {0, 0},
		.out_state.reg  = {0x33333333,0,0,0,0,0,0,0},
		.out_state.mem_state = {0, 0},
		.out_state.eflags =  FLAG(f_pf),
	},

	





	/* 08 */
	{
		.instr = "or ah,al",
//		.code = "\x00\xc4",
//		.codesize = 2,
		.in_state.reg  = {0xff01,0,0,0,0,0,0,0},
		.in_state.mem_state = {0, 0},
		.out_state.reg  = {0x0ff01,0,0,0,0,0,0,0},
		.out_state.mem_state = {0, 0},
		.out_state.eflags = FLAG(f_sf) | FLAG(f_pf),
	},
	{
		.instr = "or ch,dl",
//		.code = "\x00\xd5",
//		.codesize = 2,
		.in_state.reg  = {0,0x1000,0x20,0,0,0,0,0},
		.in_state.mem_state = {0, 0},
		.out_state.reg  = {0,0x3000,0x20,0,0,0,0,0},
		.out_state.mem_state = {0, 0},
		.out_state.eflags =  FLAG(f_pf),
	},
	{
		.instr = "or [ecx],al",
//		.code = "\x00\x01",
//		.codesize = 2,
		.in_state.reg  = {0x10,0x40000,0,0,0,0,0,0},
		.in_state.mem_state = {0x40000, 0x10101010},
		.out_state.reg  = {0x10,0x40000,0,0,0,0,0,0},
		.out_state.mem_state = {0x40000, 0x10101010},
	},
	/* 09 */
	{
		.instr = "or ax,cx",
//		.code = "\x66\x01\xc8",
//		.codesize = 3,
		.in_state.reg  = {0xffff1111,0xffff2222,0,0,0,0,0,0},
		.in_state.mem_state = {0, 0},
		.out_state.reg  = {0xffff3333,0xffff2222,0,0,0,0,0,0},
		.out_state.mem_state = {0, 0},
		.out_state.eflags =  FLAG(f_pf), 
	},
	{
		.instr = "or [ecx],ax",
//		.code = "\x66\x01\x01",
//		.codesize = 3,
		.in_state.reg  = {0xffff1111,0x40000,0,0,0,0,0,0},
		.in_state.mem_state = {0x40000, 0x22224444},
		.out_state.reg  = {0xffff1111,0x40000,0,0,0,0,0,0},
		.out_state.mem_state = {0x40000, 0x22225555},
		.out_state.eflags =  FLAG(f_pf), 
	},
	{
		.instr = "or eax,ecx",
//		.code = "\x01\xc8",
//		.codesize = 2,
		.in_state.reg  = {0x11112222,0x22221111,0,0,0,0,0,0},
		.in_state.mem_state = {0, 0},
		.out_state.reg  = {0x33333333,0x22221111,0,0,0,0,0,0},
		.out_state.mem_state = {0, 0},
		.out_state.eflags =  FLAG(f_pf), 
	},
	{
		.instr = "or [ecx],eax",
//		.code = "\x01\x01",
//		.codesize = 2,
		.in_state.reg  = {0x22221111,0x40000,0,0,0,0,0,0},
		.in_state.mem_state = {0x40000, 0x22224444},
		.out_state.reg  = {0x22221111,0x40000,0,0,0,0,0,0},
		.out_state.mem_state = {0x40000, 0x22225555},
		.out_state.eflags =  FLAG(f_pf), 
	},
	/* 0a */
	{
		.instr = "or cl,bh",
//		.code = "\x02\xcf",	/* or cl,bh */
//		.codesize = 2,
		.in_state.reg  = {0,0xff,0,0x100,0,0,0,0},
		.in_state.mem_state = {0, 0},
		.out_state.reg  = {0,0xff,0,0x100,0,0,0,0},
		.out_state.mem_state = {0, 0},
		.out_state.eflags =  FLAG(f_pf) | FLAG(f_sf), 
	},
	{
		.instr = "or al,[ecx]",
//		.code = "\x02\x01",
//		.codesize = 2,
		.in_state.reg  = {0x3,0x40000,0,0,0,0,0,0},
		.in_state.mem_state = {0x40000, 0x30303030},
		.out_state.reg  = {0x33,0x40000,0,0,0,0,0,0},
		.out_state.mem_state = {0x40000, 0x30303030},
		.out_state.eflags =  FLAG(f_pf),
	},
	/* 0b */
	{
		.instr = "or cx,di",
//		.code = "\x66\x03\xcf",	/* or cx,di */
//		.codesize = 3,
		.in_state.reg  = {0,0x10101010,0,0,0,0,0,0x02020202},
		.in_state.mem_state = {0, 0},
		.out_state.reg  = {0,0x10101212,0,0,0,0,0,0x02020202},
		.out_state.mem_state = {0, 0},
		.out_state.eflags =  FLAG(f_pf),
	},
	{
		.instr = "or ax,[ecx]",
//		.code = "\x66\x03\x01",
//		.codesize = 3,
		.in_state.reg  = {0x11112222,0x40000,0,0,0,0,0,0},
		.in_state.mem_state = {0x40000, 0x44443333},
		.out_state.reg  = {0x11113333,0x40000,0,0,0,0,0,0},
		.out_state.mem_state = {0x40000, 0x44443333},
		.out_state.eflags =  FLAG(f_pf),
	},
	{
		.instr = "or ecx,edi",
//		.code = "\x03\xcf",	/* or ecx,edi */
//		.codesize = 2,
		.in_state.reg  = {0,0x10101010,0,0,0,0,0,0x02020202},
		.in_state.mem_state = {0, 0},
		.out_state.reg  = {0,0x12121212,0,0,0,0,0,0x02020202},
		.out_state.mem_state = {0, 0},
		.out_state.eflags =  FLAG(f_pf),
	},
	{
		.instr = "or eax,[ecx]",
//		.code = "\x03\x01",
//		.codesize = 2,
		.in_state.reg  = {0x11112222,0x40000,0,0,0,0,0,0},
		.in_state.mem_state = {0x40000, 0x44443333},
		.out_state.reg  = {0x55553333,0x40000,0,0,0,0,0,0},
		.out_state.mem_state = {0x40000, 0x44443333},
		.out_state.eflags =  FLAG(f_pf),
	},
	{
		.instr = "or ecx,[ebx+eax*4+0xdeadbeef]",
		.code = "\x0b\x8c\x83\xef\xbe\xad\xde",
		.codesize = 7,
		.in_state.reg  = {0x2,0x1,0,0x1,0,0,0,0},
		.in_state.mem_state = {0xdeadbef8, 0x44443333},
		.out_state.reg  = {0x2,0x44443333,0,0x1,0,0,0,0},
		.out_state.mem_state = {0xdeadbef8, 0x44443333},
		.out_state.eflags =  FLAG(f_pf),
	},
	/* 0c */
	{
		.instr = "or al,0x11",
//		.code = "\x04\x11",
//		.codesize = 2,
		.in_state.reg  = {0x22222222,0,0,0,0,0,0,0},
		.in_state.mem_state = {0, 0},
		.out_state.reg  = {0x22222233,0,0,0,0,0,0,0},
		.out_state.mem_state = {0, 0},
		.out_state.eflags =  FLAG(f_pf),
	},
	/* 0d */
	{
		.instr = "or ax,0x1111",
		.code = "\x66\x0d\x11\x11",
		.codesize = 4,
		.in_state.reg  = {0x22222222,0,0,0,0,0,0,0},
		.in_state.mem_state = {0, 0},
		.out_state.reg  = {0x22223333,0,0,0,0,0,0,0},
		.out_state.mem_state = {0, 0},
		.out_state.eflags =  FLAG(f_pf),
	},
	{
		.instr = "or eax,0x11111111",
		.code = "\x0d\x11\x11\x11\x11",
		.codesize = 5,
		.in_state.reg  = {0x22222222,0,0,0,0,0,0,0},
		.in_state.mem_state = {0, 0},
		.out_state.reg  = {0x33333333,0,0,0,0,0,0,0},
		.out_state.mem_state = {0, 0},
		.out_state.eflags =  FLAG(f_pf),
	},









    /* 10 */
	{
		.instr = "adc ah,al",
//		.code = "\x00\xc4",
//		.codesize = 2,
		.in_state.reg  = {0xff01,0,0,0,0,0,0,0},
		.in_state.mem_state = {0, 0},
		.in_state.eflags = FLAG(f_cf),
		.out_state.reg  = {0x101,0,0,0,0,0,0,0},
		.out_state.mem_state = {0, 0},
		.out_state.eflags = FLAG(f_cf) ,
	},
	{
		.instr = "adc ch,dl",
//		.code = "\x00\xd5",
//		.codesize = 2,
		.in_state.reg  = {0,0x1000,0x20,0,0,0,0,0},
		.in_state.mem_state = {0, 0},
		.in_state.eflags = FLAG(f_cf),
		.out_state.reg  = {0,0x3100,0x20,0,0,0,0,0},
		.out_state.mem_state = {0, 0},

	},
	{
		.instr = "adc [ecx],al",
//		.code = "\x00\x01",
//		.codesize = 2,
		.in_state.reg  = {0x10,0x40000,0,0,0,0,0,0},
		.in_state.mem_state = {0x40000, 0x10101010},
		.in_state.eflags = FLAG(f_cf),
		.out_state.reg  = {0x10,0x40000,0,0,0,0,0,0},
		.out_state.mem_state = {0x40000, 0x10101021},
		.out_state.eflags = FLAG(f_pf) ,
	},
	/* 11 */
	{
		.instr = "adc ax,cx",
//		.code = "\x66\x01\xc8",
//		.codesize = 3,
		.in_state.reg  = {0xffff1111,0xffff2222,0,0,0,0,0,0},
		.in_state.mem_state = {0, 0},
		.in_state.eflags = FLAG(f_cf),
		.out_state.reg  = {0xffff3334,0xffff2222,0,0,0,0,0,0},
		.out_state.mem_state = {0, 0},
//		.out_state.eflags =  FLAG_SET(f_pf), 
	},
	{
		.instr = "adc [ecx],ax",
//		.code = "\x66\x01\x01",
//		.codesize = 3,
		.in_state.reg  = {0xffff1111,0x40000,0,0,0,0,0,0},
		.in_state.mem_state = {0x40000, 0x22224444},
		.in_state.eflags = FLAG(f_cf),
		.out_state.reg  = {0xffff1111,0x40000,0,0,0,0,0,0},
		.out_state.mem_state = {0x40000, 0x22225556},
		.out_state.eflags =  FLAG(f_pf), 
	},
	{
		.instr = "adc eax,ecx",
//		.code = "\x01\xc8",
//		.codesize = 2,
		.in_state.reg  = {0x11112222,0x22221111,0,0,0,0,0,0},
		.in_state.mem_state = {0, 0},
		.in_state.eflags = FLAG(f_cf),
		.out_state.reg  = {0x33333334,0x22221111,0,0,0,0,0,0},
		.out_state.mem_state = {0, 0},
//		.out_state.eflags =  FLAG_SET(f_pf), 
	},
	{
		.instr = "adc [ecx],eax",
//		.code = "\x01\x01",
//		.codesize = 2,
		.in_state.reg  = {0x22221111,0x40000,0,0,0,0,0,0},
		.in_state.mem_state = {0x40000, 0x22224444},
		.in_state.eflags = FLAG(f_cf),
		.out_state.reg  = {0x22221111,0x40000,0,0,0,0,0,0},
		.out_state.mem_state = {0x40000, 0x44445556},
		.out_state.eflags =  FLAG(f_pf), 
	},
	/* 12 */
	{
		.instr = "adc cl,bh",
//		.code = "\x02\xcf",	/* adc cl,bh */
//		.codesize = 2,
		.in_state.reg  = {0,0xff,0,0x100,0,0,0,0},
		.in_state.mem_state = {0, 0},
		.in_state.eflags = FLAG(f_cf),
		.out_state.reg  = {0,0x1,0,0x100,0,0,0,0},
		.out_state.mem_state = {0, 0},
		.out_state.eflags =  FLAG(f_cf), 
	},
	{
		.instr = "adc al,[ecx]",
//		.code = "\x02\x01",
//		.codesize = 2,
		.in_state.reg  = {0x3,0x40000,0,0,0,0,0,0},
		.in_state.mem_state = {0x40000, 0x30303030},
		.in_state.eflags = FLAG(f_cf),
		.out_state.reg  = {0x34,0x40000,0,0,0,0,0,0},
		.out_state.mem_state = {0x40000, 0x30303030},

	},
	/* 13 */
	{
		.instr = "adc cx,di",
//		.code = "\x66\x03\xcf",	/* adc cx,di */
//		.codesize = 3,
		.in_state.reg  = {0,0x10101010,0,0,0,0,0,0x02020202},
		.in_state.mem_state = {0, 0},
		.in_state.eflags = FLAG(f_cf),
		.out_state.reg  = {0,0x10101213,0,0,0,0,0,0x02020202},
		.out_state.mem_state = {0, 0},

	},
	{
		.instr = "adc ax,[ecx]",
//		.code = "\x66\x03\x01",
//		.codesize = 3,
		.in_state.reg  = {0x11112222,0x40000,0,0,0,0,0,0},
		.in_state.mem_state = {0x40000, 0x44443333},
		.in_state.eflags = FLAG(f_cf),
		.out_state.reg  = {0x11115556,0x40000,0,0,0,0,0,0},
		.out_state.mem_state = {0x40000, 0x44443333},
		.out_state.eflags =  FLAG(f_pf),
	},
	{
		.instr = "adc ecx,edi",
//		.code = "\x03\xcf",	/* adc ecx,edi */
//		.codesize = 2,
		.in_state.reg  = {0,0x10101010,0,0,0,0,0,0x02020202},
		.in_state.mem_state = {0, 0},
		.in_state.eflags = FLAG(f_cf),
		.out_state.reg  = {0,0x12121213,0,0,0,0,0,0x02020202},
		.out_state.mem_state = {0, 0},
//		.out_state.eflags =  FLAG_SET(f_pf),
	},
	{
		.instr = "adc eax,[ecx]",
//		.code = "\x03\x01",
//		.codesize = 2,
		.in_state.reg  = {0x11112222,0x40000,0,0,0,0,0,0},
		.in_state.mem_state = {0x40000, 0x44443333},
		.in_state.eflags = FLAG(f_cf),
		.out_state.reg  = {0x55555556,0x40000,0,0,0,0,0,0},
		.out_state.mem_state = {0x40000, 0x44443333},
		.out_state.eflags =  FLAG(f_pf),
	},
	{
		.instr = "adc ecx,[ebx+eax*4+0xdeadbeef]",
		.code = "\x13\x8c\x83\xef\xbe\xad\xde",
		.codesize = 7,
		.in_state.reg  = {0x2,0x1,0,0x1,0,0,0,0},
		.in_state.mem_state = {0xdeadbef8, 0x44443333},
		.out_state.reg  = {0x2,0x44443334,0,0x1,0,0,0,0},
		.out_state.mem_state = {0xdeadbef8, 0x44443333},
	},
	/* 14 */
	{
		.instr = "adc al,0x11",
//		.code = "\x04\x11",
//		.codesize = 2,
		.in_state.reg  = {0x22222222,0,0,0,0,0,0,0},
		.in_state.mem_state = {0, 0},
		.out_state.reg  = {0x22222233,0,0,0,0,0,0,0},
		.out_state.mem_state = {0, 0},
		.out_state.eflags =  FLAG(f_pf),
	},
	/* 15 */
	{
		.instr = "adc ax,0x1111",
		.code = "\x66\x15\x11\x11",
		.codesize = 4,
		.in_state.reg  = {0x22222222,0,0,0,0,0,0,0},
		.in_state.mem_state = {0, 0},
		.out_state.reg  = {0x22223333,0,0,0,0,0,0,0},
		.out_state.mem_state = {0, 0},
		.out_state.eflags =  FLAG(f_pf),
	},
	{
		.instr = "adc eax,0x11111111",
		.code = "\x15\x11\x11\x11\x11",
		.codesize = 5,
		.in_state.reg  = {0x22222222,0,0,0,0,0,0,0},
		.in_state.mem_state = {0, 0},
		.out_state.reg  = {0x33333333,0,0,0,0,0,0,0},
		.out_state.mem_state = {0, 0},
		.out_state.eflags =  FLAG(f_pf),
	},




	{
		.instr = "jmp ecx",
		.in_state.reg  = {0,0xdeadbeef,0,0,0,0,0,0},
		.in_state.mem_state = {0, 0},
		.out_state.reg  = {0,0xdeadbeef,0,0,0,0,0,0},
		.out_state.mem_state = {0, 0},
		.out_state.eip = 0xdeadbeef,
	},
	{
		.instr = "jmp [eax]",
		.in_state.reg  = {0xdeadbabe,0,0,0,0,0,0,0},
		.in_state.mem_state = {0xdeadbabe, 0xdeafcafe},
		.out_state.reg  = {0xdeadbabe,0,0,0,0,0,0,0},
		.out_state.mem_state = {0xdeadbabe, 0xdeafcafe},
		.out_state.eip = 0xdeafcafe,
	},
	{
		.instr = "jmp +16",
		.code = "\xeb\x10", /* jmp +16*/
		.codesize = 2,
		.out_state.eip = (CODE_OFFSET + 2 + 0x10),
	},
	{
		.instr = "jmp -1",
		.code = "\xeb\xff", /* jmp -1 */
		.codesize = 2,
		.out_state.eip = (CODE_OFFSET + 2 + -1),
	},
	{
		.instr = "jmp +0x01000000",
		.code = "\xe9\x00\x00\x00\x01", /* jmp +0x01000000 */
		.codesize = 5,
		.out_state.eip = (CODE_OFFSET + 5 + 0x1000000),
	},
	
	{
		.instr = "mov al, 0xff",
		.out_state.reg = {0xff,0,0,0,0,0,0,0},
	},
	{
		.instr = "mov ah, 0xff",
		.out_state.reg = {0xff00,0,0,0,0,0,0,0},
	},
	{
		.instr = "mov ax, 0xffff",
		.code = "\x66\xb8\xff\xff",
		.codesize = 4,
		.out_state.reg = {0xffff,0,0,0,0,0,0,0},
	},
	{
		.instr = "mov eax, 0xffffffff",
		.code = "\xb8\xff\xff\xff\xff",
		.codesize = 5,
		.out_state.reg = {0xffffffff,0,0,0,0,0,0,0},
	},
	{
		.instr = "mov eax, [esp+0x24]",
		.in_state.mem_state = {0xffffff24, 0xfefefefe},
		.in_state.reg = {0,0,0,0,0xffffff00,0,0,0},
		.out_state.mem_state = {0xffffff24, 0xfefefefe},
		.out_state.reg = {0xfefefefe,0,0,0,0xffffff00,0,0,0},
	},
	{
		.instr = "xor dword [eax+0x1000], 0x11111111",
		.code = "\x81\xb0\x00\x10\x00\x00\x11\x11\x11\x11",
		.codesize = 10,
		.in_state.mem_state = {0x2000, 0x22222222},
		.in_state.reg = {0x1000,0,0,0,0,0,0,0},
		.out_state.mem_state = {0x2000, 0x33333333},
		.out_state.reg = {0x1000,0,0,0,0,0,0,0},
		.out_state.eflags =  FLAG(f_pf),
	},
	{
		.instr = "mov eax, [ebp+ecx*4-0x100]",
		.code = "\x8b\x84\x8d\x00\xff\xff\xff",
		.codesize = 7,
		.in_state.mem_state = {0x140, 0x22222222},
		.in_state.reg = {0x1000,0x10,0,0,0,0x200,0,0},
		.out_state.reg = {0x22222222,0x10,0,0,0,0x200,0,0},
	},
	{
		.instr = "mov eax, [ebp+ecx*4-0x10000000]",
		.code = "\x8b\x84\x8d\x00\x00\x00\xf0",
		.codesize = 7,
		.in_state.mem_state = {0x14000000, 0x22222222},
		.in_state.reg = {0x1000,0x1000000,0,0,0,0x20000000,0,0},
		.out_state.reg = {0x22222222,0x1000000,0,0,0,0x20000000,0,0},
	},



};

int prepare(void)
{
	int i;
	for (i=0;i<sizeof(tests)/sizeof(struct instr_test);i++)
	{
		if ( (opts.nasm_force == 0 && tests[i].code != NULL) || tests[i].instr == 0)
		{ // dup it so we can free it
			char *c = (char *)malloc(tests[i].codesize);
			memcpy(c,tests[i].code,tests[i].codesize);
			tests[i].code = c;
		}
		else
		{
			const char *use = "USE32\n";
			FILE *f=fopen("/tmp/foo.S","w+");

			if (f == NULL)
			{
				printf("failed to create asm file for nasm instruction %s\n\n\t%s",tests[i].instr,strerror(errno));
				return -1;
			}

			if( fwrite(use,strlen(use),1,f) != 1 ||
				fwrite(tests[i].instr,1,strlen(tests[i].instr),f) != strlen(tests[i].instr) )
				exit(-1);
			fclose(f);

			if( system("cd /tmp/; nasm foo.S") != 0 )
				exit(-1);

			f=fopen("/tmp/foo","r");
			if (f == NULL)
			{
				printf("failed to open compiled nasm file for read for instruction %s\n\n\t%s",tests[i].instr,strerror(errno));
				return -1;
			}

			fseek(f,0,SEEK_END);

			tests[i].codesize = ftell(f);
			tests[i].code = malloc(tests[i].codesize);
			fseek(f,0,SEEK_SET);
			if( fread(tests[i].code,1,tests[i].codesize,f) != tests[i].codesize )
				exit(-1);
			fclose(f);

			unlink("/tmp/foo.S");
			unlink("/tmp/foo");
		}
	}
	return 0;
}




int test(int n)
{
	int i=0;
	struct emu *e = emu_new();
	struct emu_cpu *cpu = emu_cpu_get(e);
	struct emu_memory *mem = emu_memory_get(e);

	for (i=0;i<sizeof(tests)/sizeof(struct instr_test);i++)
	{
		if( n != -1 && i != n )
			continue;
			
		int failed = 0;


		printf("testing (#%d) '%s' \t", i, tests[i].instr);
		int j=0;

		if ( opts.verbose == 1 )
		{
			printf("code '");
			for ( j=0;j<tests[i].codesize;j++ )
			{
				printf("%02x ",(uint8_t)tests[i].code[j]);
			}
			printf("' ");
		}


		/* set the registers to the initial values */
		for ( j=0;j<8;j++ )
		{
			emu_cpu_reg32_set(cpu,j ,tests[i].in_state.reg[j]);
		}
   	

		/* set the flags */
		emu_cpu_eflags_set(cpu,tests[i].in_state.eflags);


		/* write the code to the offset */
		int static_offset = CODE_OFFSET;
		for( j = 0; j < tests[i].codesize; j++ )
		{
			emu_memory_write_byte(mem, static_offset+j, tests[i].code[j]);
		}

		if (tests[i].in_state.mem_state[0] != 0 || tests[i].in_state.mem_state[1] != 0)
		{
			printf("writing mem\n");
			emu_memory_write_dword(mem, tests[i].in_state.mem_state[0], tests[i].in_state.mem_state[1]);
		}

		if (opts.verbose)
		{
			printf("memory at 0x%08x = 0x%08x (%i %i)\n",
				   tests[i].in_state.mem_state[0], 
				   tests[i].in_state.mem_state[1],
				   (int)tests[i].in_state.mem_state[1],
				   (uint32_t)tests[i].in_state.mem_state[1]);
		}


		/* set eip to the code */
		emu_cpu_eip_set(emu_cpu_get(e), static_offset);

		/* run the code */
		if (opts.verbose == 1 )
		{
        	emu_log_level_set(emu_logging_get(e),EMU_LOG_DEBUG);
			emu_cpu_debug_print(cpu);
			emu_log_level_set(emu_logging_get(e),EMU_LOG_NONE);
		}
		
		int ret; //= emu_cpu_run(emu_cpu_get(e));

		for (j=0;j<opts.steps;j++)
		{
			ret = emu_cpu_parse(emu_cpu_get(e));
			ret = emu_cpu_step(emu_cpu_get(e));
			if (opts.verbose == 1)
			{
				emu_log_level_set(emu_logging_get(e),EMU_LOG_DEBUG);
				emu_cpu_debug_print(cpu);
				emu_log_level_set(emu_logging_get(e),EMU_LOG_NONE);
			}
			if ( ret != 0 )
			{
				printf("cpu error %s\n", emu_strerror(e));
				break;
			}
		}
   

		if (opts.verbose == 1)
		{
			emu_log_level_set(emu_logging_get(e),EMU_LOG_DEBUG);
			emu_cpu_debug_print(cpu);
			emu_log_level_set(emu_logging_get(e),EMU_LOG_NONE);
		}
        	

		/* check the registers for the exptected values */

		for ( j=0;j<8;j++ )
		{
			if ( emu_cpu_reg32_get(cpu, j) ==  tests[i].out_state.reg[j] )
			{
				if (opts.verbose == 1)
					printf("\t %s "SUCCESS"\n",regm[j]);
			}
			else
			{
				printf("\t %s "FAILED" got 0x%08x expected 0x%08x\n",regm[j],emu_cpu_reg32_get(cpu, j),tests[i].out_state.reg[j]);
				failed = 1;
			}
		}


		/* check the memory for expected values */
		uint32_t value;

		if ( tests[i].out_state.mem_state[0] != 0 ||  tests[i].out_state.mem_state[1] != 0)
		{
			if ( emu_memory_read_dword(mem,tests[i].out_state.mem_state[0],&value) == 0 )
			{
				if ( value == tests[i].out_state.mem_state[1] )
				{
					if (opts.verbose == 1)
						printf("\t memory "SUCCESS" 0x%08x = 0x%08x\n",tests[i].out_state.mem_state[0], tests[i].out_state.mem_state[1]);
				}
				else
				{
					printf("\t memory "FAILED" at 0x%08x got 0x%08x expected 0x%08x\n",tests[i].out_state.mem_state[0],value, tests[i].out_state.mem_state[1]);
					failed = 1;
				}

			}
			else
			{
				printf("\tmemory "FAILED" emu says: '%s' when accessing %08x\n", strerror(emu_errno(e)),tests[i].out_state.mem_state[0]);
				failed = 1;
			}

		}

		/* check the cpu flags for expected values */
		if ( tests[i].out_state.eflags != emu_cpu_eflags_get(cpu) )
		{
			printf("\t flags "FAILED" got %08x expected %08x\n",emu_cpu_eflags_get(cpu),tests[i].out_state.eflags);
			for(j=0;j<32;j++)
			{
				uint32_t f = emu_cpu_eflags_get(cpu);
				if ( (tests[i].out_state.eflags & (1 << j)) != (f & (1 <<j)))
					printf("\t flag %s (bit %i) failed, expected %i is %i\n",flags[j], j, 
						   (tests[i].out_state.eflags & (1 << j)),
						   (f & (1 <<j)));
			}

			failed = 1;
		}else
		{
			if (opts.verbose == 1)
				printf("\t flags "SUCCESS"\n");
		}

		
		if( tests[i].out_state.eip != 0 && tests[i].out_state.eip != emu_cpu_eip_get(cpu) )
		{
			printf("\t %s "FAILED" got 0x%08x expected 0x%08x\n", "eip", emu_cpu_eip_get(cpu), tests[i].out_state.eip);
			failed = 1;
		}


		/* bail out on *any* error */
		if (failed == 0)
		{
			printf(SUCCESS"\n");
		}else
		{
			return -1;
		}
		
	}
	emu_free(e);
	return 0;
}

void cleanup(void)
{
	int i;
	for (i=0;i<sizeof(tests)/sizeof(struct instr_test);i++)
    	if (tests[i].code != NULL)
    		free(tests[i].code);
		
}

void dump_export_table(void)
{
/*	int i;
	for (i=0;i<sizeof(mem_7c801000);i++)
		printf("%c",mem_7c801000[i]);
*/
}

int main(int argc, char *argv[])
{
	memset(&opts,0,sizeof(struct run_time_options));

	opts.steps = 1;
	opts.testnumber = -1;

	while ( 1 )
	{	
		int c;
		int option_index = 0;
		static struct option long_options[] = {
			{"verbose"			, 0, 0, 'v'},
			{"nasm-force"		, 0, 0, 'n'},
			{"steps"			, 1, 0, 's'},
			{"testnumber"		, 1, 0, 't'},
			{0, 0, 0, 0}
		};

		c = getopt_long (argc, argv, "vns:t:", long_options, &option_index);
		if ( c == -1 )
			break;

		switch ( c )
		{
		case 'v':
			opts.verbose = 1;
			break;

		case 'n':
			opts.nasm_force = 1;
			break;

		case 's':
			opts.steps = atoi(optarg);
			break;

		case 't':
			opts.testnumber = atoi(optarg);
			break;


		default:
			printf ("?? getopt returned character code 0%o ??\n", c);
			break;
		}
	}



	if ( prepare() != 0)
		return -1;

	if ( test(opts.testnumber) != 0 )
		return -1;

	cleanup();

//	dump_export_table();
	return 0;
}
