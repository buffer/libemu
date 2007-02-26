/* @header@ */
#include <stdint.h>

#include "emu/emu_cpu.h"
#include "emu/emu_cpu_data.h"
#include "emu/emu_cpu_functions.h"
#include "emu/emu_memory.h"

/*Intel Architecture Software Developer's Manual Volume 2: Instruction Set Reference (24319102.PDF) page 335*/

#ifdef INSTR_CALC_AND_SET_FLAGS
#undef INSTR_CALC_AND_SET_FLAGS
#endif // INSTR_CALC_AND_SET_FLAGS

#define INSTR_CALC_AND_SET_FLAGS(bits, cpu, a, b, c, operation)	\
INSTR_CALC(bits, a, b, c, operation)							

//INSTR_SET_FLAG_CF(cpu)											
//INSTR_SET_FLAG_OF(cpu)											



int32_t instr_imul_69(struct emu_cpu *c, struct instruction *i)
{

	/* 69 /r iw 
	 * word register <- r/m16 * immediate word                        
	 * IMUL r16,r/m16,imm16         
	 */
                                                                                        
	/* 69 /r id 
	 * doubleword register <- r/m32 * immediate doubleword            
	 * IMUL r32,r/m32,imm32         
	 */                                                                                        
																								 
	/* 69 /r iw 
	 * word register <- r/m16 * immediate word                        
	 * IMUL r16,imm16      
	 */
																								 
	/* 69 /r id 
	 * doubleword register <- r/m32 * immediate doubleword            
	 * IMUL r32,imm32      
	 */

	return 0;
}

int32_t instr_imul_6b(struct emu_cpu *c, struct instruction *i)
{
	if ( i->modrm.mod != 3 )
	{
		if ( i->prefixes & PREFIX_OPSIZE )
		{
			/* 6B /r ib 
			 * word register <- m16 * sign-extended immediate byte          
			 * IMUL r16,m16,imm8 
			 */
			int16_t sexd = (int16_t)*i->imm8;

			uint16_t dst;
			MEM_WORD_READ(c, i->modrm.ea, &dst);

			INSTR_CALC_AND_SET_FLAGS(16, 
									 c, 
									 dst,
									 sexd, 
									 *c->reg16[i->modrm.opc], 
									 *)

		}
		else
		{
			/* 6B /r ib 
			 * word register <- m16 * sign-extended immediate byte          
			 * IMUL r16,m16,imm8 
			 */

			int32_t sexd = (int32_t)*i->imm8;

			uint32_t dst;
			MEM_DWORD_READ(c, i->modrm.ea, &dst);

			INSTR_CALC_AND_SET_FLAGS(32, 
									 c, 
									 dst,
									 sexd, 
									 c->reg[i->modrm.opc],
									 *)

		}
	}
	else
	{
		if ( i->prefixes & PREFIX_OPSIZE )
		{
			/* 6B /r ib 
			 * word register <- r16 * sign-extended immediate byte          
			 * IMUL r16,r16,imm8 
			 *
			 * word register <- word register * sign-extended immediate byte                                                                
			 * IMUL r16,imm8       
			 */
			int16_t sexd = (int16_t)*i->imm8;

			INSTR_CALC_AND_SET_FLAGS(16, 
									 c, 
									 *c->reg16[i->modrm.rm],
									 sexd, 
									 *c->reg16[i->modrm.opc], 
									 *)

		}
		else
		{
			/* 6B /r ib 
			 * doubleword register <- r32 * sign-extended immediate byte                                                                
			 * IMUL r32,r32,imm8 
			 *
			 * doubleword register <- doubleword register * sign-extended immediate byte                                             
			 * IMUL r32,imm8       
			 */
			int32_t sexd = (int32_t)*i->imm8;

			INSTR_CALC_AND_SET_FLAGS(32, 
									 c, 
									 c->reg[i->modrm.rm],
									 sexd, 
									 c->reg[i->modrm.opc],
									 *)

		}
	}
	return 0;
}


int32_t instr_imul_0f_af(struct emu_cpu *c, struct instruction *i)
{

	/* 0F AF /r 
	 * word register <- word register * r/m word                      
	 * IMUL r16,r/m16      
	 */																								 
																								 
	/* 0F AF /r 
	 * doubleword register <- doubleword register * r/m doubleword                                                          
	 * IMUL r32,r/m32      
	 */

	return 0;
}



int32_t instr_imul_group_3_f6(struct emu_cpu *c, struct instruction *i)
{

	/* F6 /5    
	 * AX<- AL * r/m byte                                             
	 * IMUL r/m8			 
	 */
	return 0;
}

int32_t instr_imul_group_3_f7(struct emu_cpu *c, struct instruction *i)
{
	/* F7 /5    
	 * DX:AX <- AX * r/m word                                         
	 * IMUL r/m16          
	 */
																								 
																								 
	/* F7 /5    
	 * EDX:EAX <- EAX * r/m doubleword                                
	 * IMUL r/m32          
	 */																								 

	return 0;
}

