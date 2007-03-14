/* @header@ */
#include <malloc.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdint.h>

#include <emu/emu_cpu.h>
#include <emu/emu_cpu_data.h>
#include <emu/emu_memory.h>
#include <emu/emu.h>
#include <emu/emu_log.h>

static const char *regm[] = {
	"eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi"
};

static uint8_t scalem[] = {
	1, 2, 4, 8
};


static uint16_t prefix_map[0x100];

#include <emu/emu_cpu_itables.h>

static void init_prefix_map()
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
}

struct emu_cpu *emu_cpu_new(struct emu *e)
{
	struct emu_cpu *c = malloc(sizeof(struct emu_cpu));
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
	}
	
	
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
}

uint32_t emu_cpu_eip_get(struct emu_cpu *c)
{
	return c->eip;
}

void emu_cpu_free(struct emu_cpu *c)
{
	free(c);
}

void emu_cpu_debug_print(struct emu_cpu *c)
{
	logDebug(c->emu,"cpu state    eip=0x%08x\n", c->eip);
	logDebug(c->emu,"eax=0x%08x  ecx=0x%08x  edx=0x%08x  ebx=0x%08x\n", c->reg[eax], c->reg[ecx], c->reg[edx], c->reg[ebx]);
	logDebug(c->emu,"esp=0x%08x  ebp=0x%08x  esi=0x%08x  edi=0x%08x\n", c->reg[esp], c->reg[ebp], c->reg[esi], c->reg[edi]);

	                      /* 0     1     2     3      4       5       6     7 */
	const char *flags[] = { "CF", "  ", "PF", "  " , "AF"  , "    ", "ZF", "SF", 
	                        "TF", "IF", "DF", "OF" , "IOPL", "IOPL", "NT", "  ",
	                        "RF", "VM", "AC", "VIF", "RIP" , "ID"  , "  ", "  ",
	                        "  ", "  ", "  ", "   ", "    ", "    ", "  ", "  "};

	char *fmsg;
	fmsg = (char *)malloc(32*3+1);
	memset(fmsg, 0, 32*3+1);
	int i;
	for ( i=0;i<32;i++ )
	{
		if ( CPU_FLAG_ISSET(c, i) )
		{
			strcat(fmsg, flags[i]);
			strcat(fmsg," ");
		}
	}
	logDebug(c->emu,"Flags: %s\n", fmsg);
	free(fmsg);


	uint32_t stacksize = 4294967295U - c->reg[esp];
//	printf("stacksize is %i\n",stacksize);
	if (stacksize == 0)
		return;

/*
	uint8_t *b = (uint8_t *)malloc(stacksize);
	memset(b,0,stacksize);
	logDebug(c->emu,"Stack (%i):\n",stacksize);

	for ( i=0;i<stacksize;i++ )
	{
		if (emu_memory_read_byte(c->mem,c->reg[esp]+i,&b[stacksize-i-1]) != 0)
			break;
	}


	int numlines = (stacksize / 16);
//	printf("numlines  is %i\n",numlines);
	fmsg = (char *)malloc(256);

	for (i=0;i<=numlines;i++)
	{
    	memset(fmsg,0,256);
		int j;
		for ( j=0;j<16 && i*16+j < stacksize ;j++ )
		{
			char sign[5];
			snprintf(sign,4,"%02x ",b[j+i*16]);
			strcat(fmsg,sign);
		}
		logDebug(c->emu,"0x%08x:  %s\n",c->reg[esp]+i*16,fmsg);
	}

	free(fmsg);
*/
}

static void debug_instruction(struct emu_cpu_instruction *i)
{
	struct instruction_info *ii;
	
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
}

#undef PREFIX_LOCK

#include "libdasm.h"
static uint32_t dasm_print_instruction(uint8_t *data, uint32_t size)
{
	INSTRUCTION inst;
	static char str[256];

	// step 2: fetch instruction
	uint32_t instrsize = get_instruction(&inst, data, MODE_32);

	// step 3: print it
	get_instruction_string(&inst, FORMAT_INTEL, 0, str, sizeof(str));
	printf("%s\n", str);
	return instrsize;
}

int32_t emu_cpu_step(struct emu_cpu *c)
{
	/* TODO make unstatic for threadsafety */
	static uint8_t byte;
	static uint8_t *opcode;
	static uint32_t ret;
	static struct emu_cpu_instruction i;
	static struct instruction_info *ii;
	
	i.prefixes = 0;
	
	/* TODO move to somewhere else */
	ret = 1;
	if( *((uint8_t *)&ret) == 1 )
	{
		/* le */
		i.imm16 = (uint16_t *)((void *)&i.imm);
		i.imm8 = (uint8_t *)&i.imm;
	}
	else
	{
		/* be */
		i.imm16 = (uint16_t *)((void *)&i.imm + 1);
		i.imm8 = (uint8_t *)&i.imm + 3;
	}
	
	logDebug(c->emu,"decoding\n");
	emu_cpu_debug_print(c);

	uint8_t dis[32];
	emu_memory_read_block(c->mem,c->eip,dis,32);
	printf("%08x ",c->eip);
	uint32_t expected_instr_size = dasm_print_instruction(dis,0);

	uint32_t eip_before = c->eip;
	uint32_t eip_after = 0;

	while( 1 )
	{
		
		ret = emu_memory_read_byte(c->mem, c->eip++, &byte);
		
		if( ret != 0 )
			return ret;
		
		ii = &ii_onebyte[byte];

		if( ii->function == prefix_fn )
		{
			i.prefixes |= prefix_map[byte];
			continue;
		}
		else
		{
			i.opc = byte;
			
			if( i.opc == 0x0f )
			{
				ret = emu_memory_read_byte(c->mem, c->eip++, &byte);
		
				if( ret != 0 )
					return ret;
					
				i.opc_2nd = byte;
				opcode = &i.opc_2nd;
				ii = &ii_twobyte[byte];
			}
			else
			{
				opcode = &i.opc;
			}
			
			if ( ii->function == 0 )
			{
				emu_strerror_set(c->emu,"opcode %02x not supported\n", i.opc);
				emu_errno_set(c->emu, ENOTSUP);
				int y=0;
				for (y=0;y<expected_instr_size;y++)
				{
					printf("%02x ", dis[y]);
				}
				printf("\n");
				return -1;
			}
			
			i.w_bit = *opcode & 1;
			i.s_bit = (*opcode >> 1) & 1;

			/* mod r/m byte?  sib/disp */
			if( ii->format.modrm_byte != 0 )
			{
				ret = emu_memory_read_byte(c->mem, c->eip++, &byte);
		
				if( ret != 0 )
					return ret;
					
				i.modrm.mod = MODRM_MOD(byte);
				i.modrm.opc = MODRM_REGOPC(byte);
				i.modrm.rm = MODRM_RM(byte);
				
				if( ii->format.modrm_byte == II_MOD_REG_RM || ii->format.modrm_byte == II_MOD_YYY_RM ||
					ii->format.modrm_byte == II_XX_REG1_REG2) /* cases with possible sib/disp*/
				{
					if( i.modrm.mod != 3 )
					{
						if( i.modrm.rm != 4 && !(i.modrm.mod == 0 && i.modrm.rm == 5) )
							i.modrm.ea = c->reg[i.modrm.rm];
						else
							i.modrm.ea = 0;
						
						if( i.modrm.rm == 4 ) /* sib byte present */
						{
							ret = emu_memory_read_byte(c->mem, c->eip++, &byte);
		
							if( ret != 0 )
								return ret;
								
							i.modrm.sib.base = SIB_BASE(byte);
							i.modrm.sib.scale = SIB_SCALE(byte);
							i.modrm.sib.index = SIB_INDEX(byte);
							
							if( i.modrm.sib.base != 5 )
							{
								i.modrm.ea += c->reg[i.modrm.sib.base];
							}
							else if( i.modrm.mod != 0 )
							{
								i.modrm.ea += c->reg[ebp];
							}

							if( i.modrm.sib.index != 4 )
							{
								i.modrm.ea += c->reg[i.modrm.sib.index] * scalem[i.modrm.sib.scale];
							}
						}
						
						if( i.modrm.mod == 1 ) /* disp8 */
						{
							ret = emu_memory_read_byte(c->mem, c->eip++, &i.modrm.disp.s8);
		
							if( ret != 0 )
								return ret;
							
							i.modrm.ea += i.modrm.disp.s8;
						}
						else if( i.modrm.mod == 2 || (i.modrm.mod == 0 && i.modrm.rm == 5) ) /* disp32 */
						{
							ret = emu_memory_read_dword(c->mem, c->eip, &i.modrm.disp.s32);
							c->eip += 4;
		
							if( ret != 0 )
								return ret;

							i.modrm.ea += i.modrm.disp.s32;
						}
					}
				}
			}
			
			/* */
			i.operand_size = 0;
			
			if( ii->format.imm_data == II_IMM8 || ii->format.disp_data == II_DISP8 )
				i.operand_size = OPSIZE_8;
			else if( ii->format.imm_data == II_IMM16 || ii->format.disp_data == II_DISP16 )
				i.operand_size = OPSIZE_16;
			else if( ii->format.imm_data == II_IMM32 || ii->format.disp_data == II_DISP32 )
				i.operand_size = OPSIZE_32;
			else if( ii->format.imm_data == II_IMM || ii->format.disp_data == II_DISPF )
			{
				if( ii->format.w_bit == 1 && i.w_bit == 0 )
					i.operand_size = OPSIZE_8;
				else
				{
					if( i.prefixes & PREFIX_OPSIZE )
						i.operand_size = OPSIZE_16;
					else
						i.operand_size = OPSIZE_32;
				}
			}
			
			/* imm */
			if( ii->format.imm_data != 0 )
			{
				if( i.operand_size == OPSIZE_32 )
				{
					ret = emu_memory_read_dword(c->mem, c->eip, &i.imm);
					c->eip += 4;
				}
				else if( i.operand_size == OPSIZE_8 )
				{
					ret = emu_memory_read_byte(c->mem, c->eip++, i.imm8);
				}
				else if( i.operand_size == OPSIZE_16 )
				{
					ret = emu_memory_read_word(c->mem, c->eip, i.imm16);
					c->eip += 2;
				}

				if( ret != 0 )
					return ret;
			}
			
			/* disp */
			if( ii->format.disp_data != 0 )
			{
				if( i.operand_size == OPSIZE_32 )
				{
					uint32_t disp32;
					ret = emu_memory_read_dword(c->mem, c->eip, &disp32);
					i.disp = (int32_t)disp32;
					c->eip += 4;
				}
				else if( i.operand_size == OPSIZE_16 )
				{
					uint16_t disp16;
					ret = emu_memory_read_word(c->mem, c->eip, &disp16);
					i.disp = (int16_t)disp16;
					c->eip += 2;
				}
				else if( i.operand_size == OPSIZE_8 )
				{
					uint8_t disp8;
					ret = emu_memory_read_byte(c->mem, c->eip++, &disp8);
					i.disp = (int8_t)disp8;
				}

				if( ret != 0 )
					return ret;
			}
			
			/* TODO level type ... */

			eip_after = c->eip;

			if (eip_after - eip_before != expected_instr_size)
			{
				printf("broken instr size %i %i\n",
					   eip_after - eip_before,
					   expected_instr_size);
				return -1;
			}

			if( i.prefixes & PREFIX_FS_OVR )
			{
				emu_memory_segment_select(c->mem, s_fs);
			}

			/* call the function */
			ret = ii->function(c, &i);
			
			if( i.prefixes & PREFIX_FS_OVR )
			{
				emu_memory_segment_select(c->mem, s_cs);
			}

			if (0)
				debug_instruction(&i);
			emu_cpu_debug_print(c);
			if ( ret != 0 )
				return ret;
            			
			break;
		}
		
		logDebug(c->emu,"\n");
	}
	
	return 0;
}

int32_t emu_cpu_run(struct emu_cpu *c)
{
	return emu_cpu_step(c);
}


