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
#include <errno.h>

#include "emu/emu.h"
#include "emu/emu_cpu.h"
#include "emu/emu_cpu_data.h"

#include "emu/emu_cpu_stack.h"
#include "emu/emu_memory.h"

/*Intel Architecture Software Developer's Manual Volume 2: Instruction Set Reference (24319102.PDF) page 645*/

/*
F2 A6 REPNE CMPS m8,m8   Find matching bytes in ES:[(E)DI] and DS:[(E)SI]
F2 A7 REPNE CMPS m16,m16 Find matching words in ES:[(E)DI] and DS:[(E)SI]
F2 A7 REPNE CMPS m32,m32 Find matching doublewords in ES:[(E)DI] and DS:[(E)SI]
F2 AE REPNE SCAS m8      Find AL, starting at ES:[(E)DI]
F2 AF REPNE SCAS m16     Find AX, starting at ES:[(E)DI]
F2 AF REPNE SCAS m32     Find EAX, starting at ES:[(E)DI]
F3 6C REP INS r/m8, DX   Input (E)CX bytes from port DX into ES:[(E)DI]                            
F3 6D REP INS r/m16,DX   Input (E)CX words from port DX into ES:[(E)DI]                            
F3 6D REP INS r/m32,DX   Input (E)CX doublewords from port DX into ES:[(E)DI]                      
F3 6E REP OUTS DX,r/m8   Output (E)CX bytes from DS:[(E)SI] to port DX        
F3 6F REP OUTS DX,r/m16  Output (E)CX words from DS:[(E)SI] to port DX        
F3 6F REP OUTS DX,r/m32  Output (E)CX doublewords from DS:[(E)SI] to port DX  
F3 A4 REP MOVS m8,m8     Move (E)CX bytes from DS:[(E)SI] to ES:[(E)DI]                            
F3 A5 REP MOVS m16,m16   Move (E)CX words from DS:[(E)SI] to ES:[(E)DI]                            
F3 A5 REP MOVS m32,m32   Move (E)CX doublewords from DS:[(E)SI] to ES:[(E)DI]                      
F3 A6 REPE CMPS m8,m8    Find nonmatching bytes in ES:[(E)DI] and DS:[(E)SI]
F3 A7 REPE CMPS m16,m16  Find nonmatching words in ES:[(E)DI] and DS:[(E)SI]
F3 A7 REPE CMPS m32,m32  Find nonmatching doublewords in ES:[(E)DI] and DS:[(E)SI]
F3 AA REP STOS m8        Fill (E)CX bytes at ES:[(E)DI] with AL
F3 AB REP STOS m16       Fill (E)CX words at ES:[(E)DI] with AX
F3 AB REP STOS m32       Fill (E)CX doublewords at ES:[(E)DI] with EAX
F3 AC REP LODS AL        Load (E)CX bytes from DS:[(E)SI] to AL
F3 AD REP LODS AX        Load (E)CX words from DS:[(E)SI] to AX
F3 AD REP LODS EAX       Load (E)CX doublewords from DS:[(E)SI] to EAX
F3 AE REPE SCAS m8       Find non-AL byte starting at ES:[(E)DI]
F3 AF REPE SCAS m16      Find non-AX word starting at ES:[(E)DI]
F3 AF REPE SCAS m32      Find non-EAX doubleword starting at ES:[(E)DI]
*/

int32_t instr_repcc_f2a6(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	/* F2 A6 
	 * Find matching bytes in ES:[(E)DI] and DS:[(E)SI]
	 * REPNE CMPS m8,m8   
	 */
	return 0;
}

int32_t instr_repcc_f2a7(struct emu_cpu *c, struct emu_cpu_instruction *i)
{

	/* F2 A7 
	 * Find matching words in ES:[(E)DI] and DS:[(E)SI]
	 * REPNE CMPS m16,m16 
	 */

	/* F2 A7 
	 * Find matching doublewords in ES:[(E)DI] and DS:[(E)SI]
	 * REPNE CMPS m32,m32 
	 */
	return 0;
}

int32_t instr_repcc_f2ae(struct emu_cpu *c, struct emu_cpu_instruction *i)
{

	/* F2 AE 
	 * Find AL, starting at ES:[(E)DI]
	 * REPNE SCAS m8      
	 */
	return 0;
}

int32_t instr_repcc_f2af(struct emu_cpu *c, struct emu_cpu_instruction *i)
{

	/* F2 AF 
	 * Find AX, starting at ES:[(E)DI]
	 * REPNE SCAS m16     
	 */

	/* F2 AF 
	 * Find EAX, starting at ES:[(E)DI]
	 * REPNE SCAS m32     
	 */
	return 0;
}

int32_t instr_repcc_f36c(struct emu_cpu *c, struct emu_cpu_instruction *i)
{

	/* F3 6C 
	 * Input (E)CX bytes from port DX into ES:[(E)DI]
	 * REP INS r/m8, DX   
	 */
	return 0;
}

int32_t instr_repcc_f36d(struct emu_cpu *c, struct emu_cpu_instruction *i)
{

	/* F3 6D 
	 * Input (E)CX words from port DX into ES:[(E)DI]
	 * REP INS r/m16,DX   
	 */

	/* F3 6D 
	 * Input (E)CX doublewords from port DX into ES:[(E)DI]
	 * REP INS r/m32,DX   
	 */
	return 0;
}

int32_t instr_repcc_f36e(struct emu_cpu *c, struct emu_cpu_instruction *i)
{

	/* F3 6E 
	 * Output (E)CX bytes from DS:[(E)SI] to port DX               
	 * REP OUTS DX,r/m8   
	 */
	return 0;
}

int32_t instr_repcc_f36f(struct emu_cpu *c, struct emu_cpu_instruction *i)
{

	/* F3 6F 
	 * Output (E)CX words from DS:[(E)SI] to port DX               
	 * REP OUTS DX,r/m16  
	 */

	/* F3 6F 
	 * Output (E)CX doublewords from DS:[(E)SI] to port DX         
	 * REP OUTS DX,r/m32  
	 */
	return 0;
}

int32_t instr_repcc_f3a4(struct emu_cpu *c, struct emu_cpu_instruction *i)
{

	/* F3 A4 
	 * Move (E)CX bytes from DS:[(E)SI] to ES:[(E)DI]
	 * REP MOVS m8,m8     
	 */
	return 0;
}

int32_t instr_repcc_f3a5(struct emu_cpu *c, struct emu_cpu_instruction *i)
{

	/* F3 A5 
	 * Move (E)CX words from DS:[(E)SI] to ES:[(E)DI]
	 * REP MOVS m16,m16   
	 */

	/* F3 A5 
	 * Move (E)CX doublewords from DS:[(E)SI] to ES:[(E)DI]
	 * REP MOVS m32,m32   
	 */
	return 0;
}

int32_t instr_repcc_f3aa(struct emu_cpu *c, struct emu_cpu_instruction *i)
{

	/* F3 AA 
	 * Fill (E)CX bytes at ES:[(E)DI] with AL
	 * REP STOS m8        
	 */
	return 0;
}

int32_t instr_repcc_f3ab(struct emu_cpu *c, struct emu_cpu_instruction *i)
{

	/* F3 AB 
	 * Fill (E)CX words at ES:[(E)DI] with AX
	 * REP STOS m16       
	 */

	/* F3 AB 
	 * Fill (E)CX doublewords at ES:[(E)DI] with EAX
	 * REP STOS m32       
	 */
	return 0;
}

int32_t instr_repcc_f3ac(struct emu_cpu *c, struct emu_cpu_instruction *i)
{

	/* F3 AC 
	 * Load (E)CX bytes from DS:[(E)SI] to AL
	 * REP LODS AL        
	 */
	return 0;
}

int32_t instr_repcc_f3ad(struct emu_cpu *c, struct emu_cpu_instruction *i)
{

	/* F3 AD 
	 * Load (E)CX words from DS:[(E)SI] to AX
	 * REP LODS AX        
	 */

	/* F3 AD 
	 * Load (E)CX doublewords from DS:[(E)SI] to EAX
	 * REP LODS EAX       
	 */
	return 0;
}

int32_t instr_repcc_f3a6(struct emu_cpu *c, struct emu_cpu_instruction *i)
{


	/* F3 A6 
	 * Find nonmatching bytes in ES:[(E)DI] and DS:[(E)SI]
	 * REPE CMPS m8,m8    
	 */
	return 0;
}

int32_t instr_repcc_f3a7(struct emu_cpu *c, struct emu_cpu_instruction *i)
{

	/* F3 A7 
	 * Find nonmatching words in ES:[(E)DI] and DS:[(E)SI]
	 * REPE CMPS m16,m16  
	 */

	/* F3 A7 
	 * Find nonmatching doublewords in ES:[(E)DI] and DS:[(E)SI]
	 * REPE CMPS m32,m32  
	 */
	return 0;
}

int32_t instr_repcc_f3ae(struct emu_cpu *c, struct emu_cpu_instruction *i)
{

	/* F3 AE 
	 * Find non-AL byte starting at ES:[(E)DI]
	 * REPE SCAS m8       
	 */
	return 0;
}

int32_t instr_repcc_f3af(struct emu_cpu *c, struct emu_cpu_instruction *i)
{

	/* F3 AF 
	 * Find non-AX word starting at ES:[(E)DI]
	 * REPE SCAS m16      
	 */

	/* F3 AF 
	 * Find non-EAX doubleword starting at ES:[(E)DI]
	 * REPE SCAS m32      
	 */
	return 0;
}

