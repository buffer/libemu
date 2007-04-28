/* @header@ */
#include <stdint.h>

#include "emu/emu_cpu.h"
#include "emu/emu_cpu_data.h"

#include "emu/emu_cpu_stack.h"
#include "emu/emu_memory.h"



int32_t instr_stos_aa(struct emu_cpu *c, struct emu_cpu_instruction *i)
{

	/* AA
	 * Store AL at address ES:(E)DI
	 * STOS m8  
	 */

	/* AA 
	 * Store AL at address ES:(E)DI
	 * STOSB    
	 */

	if ( i->prefixes & PREFIX_ADSIZE )
	{
//		MEM_BYTE_WRITE(c,c->reg16[si],*c->reg8[al]);
		UNIMPLEMENTED(c, SST);
	}
	else
	{
		if (i->prefixes & PREFIX_F3)
		{
			/* F3 AA 
			 * Fill ECX bytes at ES:[EDI] with AL
			 * REP STOS m8        
			 */


			if (c->reg[ecx] > 0 )
			{
				c->reg[ecx]--;
				c->repeat_current_instr = true;
				MEM_BYTE_WRITE(c,c->reg[edi],*c->reg8[al]);
				if ( !CPU_FLAG_ISSET(c,f_df) )
				{ /* increment */
					c->reg[edi] += 1;
				}else
				{ /* decrement */
					c->reg[edi] -= 1;
				}
			}
			else
				c->repeat_current_instr = false;

			
		}else
		{
			MEM_BYTE_WRITE(c,c->reg[edi],*c->reg8[al]);
			if ( !CPU_FLAG_ISSET(c,f_df) )
			{ /* increment */
				c->reg[edi] += 1;
			}else
			{ /* decrement */
				c->reg[edi] -= 1;
			}

		}

	}
	return 0;
}

int32_t instr_stos_ab(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	if ( i->prefixes & PREFIX_OPSIZE )
	{
		/* AB
		 * Store AX at address ES:(E)DI
		 * STOS m16 
		 */

		/* AB 
		 * Store AX at address ES:(E)DI
		 * STOSW    
		 */
		if ( i->prefixes & PREFIX_ADSIZE )
		{
//			MEM_WORD_WRITE(c,c->reg16[si],*c->reg16[ax]);
			UNIMPLEMENTED(c, SST);
		}
		else
		{
			MEM_WORD_WRITE(c,c->reg[edi],*c->reg16[ax]);

			if ( !CPU_FLAG_ISSET(c,f_df) )
			{ /* increment */
				c->reg[edi] += 2;
			}
			else
			{ /* decrement */
				c->reg[edi] -= 2;
			}
		}
	}
	else
	{
		/* AB 
		 * Store EAX at address ES:(E)DI
		 * STOS m32 
		 */

		/* AB 
		 * Store EAX at address ES:(E)DI
		 * STOSD    
		 */
		if ( i->prefixes & PREFIX_ADSIZE )
		{
//			MEM_DWORD_WRITE(c,c->reg16[si],c->reg[eax]);
			UNIMPLEMENTED(c, SST);
		}
		else
		{
			MEM_DWORD_WRITE(c,c->reg[edi],c->reg[eax]);

			if ( !CPU_FLAG_ISSET(c,f_df) )
			{ /* increment */
				c->reg[edi] += 4;
			}
			else
			{ /* decrement */
				c->reg[edi] -= 4;
			}

		}
	}
	return 0;
}

