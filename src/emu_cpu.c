#include <malloc.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

#include <emu/emu_cpu.h>
#include <emu/emu_cpu_itables.h>
#include <emu/emu_memory.h>

struct emu_cpu
{
	struct emu *lib;
	struct emu_memory *mem;
	
	uint32_t eip;
	uint32_t regs[8];
};

struct instruction
{
	uint8_t opc;
	uint8_t opc_2nd;
	uint8_t prefixes;

	struct /* mod r/m data */
	{
		union
		{
			uint8_t mod : 2;
			uint8_t x : 2;
		};

		union
		{
			uint8_t reg : 3;
			uint8_t reg1 : 3;
			uint8_t opc : 3;
			uint8_t sreg3 : 3;
			uint8_t y : 3;
		};

		union
		{
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
	} modrm;

	union /* immediate */
    {
		uint8_t s8;
		uint16_t s16;
		uint32_t s32;
	} imm;

	union /* displacement (non-mod-r/m) */
	{
		uint8_t s8;
		uint16_t s16;
		uint32_t s32;
	} disp;
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

static uint16_t prefix_map[0x100];

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
	
	c->lib = e;
	c->mem = emu_memory_get(e);
	
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

uint32_t emu_cpu_step(struct emu_cpu *c)
{
	static uint8_t byte;
	static uint8_t *opcode;
	static uint32_t ret;
	static struct instruction i;
	static struct instruction_info *ii;
	
	i.prefixes = 0;
	
	while( 1 )
	{
		
		ret = emu_memory_read_byte(c->mem, c->eip++, &byte);
		
		if( ret != 0 )
			return ret;
			
		ii = &ii_onebyte[byte];

		if( ii->function == prefix_fn )
		{
			i.prefixes |= prefix_map[byte];
			printf("%s ", ii->name);
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
			
			if( ii->function == 0 )
			{
				printf("opcode %02x not supported", i.opc);
				exit(-1);
			}
			else
			{
				printf("%s ", ii->name);
			}

			/* mod r/m byte?  sib/disp */
			if( ii->format.modrm_byte != 0 )
			{
				ret = emu_memory_read_byte(c->mem, c->eip++, &byte);
		
				if( ret != 0 )
					return ret;
					
				i.modrm.mod = MODRM_MOD(byte);
				i.modrm.reg = MODRM_REGOPC(byte);
				i.modrm.rm = MODRM_RM(byte);
				
				if( ii->format.modrm_byte == II_MOD_REG_RM || ii->format.modrm_byte == II_MOD_YYY_RM ||
					ii->format.modrm_byte == II_XX_REG1_REG2) /* cases with possible sib/disp*/
				{
					if( i.modrm.mod != 3 )
					{
						if( i.modrm.rm == 4 ) /* sib byte present */
						{
							ret = emu_memory_read_byte(c->mem, c->eip++, &byte);
		
							if( ret != 0 )
								return ret;
								
							i.modrm.sib.base = SIB_BASE(byte);
							i.modrm.sib.scale = SIB_SCALE(byte);
							i.modrm.sib.index = SIB_INDEX(byte);
						}
						
						if( i.modrm.mod == 1 ) /* disp8 */
						{
							ret = emu_memory_read_byte(c->mem, c->eip++, &i.modrm.disp.s8);
		
							if( ret != 0 )
								return ret;
						}
						else if( i.modrm.mod == 2 || (i.modrm.mod == 0 && i.modrm.rm == 5) ) /* disp32 */
						{
							ret = emu_memory_read_dword(c->mem, c->eip, &i.modrm.disp.s32);
							c->eip += 4;
		
							if( ret != 0 )
								return ret;
						}
					}
				}
			}
			
			/* imm */
			if( ii->format.imm_data != 0 )
			{
				if( ii->format.imm_data == II_IMM )
				{
					/* TODO read correct size: w-bit/size-prefix */
				}
				else if( ii->format.imm_data == II_IMM8 )
				{
					ret = emu_memory_read_byte(c->mem, c->eip++, &i.imm.s8);
				}
				else if( ii->format.imm_data == II_IMM16 )
				{
					ret = emu_memory_read_word(c->mem, c->eip, &i.imm.s16);
					c->eip += 2;
				}
				else if( ii->format.imm_data == II_IMM32 )
				{
					ret = emu_memory_read_dword(c->mem, c->eip, &i.imm.s32);
					c->eip += 4;
				}

				if( ret != 0 )
					return ret;
			}
			
			/* disp */
			if( ii->format.disp_data != 0 )
			{
				if( (ii->format.disp_data == II_DISPF && i.prefixes & PREFIX_OPSIZE) ||
					ii->format.disp_data == II_DISP16 )
				{
					ret = emu_memory_read_word(c->mem, c->eip, &i.disp.s16);
					c->eip += 2;
				}
				else if( ii->format.disp_data == II_DISPF || ii->format.disp_data == II_DISP32 )
				{
					ret = emu_memory_read_dword(c->mem, c->eip, &i.disp.s32);
					c->eip += 4;
				}
				else if( ii->format.disp_data == II_DISP8 )
				{
					ret = emu_memory_read_byte(c->mem, c->eip++, &i.disp.s8);
				}

				if( ret != 0 )
					return ret;
			}
			
			/* TODO level type ... */

			/* call the function */			
			ii->function(c, &i);
			
			break;
		}
		
		printf("\n");
	}
	
	return 0;
}
