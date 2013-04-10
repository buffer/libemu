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
#include <errno.h>
#include <stdint.h>
#include <stdbool.h>

#include "../config.h"

#include "emu/emu_cpu.h"
#include "emu/emu_cpu_data.h"
#include "emu/emu_memory.h"
#include "emu/emu.h"
#include "emu/emu_log.h"
#include "emu/emu_breakpoint.h"

static const char *regm[] = {
	"eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi"
};

static uint8_t scalem[] = {
	1, 2, 4, 8
};

	                      /* 0     1     2     3      4       5       6     7 */
   const char *eflagm[] = { "CF", "  ", "PF", "  " , "AF"  , "    ", "ZF", "SF", 
	                        "TF", "IF", "DF", "OF" , "IOPL", "IOPL", "NT", "  ",
	                        "RF", "VM", "AC", "VIF", "RIP" , "ID"  , "  ", "  ",
	                        "  ", "  ", "  ", "   ", "    ", "    ", "  ", "  "};


static uint16_t prefix_map[0x100];

#include "emu/emu_cpu_itables.h"

static void init_prefix_map(void)
{
	prefix_map[0x26] = PREFIX_ES_OVR;
	prefix_map[0x2e] = PREFIX_CS_OVR;
	prefix_map[0x36] = PREFIX_SS_OVR;
	prefix_map[0x3e] = PREFIX_DS_OVR;
	prefix_map[0x64] = PREFIX_FS_OVR;
	prefix_map[0x65] = PREFIX_GS_OVR;
	prefix_map[0x66] = PREFIX_OPSIZE;
	prefix_map[0x67] = PREFIX_ADSIZE;
	prefix_map[0xf0] = PREFIX_LOCK;
	prefix_map[0xf2] = PREFIX_F2;
	prefix_map[0xf3] = PREFIX_F3;
}

struct emu_cpu *emu_cpu_new(struct emu *e)
{
	struct emu_cpu *c = (struct emu_cpu *)malloc(sizeof(struct emu_cpu));
	
	if( c == NULL )
	{
		return NULL;
	}
	
	memset((void *)c, 0, sizeof(struct emu_cpu));
	
	c->emu = e;
	c->mem = emu_memory_get(e);

	int i = 1;

	if( *((uint8_t *)&i) == 1 )
	{
		logDebug(e,"little endian\n");

		for( i = 0; i < 8; i++ )
		{
			c->reg16[i] = (uint16_t *)&c->reg[i];

			if( i < 4 )
			{
				c->reg8[i] = (uint8_t *)&c->reg[i];
			}
			else
			{
				c->reg8[i] = (uint8_t *)&c->reg[i & 3] + 1;
			}
		}

		c->instr.cpu.imm16 = (uint16_t *)((void *)&c->instr.cpu.imm);
		c->instr.cpu.imm8 = (uint8_t *)&c->instr.cpu.imm;

	}
	else
	{
		logDebug(e,"big endian\n");

		for( i = 0; i < 8; i++ )
		{
			c->reg16[i] = (uint16_t *)&c->reg[i] + 1;

			if( i < 4 )
			{
				c->reg8[i] = (uint8_t *)&c->reg[i] + 3;
			}
			else
			{
				c->reg8[i] = (uint8_t *)&c->reg[i & 3] + 2;
			}
		}

		c->instr.cpu.imm16 = (uint16_t *)((void *)&c->instr.cpu.imm + 1);
		c->instr.cpu.imm8 = (uint8_t *)&c->instr.cpu.imm + 3;

	}

	c->instr_string = (char *)malloc(92);
	c->repeat_current_instr = false;
	init_prefix_map();
	
	return c;
}

inline uint32_t emu_cpu_reg32_get(struct emu_cpu *cpu_p, enum emu_reg32 reg)
{
	return cpu_p->reg[reg];
}

inline void  emu_cpu_reg32_set(struct emu_cpu *cpu_p, enum emu_reg32 reg, uint32_t val)
{
	cpu_p->reg[reg] = val;
}

inline uint16_t emu_cpu_reg16_get(struct emu_cpu *cpu_p, enum emu_reg16 reg)
{
	return *cpu_p->reg16[reg];
}

inline void emu_cpu_reg16_set(struct emu_cpu *cpu_p, enum emu_reg16 reg, uint16_t val)
{
	*cpu_p->reg16[reg] = val; 
}

inline uint8_t emu_cpu_reg8_get(struct emu_cpu *cpu_p, enum emu_reg8 reg)
{
	return *cpu_p->reg8[reg];
}


inline void emu_cpu_reg8_set(struct emu_cpu *cpu_p, enum emu_reg8 reg, uint8_t val)
{
	*cpu_p->reg8[reg] = val;
}

uint32_t emu_cpu_eflags_get(struct emu_cpu *c)
{
	return c->eflags;
}

void emu_cpu_eflags_set(struct emu_cpu *c, uint32_t val)
{
	c->eflags = val;
}

void emu_cpu_eip_set(struct emu_cpu *c, uint32_t val)
{
	c->eip = val;
	c->repeat_current_instr = false;
}

uint32_t emu_cpu_eip_get(struct emu_cpu *c)
{
	return c->eip;
}

void emu_cpu_free(struct emu_cpu *c)
{
	free(c->instr_string);
	free(c);
}

void emu_cpu_debug_print(struct emu_cpu *c)
{
	logDebug(c->emu,"cpu state    eip=0x%08x\n", c->eip);
	logDebug(c->emu,"eax=0x%08x  ecx=0x%08x  edx=0x%08x  ebx=0x%08x\n", c->reg[eax], c->reg[ecx], c->reg[edx], c->reg[ebx]);
	logDebug(c->emu,"esp=0x%08x  ebp=0x%08x  esi=0x%08x  edi=0x%08x\n", c->reg[esp], c->reg[ebp], c->reg[esi], c->reg[edi]);


	char *fmsg;
	fmsg = (char *)malloc(32*3+1);
	memset(fmsg, 0, 32*3+1);
	int i;
	for ( i=0;i<32;i++ )
	{
		if ( CPU_FLAG_ISSET(c, i) )
		{
			strcat(fmsg, eflagm[i]);
			strcat(fmsg," ");
		}
	}
	logDebug(c->emu,"Flags: %s\n", fmsg);
	free(fmsg);

	return;




	for (i=0; i<8; i++)
	{
		printf("%08x ",c->reg[esp] + i * 4);
	}
	printf("\n");

	for (i=0; i<8; i++)
	{
		uint32_t d;
		emu_memory_read_dword(c->mem, c->reg[esp] + i * 4,  &d);
		printf("%08x ",d);
	}
	printf("\n");
	
}

void debug_instruction(struct emu_instruction *ei)
{
	struct emu_cpu_instruction *i = &ei->cpu;
	struct emu_cpu_instruction_info *ii;
	
	if( i->opc == 0x0f )
		ii = &ii_twobyte[i->opc_2nd];
	else
		ii = &ii_onebyte[i->opc];
	
	printf("%s ", ii->name);
	
	if( ii->format.modrm_byte != 0 )
	{
		if( ii->format.modrm_byte == II_XX_YYY_REG )
		{	
			printf("%s", regm[i->modrm.reg]);
		}
		else if( ii->format.modrm_byte == II_XX_REG1_REG2 )
		{
			printf("%s,%s", regm[i->modrm.reg1], regm[i->modrm.reg2]);
		}
		else if( ii->format.modrm_byte == II_MOD_REG_RM ||
			ii->format.modrm_byte == II_MOD_YYY_RM )
		{
			if( ii->format.modrm_byte == II_MOD_REG_RM )
			{
				printf("%s,", regm[i->modrm.opc]);
			}
			
			if( i->modrm.mod == 3 )
			{
				printf("%s", regm[i->modrm.rm]);
			}
			else
			{
				printf("[");
				
				if( i->modrm.rm != 4 && !(i->modrm.mod == 0 && i->modrm.rm == 5) )
					printf("%s", regm[i->modrm.rm]);
				
				if( i->modrm.rm == 4 ) /* sib? */
				{
					if( i->modrm.sib.base != 5 )
					{
						printf("%s", regm[i->modrm.sib.base]);
					}
					else
					{
						if( i->modrm.mod != 0 )
						{
							printf("%s", regm[ebp]);
						}
					}
					
					if( i->modrm.sib.index != 4 )
					{
						printf("+%s", regm[i->modrm.sib.index]);
						
						if( i->modrm.sib.scale > 0 )
						{
							printf("*%d", scalem[i->modrm.sib.scale]);
						}
					}
				}
				
				if( i->modrm.mod == 1 ) /* disp8 */
				{
					printf("+0x%02x", i->modrm.disp.s8);
				}
				else if( i->modrm.mod == 2 ) /* disp32 */
				{
					printf("+0x%08x", i->modrm.disp.s32);
				}
				
				printf("]");
				
				printf(" (ea=0x%08x)", i->modrm.ea);
			}
		}
	}
	
	if( ii->format.imm_data != 0 )
	{
	}
	printf("\n");

//	return;

	if (ei->is_fpu)
		return;

//	printf("%s\n",

	int j;

	bool trace_eflag_need = false;
	bool trace_eflag_init = false;
	bool trace_reg_need = false;
	bool trace_reg_init = false;

	for (j=0;j<8;j++)
	{
		if (ei->track.need.reg[j] != 0)
			trace_reg_need = true;

		if ( ei->track.need.eflags & 1 << j)
			trace_eflag_need = true;

		if (ei->track.init.reg[j] != 0)
			trace_reg_init = true;

		if ( ei->track.init.eflags & 1 << j)
			trace_eflag_init = true;

	}

	if ( trace_eflag_need ||  trace_reg_need )
	{


		printf("\ttrace:\n");

		if ( trace_eflag_need || trace_reg_need )
			printf("\t\tneeds \n");

		if ( trace_reg_need )
		{
			printf("\t\t\t reg  ");
			for ( j=0;j<8;j++ )
			{
				if ( ei->track.need.reg[j] != 0 )
					printf("%s ", regm[j]);

			}
			printf("\n");
		}

		if ( trace_eflag_need )
		{
			printf("\t\t\t eflag ");
			for ( j=0;j<8;j++ )
			{
				if ( ei->track.need.eflags & 1 << j )
					printf("%s ", eflagm[j]);

			}
			printf("\n");
		}
	}

	if ( trace_eflag_init || trace_reg_init )
		printf("\t\tinits \n");


	if ( trace_reg_init )
	{
		printf("\t\t\t reg ");
		for ( j=0;j<8;j++ )
		{
			if ( ei->track.init.reg[j] != 0 )
				printf("%s ", regm[j]);

		}
		printf("\n");
	}

	if ( trace_eflag_init )
	{
		printf("\t\t\t eflag ");
		for ( j=0;j<8;j++ )
		{
			if ( ei->track.init.eflags & 1 << j )
				printf("%s ", eflagm[j]);

		}
		printf("\n");
	}

	printf("\tsource:\n");
	printf("\t\tnormal pos 0x%08x\n", ei->source.norm_pos);
	if (ei->source.has_cond_pos == 1)
    	printf("\t\tcond pos 0x%08x\n", ei->source.cond_pos);
	

}

#undef PREFIX_LOCK

#include "libdasm.h"
uint32_t dasm_print_instruction(uint32_t eip, uint8_t *data, uint32_t size, char *str)
{
	INSTRUCTION inst;

	// step 2: fetch instruction-
	uint32_t instrsize = get_instruction(&inst, data, MODE_32);
	if( instrsize == 0 )
	{
//		printf("invalid instruction\n");
		return 0;
	}

#ifdef DEBUG
	str[81] = '\0';
	memset(str, 0x20, 81);

	int i; 
	for (i=0;i<instrsize;i++)
	{
		snprintf(str+i*2, 36-2*i, "%02X", data[i]);
	}
	memset(str+strlen(str), 0x20, 81-strlen(str));



	// step 3: print it
	get_instruction_string(&inst, FORMAT_INTEL, 0, str+32, 31);
#endif // DEBUG

	return instrsize;
}

int32_t emu_cpu_parse(struct emu_cpu *c)
{
	if (c->repeat_current_instr == true)
	{
		return 0;
	}

	/* TODO make unstatic for threadsafety */
	uint8_t byte;
	uint8_t *opcode;
	uint32_t ret;
	
	c->instr.prefixes = 0;
	
	
//	logDebug(c->emu,"decoding\n");
//	emu_cpu_debug_print(c);

	uint8_t dis[32];
	emu_memory_read_block(c->mem,c->eip,dis,32);
	emu_breakpoint_check(c->mem,c->eip, EMU_ACCESS_EXECUTE);

	uint32_t expected_instr_size = 0;
	if( CPU_DEBUG_FLAG_ISSET(c, instruction_string ) || CPU_DEBUG_FLAG_ISSET(c, instruction_size ) )
	{
		expected_instr_size = dasm_print_instruction(c->eip,dis,0,c->instr_string);
	}

	uint32_t eip_before = c->eip;
	uint32_t eip_after = 0;


	/* reset the instruction source and track infos, maybe move to a fn and call the fn instead? */

	c->instr.source.has_cond_pos = 0;

	c->instr.track.init.eflags = 0;
	memset(c->instr.track.init.reg, 0, sizeof(uint32_t) * 8);
	c->instr.track.init.fpu = 0;

	c->instr.track.need.eflags = 0;
	memset(c->instr.track.need.reg, 0, sizeof(uint32_t) * 8);
	c->instr.track.need.fpu = 0;


	while( 1 )
	{
		ret = emu_memory_read_byte(c->mem, c->eip++, &byte);
		
		if( ret != 0 )
			return ret;
		
		c->cpu_instr_info = &ii_onebyte[byte];
	
		if( c->cpu_instr_info->function == prefix_fn )
		{
			c->instr.prefixes |= prefix_map[byte];
			continue;
		}
		else
		{
			c->instr.opc = byte;
			if( c->cpu_instr_info->format.fpu_info == 0 )
			{
				c->instr.is_fpu = 0;
				c->instr.cpu.opc = c->instr.opc;
				c->instr.cpu.prefixes = c->instr.prefixes;
				
				if( c->instr.cpu.opc == 0x0f )
				{
					ret = emu_memory_read_byte(c->mem, c->eip++, &byte);
			
					if( ret != 0 )
						return ret;
						
					c->instr.cpu.opc_2nd = byte;
					opcode = &c->instr.cpu.opc_2nd;
					c->cpu_instr_info = &ii_twobyte[byte];
				}
				else
				{
					opcode = &c->instr.cpu.opc;
				}
				
				if ( c->cpu_instr_info->function == 0 )
				{
					emu_strerror_set(c->emu,"opcode %02x not supported\n", c->instr.cpu.opc);
					emu_errno_set(c->emu, EOPNOTSUPP);
/*					int y=0;
					for (y=0;y<expected_instr_size;y++)
					{
						printf("%02x ", dis[y]);
					}
					printf("\n");
*/
					return -1;
				}
				
				c->instr.cpu.w_bit = *opcode & 1;
				c->instr.cpu.s_bit = (*opcode >> 1) & 1;
	
				/* mod r/m byte?  sib/disp */
				if( c->cpu_instr_info->format.modrm_byte != 0 )
				{
					ret = emu_memory_read_byte(c->mem, c->eip++, &byte);
			
					if( ret != 0 )
						return ret;
						
					c->instr.cpu.modrm.mod = MODRM_MOD(byte);
					c->instr.cpu.modrm.opc = MODRM_REGOPC(byte);
					c->instr.cpu.modrm.rm = MODRM_RM(byte);
					
					if( c->cpu_instr_info->format.modrm_byte == II_MOD_REG_RM || c->cpu_instr_info->format.modrm_byte == II_MOD_YYY_RM ||
						c->cpu_instr_info->format.modrm_byte == II_XX_REG1_REG2) /* cases with possible sib/disp*/
					{
						if( c->instr.cpu.modrm.mod != 3 )
						{
							if( c->instr.cpu.modrm.rm != 4 && !(c->instr.cpu.modrm.mod == 0 && c->instr.cpu.modrm.rm == 5) )
							{
                                c->instr.cpu.modrm.ea = c->reg[c->instr.cpu.modrm.rm];
								TRACK_NEED_REG32(c->instr, c->instr.cpu.modrm.rm);
							}
							else
								c->instr.cpu.modrm.ea = 0;
							
							if( c->instr.cpu.modrm.rm == 4 ) /* sib byte present */
							{
								ret = emu_memory_read_byte(c->mem, c->eip++, &byte);
			
								if( ret != 0 )
									return ret;
									
								c->instr.cpu.modrm.sib.base = SIB_BASE(byte);
								c->instr.cpu.modrm.sib.scale = SIB_SCALE(byte);
								c->instr.cpu.modrm.sib.index = SIB_INDEX(byte);
								
								if( c->instr.cpu.modrm.sib.base != 5 )
								{
									c->instr.cpu.modrm.ea += c->reg[c->instr.cpu.modrm.sib.base];
									TRACK_NEED_REG32(c->instr, c->instr.cpu.modrm.sib.base);
								}
								else if( c->instr.cpu.modrm.mod != 0 )
								{
									c->instr.cpu.modrm.ea += c->reg[ebp];
									TRACK_NEED_REG32(c->instr, ebp);
								}
	
								if( c->instr.cpu.modrm.sib.index != 4 )
								{
									c->instr.cpu.modrm.ea += c->reg[c->instr.cpu.modrm.sib.index] * scalem[c->instr.cpu.modrm.sib.scale];
									TRACK_NEED_REG32(c->instr, c->instr.cpu.modrm.sib.index);
								}
							}
							
							if( c->instr.cpu.modrm.mod == 1 ) /* disp8 */
							{
								ret = emu_memory_read_byte(c->mem, c->eip++, &c->instr.cpu.modrm.disp.s8);
			
								if( ret != 0 )
									return ret;
								
								c->instr.cpu.modrm.ea += (int8_t)c->instr.cpu.modrm.disp.s8;
							}
							else if( c->instr.cpu.modrm.mod == 2 || (c->instr.cpu.modrm.mod == 0 && c->instr.cpu.modrm.rm == 5) ) /* disp32 */
							{
								ret = emu_memory_read_dword(c->mem, c->eip, &c->instr.cpu.modrm.disp.s32);
								c->eip += 4;
			
								if( ret != 0 )
									return ret;
	
								c->instr.cpu.modrm.ea += c->instr.cpu.modrm.disp.s32;
							}
						}
					}
				}
				
				/* */
				c->instr.cpu.operand_size = 0;
				
				if( c->cpu_instr_info->format.imm_data == II_IMM8 || c->cpu_instr_info->format.disp_data == II_DISP8 )
					c->instr.cpu.operand_size = OPSIZE_8;
				else if( c->cpu_instr_info->format.imm_data == II_IMM16 || c->cpu_instr_info->format.disp_data == II_DISP16 )
					c->instr.cpu.operand_size = OPSIZE_16;
				else if( c->cpu_instr_info->format.imm_data == II_IMM32 || c->cpu_instr_info->format.disp_data == II_DISP32 )
					c->instr.cpu.operand_size = OPSIZE_32;
				else if( c->cpu_instr_info->format.imm_data == II_IMM || c->cpu_instr_info->format.disp_data == II_DISPF
					 || (c->cpu_instr_info->format.type && !c->instr.cpu.modrm.opc))
				{
					if( c->cpu_instr_info->format.w_bit == 1 && c->instr.cpu.w_bit == 0 )
						c->instr.cpu.operand_size = OPSIZE_8;
					else
					{
						if( c->instr.cpu.prefixes & PREFIX_OPSIZE )
							c->instr.cpu.operand_size = OPSIZE_16;
						else
							c->instr.cpu.operand_size = OPSIZE_32;
					}
				}
				
				/* imm */
				if( c->cpu_instr_info->format.imm_data != 0 || (c->cpu_instr_info->format.type && !c->instr.cpu.modrm.opc))
				{
					if( c->instr.cpu.operand_size == OPSIZE_32 )
					{
						ret = emu_memory_read_dword(c->mem, c->eip, &c->instr.cpu.imm);
						c->eip += 4;
					}
					else if( c->instr.cpu.operand_size == OPSIZE_8 )
					{
						ret = emu_memory_read_byte(c->mem, c->eip++, c->instr.cpu.imm8);
					}
					else if( c->instr.cpu.operand_size == OPSIZE_16 )
					{
						ret = emu_memory_read_word(c->mem, c->eip, c->instr.cpu.imm16);
						c->eip += 2;
					}
	
					if( ret != 0 )
						return ret;
				}
				
				/* disp */
				if( c->cpu_instr_info->format.disp_data != 0 )
				{
					if( c->instr.cpu.operand_size == OPSIZE_32 )
					{
						uint32_t disp32;
						ret = emu_memory_read_dword(c->mem, c->eip, &disp32);
						c->instr.cpu.disp = (int32_t)disp32;
						c->eip += 4;
					}
					else if( c->instr.cpu.operand_size == OPSIZE_16 )
					{
						uint16_t disp16;
						ret = emu_memory_read_word(c->mem, c->eip, &disp16);
						c->instr.cpu.disp = (int16_t)disp16;
						c->eip += 2;
					}
					else if( c->instr.cpu.operand_size == OPSIZE_8 )
					{
						uint8_t disp8;
						ret = emu_memory_read_byte(c->mem, c->eip++, &disp8);
						c->instr.cpu.disp = (int8_t)disp8;
					}
	
					if( ret != 0 )
						return ret;
				}
				
				/* TODO level type ... */
			}
			else /* fpu */
			{
				/* this is a minimal parser without exact decomposition
				 * into all fields. instead it determines the length of
				 * the instruction and ignores pretty much everything else
				 * except for a few explicitly implemented instructions. */
				c->instr.is_fpu = 1;
				c->instr.fpu.prefixes = c->instr.prefixes;

				c->instr.fpu.fpu_data[0] = c->instr.opc;

				ret = emu_memory_read_byte(c->mem, c->eip++, &c->instr.fpu.fpu_data[1]);
				if( ret != 0 )
					return ret;
				
				if( FPU_MOD(c->instr.fpu.fpu_data) != 3 ) /* intel pdf page 36 */
				{
					/* trivial case, one register is ea */
		   			if( FPU_RM(c->instr.fpu.fpu_data) != 4 && !(FPU_MOD(c->instr.fpu.fpu_data) == 0 && FPU_RM(c->instr.fpu.fpu_data) == 5) )
						c->instr.fpu.ea = c->reg[FPU_RM(c->instr.fpu.fpu_data)];
					else
						c->instr.fpu.ea = 0;
					
					/* sib byte */
					if( FPU_RM(c->instr.fpu.fpu_data) == 4 )
					{
						ret = emu_memory_read_byte(c->mem, c->eip++, &byte);
			
						if( ret != 0 )
							return ret;
						
						if( SIB_BASE(byte) != 5 )
						{
							c->instr.fpu.ea += c->reg[SIB_BASE(byte)];
						}
						else if( FPU_MOD(c->instr.fpu.fpu_data) != 0 )
						{
							c->instr.fpu.ea += c->reg[ebp];
						}

						if( SIB_INDEX(byte) != 4 )
						{
							c->instr.fpu.ea += c->reg[SIB_INDEX(byte)] * scalem[SIB_SCALE(byte)];
						}
					}
					
					/* modrm */
					if( FPU_MOD(c->instr.fpu.fpu_data) == 1 )
					{
						ret = emu_memory_read_byte(c->mem, c->eip++, &byte);

						if( ret != 0 )
							return ret;
						
						c->instr.fpu.ea += (int8_t)byte;
					}
					else if( FPU_MOD(c->instr.fpu.fpu_data) == 2 || (FPU_MOD(c->instr.fpu.fpu_data) == 0 && FPU_RM(c->instr.fpu.fpu_data) == 5) ) 
					{
						uint32_t dword;
						ret = emu_memory_read_dword(c->mem, c->eip, &dword);
						c->eip += 4;

						if( ret != 0 )
							return ret;

						c->instr.fpu.ea += dword;
					}
				}
				
				/*c->instr.fpu.last_instr = c->last_fpu_instr;*/
				
				
				c->last_fpu_instr[1] = c->last_fpu_instr[0]; 
				c->last_fpu_instr[0] = eip_before;
			}
			
//			logDebug(c->emu,"\n");

			eip_after = c->eip;
			if ( CPU_DEBUG_FLAG_ISSET(c, instruction_size ) && eip_after - eip_before != expected_instr_size)
			{
				logDebug(c->emu, "broken instr.cpu size %i %i\n",
					   eip_after - eip_before,
					   expected_instr_size);
				return -1;
			}


			/* the default normal position is behind the instruction, specific instructions as call jmp set their
			 * norm position 
			 */
			if ( c->instr.is_fpu == 0 )
			{
				SOURCE_NORM_POS(c->instr, c->eip);
			}
			else
			{
				SOURCE_NORM_POS(c->instr, c->eip);
			}


			break;
		}
	}
	
	return 0;
}

int32_t emu_cpu_step(struct emu_cpu *c)
{
	int32_t ret = 0;

	/* call the function */
	if( c->instr.is_fpu == 0 )
	{
		if( c->instr.cpu.prefixes & PREFIX_FS_OVR )
		{
			emu_memory_segment_select(c->mem, s_fs);
		}

		ret = c->cpu_instr_info->function(c, &c->instr.cpu);

		if( c->instr.cpu.prefixes & PREFIX_FS_OVR )
		{
			emu_memory_segment_select(c->mem, s_cs);
		}
	}
	else
	{
		/* for now we only support three critical instructions. */
		if( c->instr.fpu.fpu_data[0] == 0xd9 )
		{
			if( (c->instr.fpu.fpu_data[1] & 0x38) == 0x30 )
			{
				/* fnstenv volume 1, page 230 */
				static int null = 0;
				MEM_DWORD_WRITE(c, c->instr.fpu.ea + 0x00, null);
				MEM_DWORD_WRITE(c, c->instr.fpu.ea + 0x04, null);
				MEM_DWORD_WRITE(c, c->instr.fpu.ea + 0x08, null);
				MEM_DWORD_WRITE(c, c->instr.fpu.ea + 0x0c, c->last_fpu_instr[1]);
				MEM_DWORD_WRITE(c, c->instr.fpu.ea + 0x10, null);
				MEM_DWORD_WRITE(c, c->instr.fpu.ea + 0x14, null);
				MEM_DWORD_WRITE(c, c->instr.fpu.ea + 0x18, null);

				TRACK_NEED_FPU(c->instr, TRACK_FPU_LAST_INSTRUCTION);
//				TRACK_INIT_FPU(c->instr, TRACK_FPU_LAST_INSTRUCTION);
			}
			else if( c->instr.fpu.fpu_data[1] == 0xee )
			{
				/* fldz */
				TRACK_INIT_FPU(c->instr, TRACK_FPU_LAST_INSTRUCTION);
			}
			else
			{ // catch all others to init fpu
				TRACK_INIT_FPU(c->instr, TRACK_FPU_LAST_INSTRUCTION);
			}

		}
		else if( c->instr.fpu.fpu_data[0] == 0xdd )
		{
			if( (c->instr.fpu.fpu_data[1] & 0xf8) == 0xc0 )
			{
				/* ffree */
				TRACK_INIT_FPU(c->instr, TRACK_FPU_LAST_INSTRUCTION);
			}
		}
	}


	if (0)
		debug_instruction(&c->instr);
//	emu_cpu_debug_print(c);

	return ret;
}

int32_t emu_cpu_run(struct emu_cpu *c)
{
	int steps=0;
	while (emu_cpu_parse(c) == 0)
	{
//		printf("%s \n", c->instr_string);

		if ( emu_cpu_step(c) != 0 )
			break;

		steps++;
	}
//	printf("%s \n", emu_strerror(c->emu));
	return steps;
}

void emu_cpu_debugflag_set(struct emu_cpu *c, uint8_t flag)
{
	CPU_DEBUG_FLAG_SET(c, flag);
}

void emu_cpu_debugflag_unset(struct emu_cpu *c, uint8_t flag)
{
	CPU_DEBUG_FLAG_UNSET(c, flag);	
}

