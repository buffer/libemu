/* @header@ */
#include <stdint.h>
#include <errno.h>

#define INSTR_CALC(bits, a, b, cpu)						\
UINT(bits) operand_a = (a);								\
UINT(bits) operand_b = (b);								\
{																	\
	UINT(bits) it;                                                   \
	for (it=0;it<operand_b;it++)                                       \
	{                                                               \
		if (operand_a & 1)                                          \
		{                                                           \
           	CPU_FLAG_SET(cpu,f_cf);                                 \
																	\
			operand_a = operand_a >> 1;                             \
			/*operand_a = operand_a | (1 << bits);*/                    \
		}else                                                       \
		{                                                           \
			CPU_FLAG_UNSET(cpu,f_cf);                               \
																	\
			/*operand_a = operand_a >> 1; */                             \
		}                                                           \
	}                                                               \
	if (operand_b == 1)                                             \
	{                                                               \
		if (														\
				((INT(bits))operand_a > 0 && (INT(bits))(a) < 0) || \
				((INT(bits))operand_a < 0 && (INT(bits))(a) > 0)	\
			)       												\
		{                                                           \
			CPU_FLAG_SET(cpu,f_of);                                 \
		}                                                           \
	}                                                               \
	(a) = operand_a;                                                  \
}


#define INSTR_CALC_AND_SET_FLAGS(bits, cpu, a, b)	\
INSTR_CALC(bits, a, b, cpu)

#include "emu/emu.h"
#include "emu/emu_cpu.h"
#include "emu/emu_cpu_data.h"
#include "emu/emu_cpu_functions.h"
#include "emu/emu_cpu_stack.h"
#include "emu/emu_memory.h"

/*Intel Architecture Software Developer's Manual Volume 2: Instruction Set Reference (24319102.PDF) page 631*/


int32_t instr_group_2_c0_ror(struct emu_cpu *c, struct instruction *i)
{
	if ( i->modrm.mod != 3 )
	{
		/* C0 /1 ib 
		 * Rotate eight bits r/m16 right imm8 times
		 * ROR r/m8,imm8  
		 */
		uint8_t m8;
		MEM_BYTE_READ(c, i->modrm.ea, &m8);
		INSTR_CALC_AND_SET_FLAGS(8,
								 c,
								 m8,
								 *i->imm8);

		MEM_BYTE_WRITE(c, i->modrm.ea, m8);     
	}
	else
	{
		/* C0 /1 ib 
		 * Rotate eight bits r/m16 right imm8 times
		 * ROR r/m8,imm8  
		 */
		INSTR_CALC_AND_SET_FLAGS(8,
								 c,
								 *c->reg8[i->modrm.rm],
								 *i->imm8);

	}

	return 0;
}

int32_t instr_group_2_c1_ror(struct emu_cpu *c, struct instruction *i)
{
	if ( i->modrm.mod != 3 )
	{
		if ( i->prefixes & PREFIX_OPSIZE )
		{
			/* C1 /1 ib 
			 * Rotate 16 bits r/m16 right imm8 times
			 * ROR r/m16,imm8 
			 */
			uint16_t m16;
			MEM_WORD_READ(c, i->modrm.ea, &m16);
			INSTR_CALC_AND_SET_FLAGS(16,
									 c,
									 m16,
									 *i->imm8);

			MEM_WORD_WRITE(c, i->modrm.ea, m16);        

		}
		else
		{
			/* C1 /1 ib 
			 * Rotate 32 bits r/m32 right imm8 times
			 * ROR r/m32,imm8 
			 */
			uint32_t m32;
			MEM_DWORD_READ(c, i->modrm.ea, &m32);
			INSTR_CALC_AND_SET_FLAGS(32,
									 c,
									 m32,
									 *i->imm8);
			MEM_DWORD_WRITE(c, i->modrm.ea, m32);       
		}
	}
	else
	{
		if ( i->prefixes & PREFIX_OPSIZE )
		{
			/* C1 /1 ib 
			 * Rotate 16 bits r/m16 right imm8 times
			 * ROR r/m16,imm8 
			 */
			INSTR_CALC_AND_SET_FLAGS(16,
									 c,
									 *c->reg16[i->modrm.rm],
									 *i->imm8);

		}
		else
		{
			/* C1 /1 ib 
			 * Rotate 32 bits r/m32 right imm8 times
			 * ROR r/m32,imm8 
			 */
			INSTR_CALC_AND_SET_FLAGS(32,
									 c,
									 c->reg[i->modrm.rm],
									 *i->imm8);

		}
	}
	return 0;
}

int32_t instr_group_2_d0_ror(struct emu_cpu *c, struct instruction *i)
{

	if ( i->modrm.mod != 3 )
	{
		/* D0 /1	 
		 * Rotate eight bits r/m8 right once              
		 * ROR r/m8,1     
		 */
		uint8_t m8;
		MEM_BYTE_READ(c, i->modrm.ea, &m8);
		INSTR_CALC_AND_SET_FLAGS(8,
								 c,
								 m8,
								 1);

		MEM_BYTE_WRITE(c, i->modrm.ea, m8);     
	}
	else
	{
		/* D0 /1	 
		 * Rotate eight bits r/m8 right once              
		 * ROR r/m8,1     
		 */
		INSTR_CALC_AND_SET_FLAGS(8,
								 c,
								 *c->reg8[i->modrm.rm],
								 1);

	}

	return 0;
}

int32_t instr_group_2_d1_ror(struct emu_cpu *c, struct instruction *i)
{
	if ( i->modrm.mod != 3 )
	{
		if ( i->prefixes & PREFIX_OPSIZE )
		{
			/* D1 /1	 
			 * Rotate 16 bits r/m16 right once                 
			 * ROR r/m16,1    
			 */                                                                     
			uint16_t m16;
			MEM_WORD_READ(c, i->modrm.ea, &m16);
			INSTR_CALC_AND_SET_FLAGS(16,
									 c,
									 m16,
									 1);

			MEM_WORD_WRITE(c, i->modrm.ea, m16);        

		}
		else
		{
			/* D1 /1	 
			 * Rotate 32 bits r/m32 right once                
			 * ROR r/m32,1    
			 */                                                                    
			uint32_t m32;
			MEM_DWORD_READ(c, i->modrm.ea, &m32);
			INSTR_CALC_AND_SET_FLAGS(32,
									 c,
									 m32,
									 1);
			MEM_DWORD_WRITE(c, i->modrm.ea, m32);       
		}
	}
	else
	{
		if ( i->prefixes & PREFIX_OPSIZE )
		{
			/* D1 /1	 
			 * Rotate 16 bits r/m16 right once                 
			 * ROR r/m16,1    
			 */                                                                     
			INSTR_CALC_AND_SET_FLAGS(16,
									 c,
									 *c->reg16[i->modrm.rm],
									 1);

		}
		else
		{
			/* D1 /1	 
			 * Rotate 32 bits r/m32 right once                
			 * ROR r/m32,1    
			 */                                                                    
			INSTR_CALC_AND_SET_FLAGS(32,
									 c,
									 c->reg[i->modrm.rm],
									 1);

		}
	}
	return 0;
}

int32_t instr_group_2_d2_ror(struct emu_cpu *c, struct instruction *i)
{

	if ( i->modrm.mod != 3 )
	{
		/* D2 /1    
		 * Rotate eight bits r/m8 right CL times          
		 * ROR r/m8,CL    
		 */
		uint8_t m8;
		MEM_BYTE_READ(c, i->modrm.ea, &m8);
		INSTR_CALC_AND_SET_FLAGS(8,
								 c,
								 m8,
								 *c->reg8[cl]);

		MEM_BYTE_WRITE(c, i->modrm.ea, m8);     
	}
	else
	{
		/* D2 /1    
		 * Rotate eight bits r/m8 right CL times          
		 * ROR r/m8,CL    
		 */
		INSTR_CALC_AND_SET_FLAGS(8,
								 c,
								 *c->reg8[i->modrm.rm],
								 *c->reg8[cl]);
	}
	return 0;
}


int32_t instr_group_2_d3_ror(struct emu_cpu *c, struct instruction *i)
{
	if ( i->modrm.mod != 3 )
	{
		if ( i->prefixes & PREFIX_OPSIZE )
		{
			/* D3 /1    
			 * Rotate 16 bits r/m16 right CL times             
			 * ROR r/m16,CL   
			 */
			uint16_t m16;
			MEM_WORD_READ(c, i->modrm.ea, &m16);
			INSTR_CALC_AND_SET_FLAGS(16,
									 c,
									 m16,
									 *c->reg8[cl]);

			MEM_WORD_WRITE(c, i->modrm.ea, m16);        
		}
		else
		{
			/* D3 /1    
			 * Rotate 32 bits r/m32 right CL times            
			 * ROR r/m32,CL   
			 */
			uint32_t m32;
			MEM_DWORD_READ(c, i->modrm.ea, &m32);
			INSTR_CALC_AND_SET_FLAGS(32,
									 c,
									 m32,
									 *c->reg8[cl]);

			MEM_DWORD_WRITE(c, i->modrm.ea, m32);       
		}
	}
	else
	{
		if ( i->prefixes & PREFIX_OPSIZE )
		{
			/* D3 /1    
			 * Rotate 16 bits r/m16 right CL times             
			 * ROR r/m16,CL   
			 */
			INSTR_CALC_AND_SET_FLAGS(16,
									 c,
									 *c->reg16[i->modrm.rm],
									 *c->reg8[cl]);
		}
		else
		{
			/* D3 /1    
			 * Rotate 32 bits r/m32 right CL times            
			 * ROR r/m32,CL   
			 */
			INSTR_CALC_AND_SET_FLAGS(32,
									 c,
									 c->reg[i->modrm.rm],
									 *c->reg8[cl]);
		}
	}
	return 0;
}

