/* @header@ */
#include <stdint.h>

#include "emu/emu_cpu.h"
#include "emu/emu_cpu_data.h"

#include "emu/emu_memory.h"

/*Intel Architecture Software Developer's Manual Volume 2: Instruction Set Reference (24319102.PDF) page 481*/

#include <ctype.h>
#include <stdio.h>

int32_t instr_movsx_0fbe(struct emu_cpu *c, struct emu_cpu_instruction *i)
{

	if ( i->modrm.mod != 3 )
	{
		if ( i->prefixes & PREFIX_OPSIZE )
		{
			/* 0F BE /r 
			 * Move byte to word with sign-extension
			 * MOVSX r16,m8  
			 */       
			uint8_t m8;
			MEM_BYTE_READ(c, i->modrm.ea, &m8);
			*c->reg16[i->modrm.opc] = (int8_t)m8;
		}
		else
		{
			/* 0F BE /r 
			 * Move byte to doubleword, sign-extension
			 * MOVSX r32,m8  
			 */       
			uint8_t m8;
			MEM_BYTE_READ(c, i->modrm.ea, &m8);
			c->reg[i->modrm.opc] = (int8_t)m8;
		}
	}
	else
	{
		if ( i->prefixes & PREFIX_OPSIZE )
		{
			/* 0F BE /r 
			 * Move byte to word with sign-extension
			 * MOVSX r16,r8  
			 */       
			*c->reg16[i->modrm.opc] = (int8_t)*c->reg8[i->modrm.rm];
		}
		else
		{
			/* 0F BE /r 
			 * Move byte to doubleword, sign-extension
			 * MOVSX r32,r8  
			 */       
			c->reg[i->modrm.opc] = (int8_t)*c->reg8[i->modrm.rm];
		}
	}
	return 0;
}

int32_t instr_movsx_0fbf(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	if ( i->modrm.mod != 3 )
	{
		/* 0F BF /r 
		 * Move word to doubleword, sign-extension
		 * MOVSX r32,m16 
		 */
		uint16_t m16;
		MEM_WORD_READ(c, i->modrm.ea, &m16);
		c->reg[i->modrm.opc] = (int16_t)m16;

	}
	else
	{
		/* 0F BF /r 
		 * Move word to doubleword, sign-extension
		 * MOVSX r32,r16 
		 */
		c->reg[i->modrm.opc] = (int16_t)*c->reg16[i->modrm.rm];
	}
	return 0;
}

