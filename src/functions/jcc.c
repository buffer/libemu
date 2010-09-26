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

#define OF_IS_ONE(cpu)  (CPU_FLAG_ISSET(cpu, f_of) != 0) 
#define OF_IS_ZERO(cpu) (CPU_FLAG_ISSET(cpu, f_of) == 0)
#define OF_IS(cpu)      (CPU_FLAG_ISSET(cpu, f_of)?1:0) 
 
#define CF_IS_ONE(cpu)  (CPU_FLAG_ISSET(cpu, f_cf) != 0) 
#define CF_IS_ZERO(cpu) (CPU_FLAG_ISSET(cpu, f_cf) == 0) 

#define ZF_IS_ONE(cpu)  (CPU_FLAG_ISSET(cpu, f_zf) != 0) 
#define ZF_IS_ZERO(cpu) (CPU_FLAG_ISSET(cpu, f_zf) == 0) 

#define SF_IS_ONE(cpu)  (CPU_FLAG_ISSET(cpu, f_sf) != 0) 
#define SF_IS_ZERO(cpu) (CPU_FLAG_ISSET(cpu, f_sf) == 0) 
#define SF_IS(cpu)      (CPU_FLAG_ISSET(cpu, f_sf)?1:0) 

#define PF_IS_ONE(cpu)  (CPU_FLAG_ISSET(cpu, f_pf) != 0) 
#define PF_IS_ZERO(cpu) (CPU_FLAG_ISSET(cpu, f_pf) == 0) 


int32_t instr_jcc_70(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	SOURCE_COND_POS(c->instr, c->eip + i->disp);
	TRACK_NEED_EFLAG(c->instr, f_of);

	/* 70 cb       Jump short if overflow (OF=1)                           JO rel8         */
	if (OF_IS_ONE(c))
	{
		c->eip += i->disp;
	}
	return 0;
}

int32_t instr_jcc_71(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	SOURCE_COND_POS(c->instr, c->eip + i->disp);
	TRACK_NEED_EFLAG(c->instr, f_of);

	/* 71 cb       Jump short if not overflow (OF=0)                       JNO rel8        */
	if (OF_IS_ZERO(c))
	{
		c->eip += i->disp;		
	}
	return 0;
}

int32_t instr_jcc_72(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	SOURCE_COND_POS(c->instr, c->eip + i->disp);
	TRACK_NEED_EFLAG(c->instr, f_cf);

	/* 72 cb       Jump short if below (CF=1)                              JB rel8         */
	/* 72 cb       Jump short if carry (CF=1)                              JC rel8         */
	/* 72 cb       Jump short if not above or equal (CF=1)                 JNAE rel8       */
	if (CF_IS_ONE(c))
	{
		c->eip += i->disp;		
	}
	return 0;
}

int32_t instr_jcc_73(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	SOURCE_COND_POS(c->instr, c->eip + i->disp);
	TRACK_NEED_EFLAG(c->instr, f_cf);

	/* 73 cb       Jump short if above or equal (CF=0)                     JAE rel8        */
	/* 73 cb       Jump short if not below (CF=0)                          JNB rel8        */
	/* 73 cb       Jump short if not carry (CF=0)                          JNC rel8        */
	if (CF_IS_ZERO(c))
	{
		c->eip += i->disp;		
	}

	return 0;
}

int32_t instr_setcc_0f94(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
    uint8_t b;
	TRACK_NEED_EFLAG(c->instr, f_zf);
	b = ZF_IS_ONE(c) ? 1 : 0;

	/* 0f 94 sete r/m8 Set byte if equal (ZF = 1) */
	if (i->modrm.mod != 3) {
	    MEM_BYTE_WRITE(c, i->modrm.ea, b);
	} else {
	    *c->reg8[i->modrm.rm] = b;
	    TRACK_INIT_REG8(c->instr, i->modrm.rm);
	}
	return 0;
}

int32_t instr_setcc_0f95(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
    uint8_t b;
	TRACK_NEED_EFLAG(c->instr, f_zf);
	b = ZF_IS_ZERO(c) ? 1 : 0;

	/* 0f 94 setne r/m8 Set byte if not equal (ZF = 0) */
	if (i->modrm.mod != 3) {
	    MEM_BYTE_WRITE(c, i->modrm.ea, b);
	} else {
	    *c->reg8[i->modrm.rm] = b;
	    TRACK_INIT_REG8(c->instr, i->modrm.rm);
	}
	return 0;
}

int32_t instr_jcc_74(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	SOURCE_COND_POS(c->instr, c->eip + i->disp);
	TRACK_NEED_EFLAG(c->instr, f_zf);

	/* 74 cb       Jump short if equal (ZF=1)                              JE rel8         */
	/* 74 cb       Jump short if zero (ZF = 1)                             JZ rel8         */
	if (ZF_IS_ONE(c))
	{
		c->eip += i->disp;		
	}

	return 0;
}

int32_t instr_jcc_75(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	SOURCE_COND_POS(c->instr, c->eip + i->disp);
	TRACK_NEED_EFLAG(c->instr, f_zf);

	/* 75 cb       Jump short if not equal (ZF=0)                          JNE rel8        */
	/* 75 cb       Jump short if not zero (ZF=0)                           JNZ rel8        */
	if (ZF_IS_ZERO(c))
	{
		c->eip += i->disp;		
	}

	return 0;
}

int32_t instr_jcc_76(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	SOURCE_COND_POS(c->instr, c->eip + i->disp);
	TRACK_NEED_EFLAG(c->instr, f_cf);
	TRACK_NEED_EFLAG(c->instr, f_zf);

	/* 76 cb       Jump short if below or equal (CF=1 or ZF=1)             JBE rel8        */
	/* 76 cb       Jump short if not above (CF=1 or ZF=1)                  JNA rel8        */
	if (CF_IS_ONE(c) || ZF_IS_ONE(c))
	{
		c->eip += i->disp;		
	}

	return 0;
}

int32_t instr_jcc_77(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	SOURCE_COND_POS(c->instr, c->eip + i->disp);
	TRACK_NEED_EFLAG(c->instr, f_cf);
	TRACK_NEED_EFLAG(c->instr, f_zf);

    /* 77 cb       Jump short if above (CF=0 and ZF=0)                     JA rel8         */
	/* 77 cb       Jump short if not below or equal (CF=0 and ZF=0)        JNBE rel8       */
	if (CF_IS_ZERO(c) && ZF_IS_ZERO(c))
	{
		c->eip += i->disp;
	}


	return 0;
}

int32_t instr_jcc_78(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	SOURCE_COND_POS(c->instr, c->eip + i->disp);
	TRACK_NEED_EFLAG(c->instr, f_sf);

	/* 78 cb       Jump short if sign (SF=1)                               JS rel8         */
	if (SF_IS_ONE(c))
	{
		c->eip += i->disp;		
	}

	return 0;
}

int32_t instr_jcc_79(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	SOURCE_COND_POS(c->instr, c->eip + i->disp);
	TRACK_NEED_EFLAG(c->instr, f_sf);

	/* 79 cb       Jump short if not sign (SF=0)                           JNS rel8        */
	if (SF_IS_ZERO(c))
	{
		c->eip += i->disp;		
	}

	return 0;
}

int32_t instr_jcc_7a(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	SOURCE_COND_POS(c->instr, c->eip + i->disp);
	TRACK_NEED_EFLAG(c->instr, f_pf);

	/* 7A cb       Jump short if parity even (PF=1)                        JPE rel8        */
	/* 7A cb       Jump short if parity (PF=1)                             JP rel8         */
	if (PF_IS_ONE(c))
	{
		c->eip += i->disp;		
	}

	return 0;
}

int32_t instr_jcc_7b(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	SOURCE_COND_POS(c->instr, c->eip + i->disp);
	TRACK_NEED_EFLAG(c->instr, f_pf);

	/* 7B cb       Jump short if not parity (PF=0)                         JNP rel8        */
	/* 7B cb       Jump short if parity odd (PF=0)                         JPO rel8        */
	if (PF_IS_ZERO(c))
	{
		c->eip += i->disp;
	}


	return 0;
}

int32_t instr_jcc_7c(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	SOURCE_COND_POS(c->instr, c->eip + i->disp);
	TRACK_NEED_EFLAG(c->instr, f_sf);
	TRACK_NEED_EFLAG(c->instr, f_of);

	/* 7C cb       Jump short if less (SF<>OF)                             JL rel8         */
	/* 7C cb       Jump short if not greater or equal (SF<>OF)             JNGE rel8       */
	if (SF_IS(c) != OF_IS(c))
	{
		c->eip += i->disp;
	}


	return 0;
}

int32_t instr_jcc_7d(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	SOURCE_COND_POS(c->instr, c->eip + i->disp);
	TRACK_NEED_EFLAG(c->instr, f_sf);
	TRACK_NEED_EFLAG(c->instr, f_of);

	/* 7D cb       Jump short if greater or equal (SF=OF)                  JGE rel8        */
	/* 7D cb       Jump short if not less (SF=OF)                          JNL rel8        */
	if (SF_IS(c) == OF_IS(c))
	{
		c->eip += i->disp;
	}

	return 0;
}

int32_t instr_jcc_7e(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	SOURCE_COND_POS(c->instr, c->eip + i->disp);
	TRACK_NEED_EFLAG(c->instr, f_zf);
	TRACK_NEED_EFLAG(c->instr, f_sf);
	TRACK_NEED_EFLAG(c->instr, f_of);

	/* 7E cb       Jump short if less or equal (ZF=1 or SF<>OF)            JLE rel8        */
	/* 7E cb       Jump short if not greater (ZF=1 or SF<>OF)              JNG rel8        */
	if ( ZF_IS_ONE(c) || (SF_IS(c) != OF_IS(c)))
	{
		c->eip += i->disp;
	}


	return 0;
}

int32_t instr_jcc_7f(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	SOURCE_COND_POS(c->instr, c->eip + i->disp);
	TRACK_NEED_EFLAG(c->instr, f_zf);
	TRACK_NEED_EFLAG(c->instr, f_sf);
	TRACK_NEED_EFLAG(c->instr, f_of);

	/* 7F cb       Jump short if greater (ZF=0 and SF=OF)                  JG rel8         */
	/* 7F cb       Jump short if not less or equal (ZF=0 and SF=OF)        JNLE rel8       */
	if ( ZF_IS_ONE(c) && (SF_IS(c) == OF_IS(c)))
	{
		c->eip += i->disp;
	}

	return 0;
}

int32_t instr_jcc_e3(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	SOURCE_COND_POS(c->instr, c->eip + i->disp);
	

	/* E3 cb       Jump short if CX register is 0                          JCXZ rel8       */
	/* E3 cb       Jump short if ECX register is 0                         JECXZ rel8      */
	if ( i->prefixes & PREFIX_OPSIZE )
	{
		TRACK_NEED_REG16(c->instr, cx);
		if (*c->reg16[cx] == 0)
		{
			c->eip += i->disp;		
		}
	}else
	{
		TRACK_NEED_REG32(c->instr, ecx);
		if (c->reg[ecx] == 0)
		{
			c->eip += i->disp;		
		}
	}

	return 0;
}

int32_t instr_jcc_0f80(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	SOURCE_COND_POS(c->instr, c->eip + i->disp);
	TRACK_NEED_EFLAG(c->instr, f_of);

	/* 0F 80 cw/cd  Jump near if overflow (OF=1)                           JO rel16/32     */
	if (OF_IS_ONE(c))
	{
		c->eip += i->disp;		
	}

	return 0;
}

int32_t instr_jcc_0f81(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	SOURCE_COND_POS(c->instr, c->eip + i->disp);
	TRACK_NEED_EFLAG(c->instr, f_of);

	/* 0F 81 cw/cd  Jump near if not overflow (OF=0)                       JNO rel16/32    */
	if (OF_IS_ZERO(c))
	{
		c->eip += i->disp;		
	}

	return 0;
}

int32_t instr_jcc_0f82(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	SOURCE_COND_POS(c->instr, c->eip + i->disp);
	TRACK_NEED_EFLAG(c->instr, f_cf);

	/* 0F 82 cw/cd  Jump near if below (CF=1)                              JB rel16/32     */
	/* 0F 82 cw/cd  Jump near if carry (CF=1)                              JC rel16/32     */
	/* 0F 82 cw/cd  Jump near if not above or equal (CF=1)                 JNAE rel16/32   */
	if (CF_IS_ONE(c))
	{
		c->eip += i->disp;		
	}

	return 0;
}

int32_t instr_jcc_0f83(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	SOURCE_COND_POS(c->instr, c->eip + i->disp);
	TRACK_NEED_EFLAG(c->instr, f_cf);

	/* 0F 83 cw/cd  Jump near if above or equal (CF=0)                     JAE rel16/32    */
	/* 0F 83 cw/cd  Jump near if not below (CF=0)                          JNB rel16/32    */
	/* 0F 83 cw/cd  Jump near if not carry (CF=0)                          JNC rel16/32    */
	if (CF_IS_ZERO(c))
	{
		c->eip += i->disp;		
	}

	return 0;
}

int32_t instr_jcc_0f84(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	SOURCE_COND_POS(c->instr, c->eip + i->disp);
	TRACK_NEED_EFLAG(c->instr, f_zf);

	/* 0F 84 cw/cd  Jump near if equal (ZF=1)                              JE rel16/32     */
	/* 0F 84 cw/cd  Jump near if zero (ZF=1)                               JZ rel16/32     */
	if (ZF_IS_ONE(c))
	{
		c->eip += i->disp;		
	}

	return 0;
}

int32_t instr_jcc_0f85(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	SOURCE_COND_POS(c->instr, c->eip + i->disp);
	TRACK_NEED_EFLAG(c->instr, f_zf);

	/* 0F 85 cw/cd  Jump near if not equal (ZF=0)                          JNE rel16/32    */
	/* 0F 85 cw/cd  Jump near if not zero (ZF=0)                           JNZ rel16/32    */
	if (ZF_IS_ZERO(c))
	{
		c->eip += i->disp;		
	}

	return 0;
}

int32_t instr_jcc_0f86(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	SOURCE_COND_POS(c->instr, c->eip + i->disp);
	TRACK_NEED_EFLAG(c->instr, f_cf);
	TRACK_NEED_EFLAG(c->instr, f_zf);

	/* 0F 86 cw/cd  Jump near if below or equal (CF=1 or ZF=1)             JBE rel16/32    */
	/* 0F 86 cw/cd  Jump near if not above (CF=1 or ZF=1)                  JNA rel16/32    */
	if (CF_IS_ONE(c) || ZF_IS_ONE(c))
	{
		c->eip += i->disp;		
	}

	return 0;
}

int32_t instr_jcc_0f87(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	SOURCE_COND_POS(c->instr, c->eip + i->disp);
	TRACK_NEED_EFLAG(c->instr, f_cf);
	TRACK_NEED_EFLAG(c->instr, f_zf);

	/* 0F 87 cw/cd  Jump near if above (CF=0 and ZF=0)                     JA rel16/32     */
	/* 0F 87 cw/cd  Jump near if not below or equal (CF=0 and ZF=0)        JNBE rel16/32   */
	if (CF_IS_ZERO(c) && ZF_IS_ZERO(c))
	{
		c->eip += i->disp;
	}
	

	return 0;
}

int32_t instr_jcc_0f88(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	SOURCE_COND_POS(c->instr, c->eip + i->disp);
	TRACK_NEED_EFLAG(c->instr, f_sf);

	/* 0F 88 cw/cd  Jump near if sign (SF=1)                               JS rel16/32     */
	if (SF_IS_ONE(c))
	{
		c->eip += i->disp;		
	}

	return 0;
}

int32_t instr_jcc_0f89(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	SOURCE_COND_POS(c->instr, c->eip + i->disp);
	TRACK_NEED_EFLAG(c->instr, f_sf);

	/* 0F 89 cw/cd  Jump near if not sign (SF=0)                           JNS rel16/32    */
	if (SF_IS_ZERO(c))
	{
		c->eip += i->disp;		
	}

	return 0;
}

int32_t instr_jcc_0f8a(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	SOURCE_COND_POS(c->instr, c->eip + i->disp);
	TRACK_NEED_EFLAG(c->instr, f_pf);

	/* 0F 8A cw/cd  Jump near if parity even (PF=1)                        JPE rel16/32    */
	/* 0F 8A cw/cd  Jump near if parity (PF=1)                             JP rel16/32     */
	if (PF_IS_ONE(c))
	{
		c->eip += i->disp;		
	}

	return 0;
}

int32_t instr_jcc_0f8b(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	SOURCE_COND_POS(c->instr, c->eip + i->disp);
	TRACK_NEED_EFLAG(c->instr, f_pf);

	/* 0F 8B cw/cd  Jump near if not parity (PF=0)                         JNP rel16/32    */
	/* 0F 8B cw/cd  Jump near if parity odd (PF=0)                         JPO rel16/32    */
	if (PF_IS_ZERO(c))
	{
		c->eip += i->disp;		
	}

	return 0;
}

int32_t instr_jcc_0f8c(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	SOURCE_COND_POS(c->instr, c->eip + i->disp);
	TRACK_NEED_EFLAG(c->instr, f_sf);
	TRACK_NEED_EFLAG(c->instr, f_of);

	/* 0F 8C cw/cd  Jump near if less (SF<>OF)                             JL rel16/32     */
	/* 0F 8C cw/cd  Jump near if not greater or equal (SF<>OF)             JNGE rel16/32   */
	if (SF_IS(c) != OF_IS(c))
	{
		c->eip += i->disp;		
	}

	return 0;
}

int32_t instr_jcc_0f8d(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	SOURCE_COND_POS(c->instr, c->eip + i->disp);
	TRACK_NEED_EFLAG(c->instr, f_sf);
	TRACK_NEED_EFLAG(c->instr, f_of);

	/* 0F 8D cw/cd  Jump near if greater or equal (SF=OF)                  JGE rel16/32    */
	/* 0F 8D cw/cd  Jump near if not less (SF=OF)                          JNL rel16/32    */
	if (SF_IS(c) == OF_IS(c))
	{
		c->eip += i->disp;		
	}

	return 0;
}

int32_t instr_jcc_0f8e(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	SOURCE_COND_POS(c->instr, c->eip + i->disp);
	TRACK_NEED_EFLAG(c->instr, f_zf);
	TRACK_NEED_EFLAG(c->instr, f_sf);
	TRACK_NEED_EFLAG(c->instr, f_of);

	/* 0F 8E cw/cd  Jump near if less or equal (ZF=1 or SF<>OF)            JLE rel16/32    */
	/* 0F 8E cw/cd  Jump near if not greater (ZF=1 or SF<>OF)              JNG rel16/32    */
	if (ZF_IS_ONE(c) || SF_IS(c) != OF_IS(c))
	{
		c->eip += i->disp;		
	}

	return 0;
}

int32_t instr_jcc_0f8f(struct emu_cpu *c, struct emu_cpu_instruction *i)
{
	SOURCE_COND_POS(c->instr, c->eip + i->disp);
	TRACK_NEED_EFLAG(c->instr, f_zf);
	TRACK_NEED_EFLAG(c->instr, f_sf);
	TRACK_NEED_EFLAG(c->instr, f_of);

	/* 0F 8F cw/cd  Jump near if greater (ZF=0 and SF=OF)                  JG rel16/32     */
	/* 0F 8F cw/cd  Jump near if not less or equal (ZF=0 and SF=OF)        JNLE rel16/32   */
	if (ZF_IS_ZERO(c) && SF_IS(c) == OF_IS(c))
	{
		c->eip += i->disp;
	}


	return 0;
}

