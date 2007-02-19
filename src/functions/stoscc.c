/* @header@ */
#include <stdint.h>

#include "emu/emu_cpu.h"
#include "emu/emu_cpu_data.h"
#include "emu/emu_cpu_functions.h"
#include "emu/emu_cpu_stack.h"
#include "emu/emu_memory.h"



int32_t instr_stos_aa(struct emu_cpu *c, struct instruction *i)
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
	} else
	{
		MEM_BYTE_WRITE(c,c->reg[esi],*c->reg8[al]);
	}
	return 0;
}

int32_t instr_stos_ab(struct emu_cpu *c, struct instruction *i)
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
		} else
		{
			MEM_WORD_WRITE(c,c->reg[esi],*c->reg16[ax]);
		}
	} else
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
		} else
		{
			MEM_DWORD_WRITE(c,c->reg[esi],c->reg[eax]);
		}
	}
	return 0;
}

