/* @header@ */
#include <stdint.h>
#include <errno.h>

#include "emu/emu.h"
#include "emu/emu_cpu.h"
#include "emu/emu_cpu_data.h"
#include "emu/emu_cpu_functions.h"
#include "emu/emu_cpu_stack.h"
#include "emu/emu_memory.h"

/*Intel Architecture Software Developer's Manual Volume 2: Instruction Set Reference (24319102.PDF) page 645*/

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

