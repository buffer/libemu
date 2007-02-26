/* @header@ */
#include <stdint.h>
#include <errno.h>

#include "emu/emu.h"
#include "emu/emu_cpu.h"
#include "emu/emu_cpu_data.h"
#include "emu/emu_cpu_functions.h"
#include "emu/emu_cpu_stack.h"
#include "emu/emu_memory.h"

/*Intel Architecture Software Developer's Manual Volume 2: Instruction Set Reference (24319102.PDF) page 728*/

int32_t instr_test_84(struct emu_cpu *c, struct instruction *i)
{
	/* 84 /r    
	 * AND r8 with r/m8; set SF, ZF, PF according to result
	 * TEST r/m8,r8     
	 */
	return 0;
}

int32_t instr_test_85(struct emu_cpu *c, struct instruction *i)
{

	/* 85 /r    
	 * AND r16 with r/m16; set SF, ZF, PF according to result
	 * TEST r/m16,r16   
	 */

	/* 85 /r    
	 * AND r32 with r/m32; set SF, ZF, PF according to result
	 * TEST r/m32,r32   
	 */
	return 0;
}

int32_t instr_test_a8(struct emu_cpu *c, struct instruction *i)
{
	/* A8 ib    
	 * AND imm8 with AL; set SF, ZF, PF according to result      
	 * TEST AL,imm8     
	 */
	return 0;
}

int32_t instr_test_a9(struct emu_cpu *c, struct instruction *i)
{
	/* A9 iw    
	 * AND imm16 with AX; set SF, ZF, PF according to result     
	 * TEST AX,imm16    
	 */

	/* A9 id    
	 * AND imm32 with EAX; set SF, ZF, PF according to result    
	 * TEST EAX,imm32   
	 */

	return 0;
}

int32_t instr_group_3_f6_test(struct emu_cpu *c, struct instruction *i)
{
	/* F6 /0 ib 
	 * AND imm8 with r/m8; set SF, ZF, PF according to result
	 * TEST r/m8,imm8   
	 */
	return 0;
}


int32_t instr_group_3_f7_test(struct emu_cpu *c, struct instruction *i)
{
	/* F7 /0 id 
	 * AND imm32 with r/m32; set SF, ZF, PF according to result
	 * TEST r/m32,imm32 
	 */

	/* F7 /0 iw 
	 * AND imm16 with r/m16; set SF, ZF, PF according to result
	 * TEST r/m16,imm16 
	 */
	return 0;
}

