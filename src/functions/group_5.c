/* @header@ */
#include <stdint.h>
#include <errno.h>

#include "emu/emu.h"
#include "emu/emu_cpu.h"
#include "emu/emu_cpu_data.h"
#include "emu/emu_cpu_functions.h"
#include "emu/emu_cpu_stack.h"
#include "emu/emu_memory.h"



/* TODO */
static int32_t instr_group_5_ff_call(struct emu_cpu *c, struct instruction *i)
{

	/* FF /2  
	 * CALL r/m16    
	 * Call near, absolute indirect, address given in r/m16   
	 */

	/* FF /2  
	 * CALL r/m32    
	 * Call near, absolute indirect, address given in r/m32   
	 */

	/* FF /3  
	 * CALL m16:16   
	 * Call far, absolute indirect, address given in m16:16  
	 */

	/* FF /3  
	 * CALL m16:32   
	 * Call far, absolute indirect, address given in m16:32  
	 */

	if( i->modrm.opc == 2 )
	{
		PUSH_DWORD(c, c->eip);
		
		if( i->modrm.mod != 3 )
		{
			if( i->prefixes & PREFIX_OPSIZE )
			{
				uint16_t disp;
				MEM_WORD_READ(c, i->modrm.ea, &disp);
				
				c->eip = disp;
			}
			else
			{
				uint32_t disp;
				MEM_DWORD_READ(c, i->modrm.ea, &disp);
				
				c->eip = disp;
			}
		}
		else
		{
			if( i->prefixes & PREFIX_OPSIZE )
			{
				c->eip = *c->reg16[i->modrm.rm];
			}
			else
			{
				c->eip = c->reg[i->modrm.rm];
			}
		}
	}
	else
	{
		return -1;
	}

	return 0;
}



static int32_t instr_group_5_ff_jmp(struct emu_cpu *c, struct instruction *i)
{

	/* FF /4  
	 * Jump near, absolute indirect, address given in r/m16                   
	 * JMP r/m16    
	 */


	/* FF /4  
	 * Jump near, absolute indirect, address given in r/m32                   
	 * JMP r/m32    
	 */

	/* FF /5  
	 * Jump far, absolute indirect, address given in m16:16                   
	 * JMP m16:16   
	 */


	/* FF /5  
	 * Jump far, absolute indirect, address given in m16:32                   
	 * JMP m16:32   
	 */

	if( i->modrm.opc == 4 )
	{
		if( i->modrm.mod != 3 )
		{
			if( i->prefixes & PREFIX_OPSIZE )
			{
				uint16_t disp;
				MEM_WORD_READ(c, i->modrm.ea, &disp);
				
				c->eip = disp;
			}
			else
			{
				uint32_t disp;
				MEM_DWORD_READ(c, i->modrm.ea, &disp);
				
				c->eip = disp;
			}
		}
		else
		{
			if( i->prefixes & PREFIX_OPSIZE )
			{
				c->eip = *c->reg16[i->modrm.rm];
			}
			else
			{
				c->eip = c->reg[i->modrm.rm];
			}
		}
	}
	else /* /5 */
	{
		/* unneeded */
		return -1;
	}

	return 0;
}


/* TODO */
static int32_t instr_group_5_ff_push(struct emu_cpu *c, struct instruction *i)
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


static int32_t (*group_5_fn[8])(struct emu_cpu *c, struct instruction *i) = {
	/* 0 */ instr_group_5_ff_inc,
	/* 1 */ instr_group_5_ff_dec,
	/* 2 */ instr_group_5_ff_call,
	/* 3 */ instr_group_5_ff_call,
	/* 4 */ instr_group_5_ff_jmp,
	/* 5 */ instr_group_5_ff_jmp,
	/* 6 */ instr_group_5_ff_push,
	/* 7 */ 0,
};

int32_t instr_group_5_ff(struct emu_cpu *c, struct instruction *i)
{
	return group_5_fn[i->modrm.opc](c, i);
}

