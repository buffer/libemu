/* @header@ */
#include <stdint.h>
#include <errno.h>

#include "emu/emu.h"
#include "emu/emu_cpu.h"
#include "emu/emu_cpu_data.h"
#include "emu/emu_cpu_functions.h"
#include "emu/emu_memory.h"


#define PUSH_DWORD_TO_STACK(cpu, arg)				\
if (cpu->reg[esp] < 4)								\
{													\
	emu_errno_set((cpu)->emu, ENOMEM);				\
	emu_strerror_set((cpu)->emu,					\
	"ran out of stack space writing a dword\n");	\
	return -1;										\
}													\
emu_memory_write_dword(cpu->mem, cpu->reg[esp], arg);\
cpu->reg[esp]-=4;											


#define PUSH_WORD_TO_STACK(cpu, arg)				\
if (cpu->reg[esp] < 2)								\
{													\
	emu_errno_set((cpu)->emu, ENOMEM);				\
	emu_strerror_set((cpu)->emu,					\
	"ran out of stack space writing a word\n");		\
	return -1;										\
}													\
emu_memory_write_word(cpu->mem, cpu->reg[esp], arg);\
cpu->reg[esp]-=2;											


#define PUSH_BYTE_TO_STACK(cpu, arg)				\
if (cpu->reg[esp] < 1)								\
{													\
	emu_errno_set((cpu)->emu, ENOMEM);				\
	emu_strerror_set((cpu)->emu,					\
	"ran out of stack space writing a byte\n");		\
	return -1;										\
}													\
emu_memory_write_byte(cpu->mem, cpu->reg[esp], arg);\
cpu->reg[esp]-=1;											




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
		PUSH_WORD_TO_STACK(c, *c->reg16[i->opc & 7])
	}else
	{
        /* 50+rd 
		 * Push r32      
		 * PUSH r32   
		 */
		PUSH_DWORD_TO_STACK(c, c->reg[i->opc & 7])
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
		PUSH_WORD_TO_STACK(c, *i->imm16)
	} else
	{
		/* 68    
		 * Push imm32    
		 * PUSH imm32 
		 */
		PUSH_DWORD_TO_STACK(c, i->imm)
	}

	return 0;
}




int32_t instr_push_6a(struct emu_cpu *c, struct instruction *i)
{
	/* 6A    
	 * Push imm8     
	 * PUSH imm8  
	 */
	PUSH_BYTE_TO_STACK(c, *i->imm8);

	return 0;
}




int32_t instr_push_ff(struct emu_cpu *c, struct instruction *i)
{
	if ( i->prefixes & PREFIX_OPSIZE )
	{

		/* FF /6 
		 * Push r/m16    
		 * PUSH r/m16 
		 */

	} else
	{


		/* FF /6 
		 * Push r/m32    
		 * PUSH r/m32 
		 */
	}
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

