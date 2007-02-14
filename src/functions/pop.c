/* @header@ */
#include <stdint.h>

#include "emu/emu_cpu.h"
#include "emu/emu_cpu_data.h"
#include "emu/emu_cpu_functions.h"
#include "emu/emu_memory.h"


int32_t instr_pop_07(struct emu_cpu *c, struct instruction *i)
{
	/* 07     
	 * Pop top of stack into ES; increment stack pointer   
	 * POP ES  
	 */


	return 0;
}

int32_t instr_pop_17(struct emu_cpu *c, struct instruction *i)
{

	/* 17     
	 * Pop top of stack into SS; increment stack pointer   
	 * POP SS  
	 */


	return 0;
}

int32_t instr_pop_1f(struct emu_cpu *c, struct instruction *i)
{

	/* 1F     
	 * Pop top of stack into DS; increment stack pointer   
	 * POP DS  
	 */


	return 0;
}

int32_t instr_pop_5x(struct emu_cpu *c, struct instruction *i)
{

	/* 58+ rd 
	 * Pop top of stack into r32; increment stack pointer  
	 * POP r32 
	 */


	/* 58+ rw 
	 * Pop top of stack into r16; increment stack pointer  
	 * POP r16 
	 */


	return 0;
}

int32_t instr_pop_8f(struct emu_cpu *c, struct instruction *i)
{

	/* 8F /0  
	 * Pop top of stack into m16; increment stack pointer  
	 * POP m16 
	 */


	/* 8F /0  
	 * Pop top of stack into m32; increment stack pointer  
	 * POP m32 
	 */																   


	return 0;
}

int32_t instr_pop_0fa1(struct emu_cpu *c, struct instruction *i)
{

	/* 0F A1  
	 * Pop top of stack into FS; increment stack pointer   
	 * POP FS  
	 */


	return 0;
}

int32_t instr_pop_0fa9(struct emu_cpu *c, struct instruction *i)
{

	/* 0F A9  
	 * Pop top of stack into GS; increment stack pointer   
	 * POP GS  
	 */


	return 0;
}

