/* @header@ */
#include <stdint.h>

#include "emu/emu_cpu.h"
#include "emu/emu_cpu_data.h"
#include "emu/emu_cpu_functions.h"
#include "emu/emu_memory.h"


int32_t instr_jmp_e9(struct emu_cpu *c, struct emu_cpu_instruction *i)
{

	/* E9 cw  
	 * Jump near, relative, displacement relative to next instruction         
	 * JMP rel16    
	 */


	/* E9 cd  
	 * Jump near, relative, displacement relative to next instruction         
	 * JMP rel32    
	 */

	c->eip += i->disp;

	return 0;
}


int32_t instr_jmp_ea(struct emu_cpu *c, struct emu_cpu_instruction *i)
{

	/* EA cd  
	 * Jump far, absolute, address given in operand                           
	 * JMP ptr16:16 
	 */


	/* EA cp  
	 * Jump far, absolute, address given in operand                           
	 * JMP ptr16:32 
	 */


	return 0;
}

int32_t instr_jmp_eb(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	/* EB cb  
	 * Jump short, relative, displacement relative to next instruction        
	 * JMP rel8     
	 */

	c->eip += i->disp;

	return 0;
}


int32_t instr_group_5_ff_jmp(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	if( i->modrm.opc == 4 )
	{
		if( i->modrm.mod != 3 )
		{
			if( i->prefixes & PREFIX_OPSIZE )
			{
				/* FF /4  
				 * Jump near, absolute indirect, address given in r/m16                   
				 * JMP r/m16    
				 */

				uint16_t disp;
				MEM_WORD_READ(c, i->modrm.ea, &disp);
				
				c->eip = disp;
			}
			else
			{
				/* FF /4  
				 * Jump near, absolute indirect, address given in r/m32                   
				 * JMP r/m32    
				 */

				uint32_t disp;
				MEM_DWORD_READ(c, i->modrm.ea, &disp);
				
				c->eip = disp;
			}
		}
		else
		{
			if( i->prefixes & PREFIX_OPSIZE )
			{
				/* FF /4  
				 * Jump near, absolute indirect, address given in r/m16                   
				 * JMP r/m16    
				 */

				c->eip = *c->reg16[i->modrm.rm];
			}
			else
			{
				/* FF /4  
				 * Jump near, absolute indirect, address given in r/m32                   
				 * JMP r/m32    
				 */

				c->eip = c->reg[i->modrm.rm];
			}
		}
	}
	else /* /5 */
	{
		/* unneeded */
		/* FF /5  
		 * Jump far, absolute indirect, address given in m16:16                   
		 * JMP m16:16   
		 */


		/* FF /5  
		 * Jump far, absolute indirect, address given in m16:32                   
		 * JMP m16:32   
		 */


		return -1;
	}

	return 0;
}

