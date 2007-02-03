#include <malloc.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include <emu/emu_cpu.h>
#include <emu/emu_memory.h>


static const char *regm[] = {
	"eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi"
};

static uint8_t scalem[] = {
	1, 2, 4, 8
};


struct emu_cpu
{
	struct emu *emu;
	struct emu_memory *mem;
	
	uint32_t eip;
	uint32_t regs[8];
};

struct instruction
{
	uint8_t opc;
	uint8_t opc_2nd;
	uint8_t prefixes;
	uint8_t s_bit : 1;
	uint8_t w_bit : 1;
	uint8_t operand_size : 2;

	struct /* mod r/m data */
	{
		union
		{
			uint8_t mod : 2;
			uint8_t x : 2;
		};

		union
		{
			uint8_t reg1 : 3;
			uint8_t opc : 3;
			uint8_t sreg3 : 3;
			uint8_t y : 3;
		};

		union
		{
			uint8_t reg : 3;
			uint8_t reg2 : 3;
			uint8_t rm : 3;
			uint8_t z : 3;
		};

		struct
		{
			uint8_t scale : 2;
			uint8_t index : 3;
			uint8_t base : 3;
		} sib;

		union
		{
			uint8_t s8;
			uint16_t s16;
			uint32_t s32;
		} disp;
		
		uint32_t ea;
	} modrm;

	uint32_t imm;
	uint32_t disp;
};

#define MODRM_MOD(x) (((x) >> 6) & 3)
#define MODRM_REGOPC(x) (((x) >> 3) & 7)
#define MODRM_RM(x) ((x) & 7)
    
#define SIB_SCALE(x) (((x) >> 6) & 3)
#define SIB_INDEX(x) (((x) >> 3) & 7)
#define SIB_BASE(x) ((x) & 3)

#define PREFIX_ADSIZE (1 << 0)
#define PREFIX_OPSIZE (1 << 1)
#define PREFIX_LOCK (1 << 2)
#define PREFIX_CS_OVR (1 << 3)
#define PREFIX_DS_OVR (1 << 4)
#define PREFIX_ES_OVR (1 << 5)
#define PREFIX_FS_OVR (1 << 6)
#define PREFIX_GS_OVR (1 << 7)
#define PREFIX_SS_OVR (1 << 8)

#define OPSIZE_8 1
#define OPSIZE_16 2
#define OPSIZE_32 3

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
	memset((void *)c, 0, sizeof(c));
	
	c->emu = e;
	c->mem = emu_memory_get(e);
	
	memset((void *)c->regs, 0, sizeof(c->regs)); 

	init_prefix_map();
	
	return c;
}

void emu_cpu_register_set(struct emu_cpu *c, enum emu_reg r, uint32_t val)
{
	c->regs[r] = val;
}

uint32_t emu_cpu_register_get(struct emu_cpu *c, enum emu_reg r)
{
	return c->regs[r];
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

static void debug_cpu(struct emu_cpu *c)
{
	int i;
	
	printf("cpu state    eip=0x%08x\n", c->eip);
	
	for( i = 0; i < 4; i++ )
	{
		printf("%s=0x%08x  ", regm[i], c->regs[i]);
	} 
	printf("\n");
	for( i = 4; i < 8; i++ )
	{
		printf("%s=0x%08x  ", regm[i], c->regs[i]);
	} 
	printf("\n");
}

static void debug_instruction(struct instruction *i)
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

int32_t emu_cpu_step(struct emu_cpu *c)
{
	static uint8_t byte;
	static uint8_t *opcode;
	static uint32_t ret;
	static struct instruction i;
	static struct instruction_info *ii;
	
	i.prefixes = 0;
	
	printf("decoding\n");
	
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
				emu_errno_set(c->emu,ENOTSUP);
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
							i.modrm.ea = c->regs[i.modrm.rm];
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
								i.modrm.ea += c->regs[i.modrm.sib.base];
							}
							else if( i.modrm.mod != 0 )
							{
								i.modrm.ea += c->regs[ebp];
							}

							if( i.modrm.sib.index != 4 )
							{
								i.modrm.ea += c->regs[i.modrm.sib.index] * scalem[i.modrm.sib.scale];
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
					uint32_t imm32;
					ret = emu_memory_read_dword(c->mem, c->eip, &imm32);
					i.imm = imm32;
					c->eip += 4;
				}
				else if( i.operand_size == OPSIZE_8 )
				{
					uint8_t imm8;
					ret = emu_memory_read_byte(c->mem, c->eip++, &imm8);
					i.imm = imm8;
				}
				else if( i.operand_size == OPSIZE_16 )
				{
					uint16_t imm16;
					ret = emu_memory_read_word(c->mem, c->eip, &imm16);
					i.imm = imm16;
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
					i.disp = disp32;
					c->eip += 4;
				}
				else if( i.operand_size == OPSIZE_16 )
				{
					uint16_t disp16;
					ret = emu_memory_read_word(c->mem, c->eip, &disp16);
					i.disp = disp16;
					c->eip += 2;
				}
				else if( i.operand_size == OPSIZE_8 )
				{
					uint8_t disp8;
					ret = emu_memory_read_byte(c->mem, c->eip++, &disp8);
					i.disp = disp8;
				}

				if( ret != 0 )
					return ret;
			}
			
			/* TODO level type ... */

			/* call the function */			
			//ii->function(c, &i);
			debug_instruction(&i);
			debug_cpu(c);
			
			break;
		}
		
		printf("\n");
	}
	
	return 0;
}

int32_t emu_cpu_run(struct emu_cpu *c)
{
	return emu_cpu_step(c);
}

