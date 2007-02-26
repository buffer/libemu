/* @header@ */
#include <stdint.h>
#include <errno.h>

#define INSTR_CALC(bits, a, b, cpu)						\
UINT(bits) operand_a = (a);								\
UINT(bits) operand_b = (b);								\
{																	\
	(a) = operand_a *  operand_b;                                                  \
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

int32_t instr_group_2_c0_rcl(struct emu_cpu *c, struct instruction *i)
{
	if ( i->modrm.mod != 3 )
	{
		/* C0 /2 ib 
		 * Rotate nine bits (CF,r/m8) left imm8 times
		 * RCL r/m8,imm8  
		 */
		uint8_t m8;
		MEM_BYTE_READ(c, i->modrm.ea, &m8);
		INSTR_CALC_AND_SET_FLAGS(8,
								 c,
								 m8,
								 *i->imm8);

		MEM_BYTE_WRITE(c, i->modrm.ea, m8);     
	} else
	{
		/* C0 /2 ib 
		 * Rotate nine bits (CF,r/m8) left imm8 times
		 * RCL r/m8,imm8  
		 */
		INSTR_CALC_AND_SET_FLAGS(8,
								 c,
								 *c->reg8[i->modrm.rm],
								 *i->imm8);
	}

	return 0;
}

int32_t instr_group_2_c1_rcl(struct emu_cpu *c, struct instruction *i)
{
	if ( i->modrm.mod != 3 )
	{
		if ( i->prefixes & PREFIX_OPSIZE )
		{
			/* C1 /2 ib 
			 * Rotate 17 bits (CF,r/m16) left imm8 times
			 * RCL r/m16,imm8
			 */
			uint16_t m16;
			MEM_WORD_READ(c, i->modrm.ea, &m16);
			INSTR_CALC_AND_SET_FLAGS(16,
									 c,
									 m16,
									 *i->imm8);

			MEM_WORD_WRITE(c, i->modrm.ea, m16);        

		} else
		{
			/* C1 /2 ib 
			 * Rotate 33 bits (CF,r/m32) left imm8 times
			 * RCL r/m32,imm8 
			 */
			uint32_t m32;
			MEM_DWORD_READ(c, i->modrm.ea, &m32);
			INSTR_CALC_AND_SET_FLAGS(32,
									 c,
									 m32,
									 *i->imm8);
			MEM_DWORD_WRITE(c, i->modrm.ea, m32);       
		}
	} else
	{
		if ( i->prefixes & PREFIX_OPSIZE )
		{
			/* C1 /2 ib 
			 * Rotate 17 bits (CF,r/m16) left imm8 times
			 * RCL r/m16,imm8
			 */
			INSTR_CALC_AND_SET_FLAGS(16,
									 c,
									 *c->reg16[i->modrm.rm],
									 *i->imm8);

		} else
		{
			/* C1 /2 ib 
			 * Rotate 33 bits (CF,r/m32) left imm8 times
			 * RCL r/m32,imm8 
			 */
			INSTR_CALC_AND_SET_FLAGS(32,
									 c,
									 c->reg[i->modrm.rm],
									 *i->imm8);

		}
	}

	return 0;
}

int32_t instr_group_2_d0_rcl(struct emu_cpu *c, struct instruction *i)
{
	if ( i->modrm.mod != 3 )
	{
		/* D0 /2	 
		 * Rotate nine bits (CF,r/m8) left once        
		 * RCL r/m8,1     
		 */
		uint8_t m8;
		MEM_BYTE_READ(c, i->modrm.ea, &m8);
		INSTR_CALC_AND_SET_FLAGS(8,
								 c,
								 m8,
								 1);

		MEM_BYTE_WRITE(c, i->modrm.ea, m8);     
	} else
	{
		/* D0 /2	 
		 * Rotate nine bits (CF,r/m8) left once        
		 * RCL r/m8,1     
		 */
		INSTR_CALC_AND_SET_FLAGS(8,
								 c,
								 *c->reg8[i->modrm.rm],
								 1);

	}

	return 0;
}

int32_t instr_group_2_d1_rcl(struct emu_cpu *c, struct instruction *i)
{
	if ( i->modrm.mod != 3 )
	{
		if ( i->prefixes & PREFIX_OPSIZE )
		{
			/* D1 /2    
			 * Rotate 17 bits (CF,r/m16) left once
			 * RCL r/m16,1    
			 */
			uint16_t m16;
			MEM_WORD_READ(c, i->modrm.ea, &m16);
			INSTR_CALC_AND_SET_FLAGS(16,
									 c,
									 m16,
									 1);

			MEM_WORD_WRITE(c, i->modrm.ea, m16);        

		} else
		{
			/* D1 /2    
			 * Rotate 33 bits (CF,r/m32) left once
			 * RCL r/m32,1    
			 */
			uint32_t m32;
			MEM_DWORD_READ(c, i->modrm.ea, &m32);
			INSTR_CALC_AND_SET_FLAGS(32,
									 c,
									 m32,
									 1);
			MEM_DWORD_WRITE(c, i->modrm.ea, m32);       
		}
	} else
	{
		if ( i->prefixes & PREFIX_OPSIZE )
		{
			/* D1 /2    
			 * Rotate 17 bits (CF,r/m16) left once
			 * RCL r/m16,1    
			 */
			INSTR_CALC_AND_SET_FLAGS(16,
									 c,
									 *c->reg16[i->modrm.rm],
									 1);

		} else
		{
			/* D1 /2    
			 * Rotate 33 bits (CF,r/m32) left once
			 * RCL r/m32,1    
			 */
			INSTR_CALC_AND_SET_FLAGS(32,
									 c,
									 c->reg[i->modrm.rm],
									 1);

		}
	}

	return 0;
}

int32_t instr_group_2_d2_rcl(struct emu_cpu *c, struct instruction *i)
{
	if ( i->modrm.mod != 3 )
	{
		/* D2 /2    
		 * Rotate nine bits (CF,r/m8) left CL times    
		 * RCL r/m8,CL    
		 */
		uint8_t m8;
		MEM_BYTE_READ(c, i->modrm.ea, &m8);
		INSTR_CALC_AND_SET_FLAGS(8,
								 c,
								 m8,
								 *c->reg8[cl]);

		MEM_BYTE_WRITE(c, i->modrm.ea, m8);     
	} else
	{
		/* D2 /2    
		 * Rotate nine bits (CF,r/m8) left CL times    
		 * RCL r/m8,CL    
		 */
		INSTR_CALC_AND_SET_FLAGS(8,
								 c,
								 *c->reg8[i->modrm.rm],
								 *c->reg8[cl]);
	}

	return 0;
}

int32_t instr_group_2_d3_rcl(struct emu_cpu *c, struct instruction *i)
{
	if ( i->modrm.mod != 3 )
	{
		if ( i->prefixes & PREFIX_OPSIZE )
		{
			/* D3 /2    
			 * Rotate 17 bits (CF,r/m16) left CL times
			 * RCL r/m16,CL   
			 */
			uint16_t m16;
			MEM_WORD_READ(c, i->modrm.ea, &m16);
			INSTR_CALC_AND_SET_FLAGS(16,
									 c,
									 m16,
									 *c->reg8[cl]);

			MEM_WORD_WRITE(c, i->modrm.ea, m16);        
		} else
		{
			/* D3 /2    
			 * Rotate 33 bits (CF,r/m32) left CL times
			 * RCL r/m32,CL
			 */
			uint32_t m32;
			MEM_DWORD_READ(c, i->modrm.ea, &m32);
			INSTR_CALC_AND_SET_FLAGS(32,
									 c,
									 m32,
									 *c->reg8[cl]);

			MEM_DWORD_WRITE(c, i->modrm.ea, m32);       
		}
	} else
	{
		if ( i->prefixes & PREFIX_OPSIZE )
		{
			/* D3 /2    
			 * Rotate 17 bits (CF,r/m16) left CL times
			 * RCL r/m16,CL   
			 */
			INSTR_CALC_AND_SET_FLAGS(16,
									 c,
									 *c->reg16[i->modrm.rm],
									 *c->reg8[cl]);
		} else
		{
			/* D3 /2    
			 * Rotate 33 bits (CF,r/m32) left CL times
			 * RCL r/m32,CL
			 */
			INSTR_CALC_AND_SET_FLAGS(32,
									 c,
									 c->reg[i->modrm.rm],
									 *c->reg8[cl]);
		}
	}


	return 0;
}

