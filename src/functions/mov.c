/********************************************************************************
 *                               libemu
 *
 *                    - x86 shellcode emulation -
 *
 *
 * Copyright (C) 2007  Paul Baecher & Markus Koetter
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 * 
 * 
 *             contact nepenthesdev@users.sourceforge.net  
 *
 *******************************************************************************/

#include <stdint.h>

#include "emu/emu_cpu.h"
#include "emu/emu_cpu_data.h"

#include "emu/emu_memory.h"

#include "emu/emu_track.h"
#include "emu/emu_log.h"

/*Intel Architecture Software Developer's Manual Volume 2: Instruction Set Reference (24319102.PDF) page 442*/

int32_t instr_mov_88(struct emu_cpu *c, struct emu_cpu_instruction *i)
{

	/* 88 /r  
	 * Move r8 to r/m8									
	 * MOV r/m8,r8      
	 */

	if( i->modrm.mod != 3 )
	{
		MEM_BYTE_WRITE(c, i->modrm.ea, *c->reg8[i->modrm.opc]);
	}
	else
	{
		*c->reg8[i->modrm.rm] = *c->reg8[i->modrm.opc];
	}

	return 0;
}

int32_t instr_mov_89(struct emu_cpu *c, struct emu_cpu_instruction *i)
{

																		 
	/* 89 /r  
	 * Move r16 to r/m16                                
	 * MOV r/m16,r16    
	 */																		 

	/* 89 /r  
	 * Move r32 to r/m32                                
	 * MOV r/m32,r32    
	 */																		 

	if( i->prefixes & PREFIX_OPSIZE )
	{
		if( i->modrm.mod != 3 )
		{
			MEM_WORD_WRITE(c, i->modrm.ea, *c->reg16[i->modrm.opc]);
		}
		else
		{
			*c->reg16[i->modrm.rm] = *c->reg16[i->modrm.opc];
		}
	}
	else
	{
		if( i->modrm.mod != 3 )
		{
			MEM_DWORD_WRITE(c, i->modrm.ea, c->reg[i->modrm.opc]);
		}
		else
		{
			c->reg[i->modrm.rm] = c->reg[i->modrm.opc];

			TRACK_NEED_REG32(c->instr, i->modrm.opc);
			TRACK_INIT_REG32(c->instr, i->modrm.rm);
		}
	}

	return 0;
}

int32_t instr_mov_8a(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	/* 8A /r  
	 * Move r/m8 to r8                                  
	 * MOV r8,r/m8      
	 */																		 

	if( i->modrm.mod != 3 )
	{
		MEM_BYTE_READ(c, i->modrm.ea, c->reg8[i->modrm.opc]);
		TRACK_INIT_REG16(c->instr, i->modrm.opc);
	}
	else
	{
		*c->reg8[i->modrm.opc] = *c->reg8[i->modrm.rm];
	}

	return 0;
}

int32_t instr_mov_8b(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	if( i->prefixes & PREFIX_OPSIZE )
	{
		/* 8B /r  
		 * Move r/m16 to r16                                
		 * MOV r16,r/m16    
		 */																		 

		if( i->modrm.mod != 3 )
		{
			MEM_WORD_READ(c, i->modrm.ea, c->reg16[i->modrm.opc]);
			TRACK_INIT_REG16(c->instr, i->modrm.opc);
		}
		else
		{
			*c->reg16[i->modrm.opc] = *c->reg16[i->modrm.rm];
		}
	}
	else
	{
		/* 8B /r  
		 * Move r/m32 to r32                                
		 * MOV r32,r/m32    
		 */																		 

		if( i->modrm.mod != 3 )
		{
			MEM_DWORD_READ(c, i->modrm.ea, &c->reg[i->modrm.opc]);
			TRACK_INIT_REG32(c->instr, i->modrm.opc);
		}
		else
		{
			c->reg[i->modrm.opc] = c->reg[i->modrm.rm];

			TRACK_INIT_REG32(c->instr, i->modrm.opc);

			if ( c->tracking != NULL )
			{
				c->tracking->track.reg[i->modrm.opc] = c->tracking->track.reg[i->modrm.rm];
			}
		}
	}

	return 0;
}

int32_t instr_mov_8c(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	/* 8C /r  
	 * Move segment register to r/m16                   
	 * MOV r/m16,Sreg** 
	 */
	STUB(c);
	return 0;
}

int32_t instr_mov_8e(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	/* 8E /r  
	 * Move r/m16 to segment register                   
	 * MOV Sreg,r/m16** 
	 */																		 
	STUB(c);
	return 0;
}

int32_t instr_mov_a0(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	/* A0     
	 * Move byte at (seg:offset) to AL                  
	 * MOV AL,moffs8*   
	 */																		 
	MEM_BYTE_READ(c, i->disp, c->reg8[al]);

	return 0;
}

int32_t instr_mov_a1(struct emu_cpu *c, struct emu_cpu_instruction *i)
{

	if ( i->prefixes & PREFIX_OPSIZE )
	{
		/* A1     
		 * Move word at (seg:offset) to AX                  
		 * MOV AX,moffs16*  
		 */                                                                      

		MEM_WORD_READ(c, i->disp, c->reg16[ax]);
	}
	else
	{
		/* A1     
		 * Move doubleword at (seg:offset) to EAX           
		 * MOV EAX,moffs32* 
		 */
		MEM_DWORD_READ(c, i->disp, &c->reg[eax]);
		
		TRACK_INIT_REG32(c->instr, eax);
	}
	return 0;
}

int32_t instr_mov_a2(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	/* A2     
	 * Move AL to (seg:offset)                          
	 * MOV moffs8*,AL   
	 */																		 
	MEM_BYTE_WRITE(c, i->imm, *c->reg8[al]);

	return 0;
}

int32_t instr_mov_a3(struct emu_cpu *c, struct emu_cpu_instruction *i)
{

	if ( i->prefixes & PREFIX_OPSIZE )
	{

		/* A3     
		 * Move AX to (seg:offset)                          
		 * MOV moffs16*,AX  
		 */
		MEM_WORD_WRITE(c, i->imm, *c->reg16[ax]);
	}
	else
	{
		/* A3     
		 * Move EAX to (seg:offset)                         
		 * MOV moffs32*,EAX 
		 */                                         
		MEM_DWORD_WRITE(c, i->imm, c->reg[eax]);
	}

	return 0;
}

int32_t instr_movsb(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
    if (i->prefixes & PREFIX_OPSIZE ) {
	UNIMPLEMENTED(c, SST);
    }
    if (i->prefixes & PREFIX_F3) {
	/* Copy ECX bytes from DS:[ESI] to ES:[EDI] */
	if (c->reg[ecx] > 0 )
	{
	    uint8_t tmp;
	    c->reg[ecx]--;
	    c->repeat_current_instr = true;
	    MEM_BYTE_READ(c, c->reg[esi], &tmp);
	    MEM_BYTE_WRITE(c, c->reg[edi], tmp);
	    if ( !CPU_FLAG_ISSET(c,f_df) )
	    { /* increment */
		c->reg[edi] += 1;
		c->reg[esi] += 1;
	    }else
	    { /* decrement */
		c->reg[edi] -= 1;
		c->reg[esi] -= 1;
	    }
	}
	else
	    c->repeat_current_instr = false;
    } else {
	/* a4 move ds:esi -> es->edi */
	uint8_t tmp;
	MEM_BYTE_READ(c, c->reg[esi], &tmp);
	MEM_BYTE_WRITE(c, c->reg[edi], tmp);
	if ( !CPU_FLAG_ISSET(c,f_df) )
	{ /* increment */
		c->reg[edi] += 1;
		c->reg[esi] += 1;
	} else
	{ /* decrement */
	    c->reg[edi] -= 1;
	    c->reg[esi] -= 1;
	}
    }
    return 0;
}

int32_t instr_mov_bx_1(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	/* B0+ rb 
	 * Move imm8 to r8                                  
	 * MOV r8,imm8      
	 */
	*c->reg8[i->opc & 7] = *i->imm8;

	return 0;
}

int32_t instr_mov_bx_2(struct emu_cpu *c, struct emu_cpu_instruction *i)
{


	if ( i->prefixes & PREFIX_OPSIZE )
	{
		/* B8+ rw 
		 * Move imm16 to r16                                
		 * MOV r16,imm16    
		 */
#if BYTE_ORDER == BIG_ENDIAN
		bcopy(i->imm16, c->reg16[i->opc & 7], 2);
#else
		*c->reg16[i->opc & 7] = *i->imm16;
#endif


	}
	else
	{
		/* B8+ rd 
		 * Move imm32 to r32                                
		 * MOV r32,imm32    
		 */                         

		c->reg[i->opc & 7] = i->imm;
		TRACK_INIT_REG32(c->instr, i->opc & 7);
	}

	return 0;
}

int32_t instr_mov_c6(struct emu_cpu *c, struct emu_cpu_instruction *i)
{


	/* C6 /0  
	 * Move imm8 to r/m8                                
	 * MOV r/m8,imm8    
	 */
	
	if( i->modrm.mod != 3 )
	{
		MEM_BYTE_WRITE(c, i->modrm.ea, *i->imm8);
	}
	else
	{
		*c->reg8[i->modrm.rm] = *i->imm8;
	}																	 

	return 0;
}

int32_t instr_mov_c7(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	/* C7 /0  
	 * Move imm16 to r/m16                              
	 * MOV r/m16,imm16  
	 */																		 

	/* C7 /0  
	 * Move imm32 to r/m32                              
	 * MOV r/m32,imm32  
	 */

	if( i->prefixes & PREFIX_OPSIZE )
	{
		if( i->modrm.mod != 3 )
		{
			MEM_WORD_WRITE(c, i->modrm.ea, *i->imm16);
		}
		else
		{
#if BYTE_ORDER == BIG_ENDIAN
			bcopy(i->imm16, c->reg16[i->modrm.rm], 2);
#else
			*c->reg16[i->modrm.rm] = *i->imm16;
#endif
		}                                                                    
	}
	else
	{
		if( i->modrm.mod != 3 )
		{
			MEM_DWORD_WRITE(c, i->modrm.ea, i->imm);
		}
		else
		{
			c->reg[i->modrm.rm] = i->imm;
		}																	 
	}

	return 0;
}




