/* @header@ */
#include <stdint.h>

#include "emu/emu_cpu.h"
#include "emu/emu_cpu_data.h"
#include "emu/emu_cpu_functions.h"
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
	/* 50+rd 
	 * Push r32      
	 * PUSH r32   
	 */


	/* 50+rw 
	 * Push r16      
	 * PUSH r16   
	 */


	return 0;
}




int32_t instr_push_68(struct emu_cpu *c, struct instruction *i)
{
	/* 68    
	 * Push imm16    
	 * PUSH imm16 
	 */


	/* 68    
	 * Push imm32    
	 * PUSH imm32 
	 */


	return 0;
}




int32_t instr_push_6a(struct emu_cpu *c, struct instruction *i)
{
	/* 6A    
	 * Push imm8     
	 * PUSH imm8  
	 */


	return 0;
}




int32_t instr_push_ff(struct emu_cpu *c, struct instruction *i)
{
	/* FF /6 
	 * Push r/m16    
	 * PUSH r/m16 
	 */

	/* FF /6 
	 * Push r/m32    
	 * PUSH r/m32 
	 */

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

