/* @header@ */
#include <stdint.h>
#include <errno.h>

#include "emu/emu.h"
#include "emu/emu_cpu.h"
#include "emu/emu_cpu_data.h"
#include "emu/emu_cpu_functions.h"
#include "emu/emu_cpu_stack.h"
#include "emu/emu_memory.h"



int32_t instr_push_06(struct emu_cpu *c, struct instruction *i)
{
	/* 06    
	 * Push ES		
	 * PUSH ES
	 */

	return 0;
}


int32_t instr_push_0e(struct emu_cpu *c, struct instruction *i)
{
	/* 0E    
	 * Push CS       
	 * PUSH CS    
	 */


	return 0;
}




int32_t instr_push_16(struct emu_cpu *c, struct instruction *i)
{
	/* 16    
	 * Push SS       
	 * PUSH SS    
	 */


	return 0;
}




int32_t instr_push_1e(struct emu_cpu *c, struct instruction *i)
{
	/* 1E    
	 * Push DS       
	 * PUSH DS    
	 */


	return 0;
}




int32_t instr_push_5x(struct emu_cpu *c, struct instruction *i)
{
	if ( i->prefixes & PREFIX_OPSIZE )
	{
		/* 50+rw 
		 * Push r16      
		 * PUSH r16   
		 */
		PUSH_WORD(c, *c->reg16[i->opc & 7])
	}else
	{
        /* 50+rd 
		 * Push r32      
		 * PUSH r32   
		 */
		PUSH_DWORD(c, c->reg[i->opc & 7])
	}
		


	return 0;
}




int32_t instr_push_68(struct emu_cpu *c, struct instruction *i)
{
	if ( i->prefixes & PREFIX_OPSIZE )
	{

		/* 68    
		 * Push imm16    
		 * PUSH imm16 
		 */
		PUSH_WORD(c, *i->imm16)
	} else
	{
		/* 68    
		 * Push imm32    
		 * PUSH imm32 
		 */
		PUSH_DWORD(c, i->imm)
	}

	return 0;
}




int32_t instr_push_6a(struct emu_cpu *c, struct instruction *i)
{
	/* 6A    
	 * Push imm8     
	 * PUSH imm8  
	 */
	PUSH_BYTE(c, *i->imm8);

	return 0;
}



int32_t instr_push_0fa0(struct emu_cpu *c, struct instruction *i)
{
	/* 0F A0 
	 * Push FS       
	 * PUSH FS    
	 */


	return 0;
}


int32_t instr_push_0f08(struct emu_cpu *c, struct instruction *i)
{
	/* 0F A8 
	 * Push GS       
	 * PUSH GS    
	 */


	return 0;
}

int32_t instr_pushad_60(struct emu_cpu *c, struct instruction *i)
{
	uint32_t j;
	
	if( i->prefixes & PREFIX_OPSIZE )
	{
		uint16_t temp = c->reg[sp];
		
		for( j = 0; j < 8; j++ )
		{
			if( j != 4 )
			{
				PUSH_WORD(c, c->reg[j]);
			}
			else
			{
				PUSH_WORD(c, temp);
			}
		}
	}
	else
	{
		uint32_t temp = c->reg[esp];
		
		for( j = 0; j < 8; j++ )
		{
			if( j != 4 )
			{
				PUSH_DWORD(c, c->reg[j]);
			}
			else
			{
				PUSH_DWORD(c, temp);
			}
		}
	}
	
	return 0;
}


