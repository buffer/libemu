/* @header@ */
#include <stdint.h>
#include <errno.h>
#include <string.h>

#define INSTR_CALC(inbits, outbits, a, b,cpu) \
UINT(inbits) operand_a = a; \
UINT(inbits) operand_b = b; \
UINT(outbits) operation_result = operand_a * operand_b; \
UINT(inbits) upper_result; \
UINT(inbits) lower_result; \
memcpy(&upper_result,&operation_result,inbits/8); \
memcpy(&lower_result,&operation_result+inbits/8,inbits/8); 

#include "emu/emu.h"
#include "emu/emu_cpu.h"
#include "emu/emu_cpu_data.h"
#include "emu/emu_cpu_functions.h"
#include "emu/emu_cpu_stack.h"
#include "emu/emu_memory.h"

/*Intel Architecture Software Developer's Manual Volume 2: Instruction Set Reference (24319102.PDF) page 488*/

#define INSTR_CALC_AND_SET_FLAGS(inbits, outbits, cpu, a, b)	\
INSTR_CALC(inbits, outbits, a, b, cpu)									\
if (upper_result == 0) \
{ \
	CPU_FLAG_UNSET(cpu,f_cf); \
	CPU_FLAG_UNSET(cpu,f_of); \
} \
else \
{ \
	CPU_FLAG_SET(cpu,f_cf); \
	CPU_FLAG_SET(cpu,f_of); \
}


int32_t instr_group_3_f6_mul(struct emu_cpu *c, struct instruction *i)
{
	if ( i->modrm.mod != 3 )
	{
		/* F6 /4 
		 * Unsigned multiply (AX <- AL * r/m8)             
		 * MUL r/m8  
		 */
		uint8_t m8;
		MEM_BYTE_READ(c, i->modrm.ea, &m8);
		INSTR_CALC_AND_SET_FLAGS(8, 
								 16,
								 c,
								 *c->reg8[al],
								 m8)
		*c->reg16[ax] = operation_result;

	}else
	{
		/* F6 /4 
		 * Unsigned multiply (AX <- AL * r/m8)             
		 * MUL r/m8  
		 */
		INSTR_CALC_AND_SET_FLAGS(8,
								 16,
								 c,
								 *c->reg8[al],
								 *c->reg8[i->modrm.rm])

		*c->reg16[ax] = operation_result;
	}
	return 0;
}

int32_t instr_group_3_f7_mul(struct emu_cpu *c, struct instruction *i)
{
	if ( i->modrm.mod != 3 )
	{
		if ( i->prefixes & PREFIX_OPSIZE )
		{
			/* F7 /4 
			 * Unsigned multiply (DX:AX <- AX * r/m16)         
			 * MUL r/m16 
			 */          
			uint16_t m16;
			MEM_WORD_READ(c, i->modrm.ea, &m16);
			INSTR_CALC_AND_SET_FLAGS(16,
									 32,
									 c,
									 *c->reg16[al],
									 m16)
			*c->reg16[dx] = upper_result;
			*c->reg16[ax] = lower_result;
		}
		else
		{
			/* F7 /4 
			 * Unsigned multiply (EDX:EAX <- EAX * r/m32)      
			 * MUL r/m32 
			 */
			uint32_t m32;
			MEM_DWORD_READ(c, i->modrm.ea, &m32);
			INSTR_CALC_AND_SET_FLAGS(32,
									 64,
									 c,
									 c->reg[eax],
									 m32)

			c->reg[edx] = upper_result;
			c->reg[eax] = lower_result;
		}
	}
	else
	{
		if ( i->prefixes & PREFIX_OPSIZE )
		{
			/* F7 /4 
			 * Unsigned multiply (DX:AX <- AX * r/m16)         
			 * MUL r/m16 
			 */                                                                      
			INSTR_CALC_AND_SET_FLAGS(16,
									 32,
									 c,
									 *c->reg16[al],
									 *c->reg16[i->modrm.rm])
			*c->reg16[dx] = upper_result;
			*c->reg16[ax] = lower_result;
		}
		else
		{
			/* F7 /4 
			 * Unsigned multiply (EDX:EAX <- EAX * r/m32)      
			 * MUL r/m32 
			 */
			INSTR_CALC_AND_SET_FLAGS(32,
									 64,
									 c,
									 c->reg[eax],
									 c->reg[i->modrm.rm])
			c->reg[edx] = upper_result;
			c->reg[eax] = lower_result;

		}
	}
	return 0;
}
