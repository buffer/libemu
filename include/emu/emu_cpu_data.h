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


#ifndef EMU_CPU_DATA_H_
#define EMU_CPU_DATA_H_

#include <stdint.h>
#include <stdbool.h>

#include <emu/emu.h>
#include <emu/emu_cpu_instruction.h>
#include <emu/emu_instruction.h>

enum emu_cpu_flag {
	f_cf = 0, f_pf = 2, f_af = 4, f_zf = 6, f_sf = 7, f_tf = 8, f_if = 9,
	f_df = 10, f_of = 11
};

#define CPU_FLAG_SET(cpu_p, fl) (cpu_p)->eflags |= 1 << (fl)
#define CPU_FLAG_UNSET(cpu_p, fl) (cpu_p)->eflags &= ~(1 << (fl))
#define CPU_FLAG_TOGGLE(cpu_p, fl) (cpu_p)->eflags ^= 1 << (fl)
#define CPU_FLAG_ISSET(cpu_p, fl) ((cpu_p)->eflags & (1 << (fl)))

struct emu_track_and_source;


#define CPU_DEBUG_FLAG_SET(cpu_p, fl) (cpu_p)->debugflags |= 1 << (fl)
#define CPU_DEBUG_FLAG_UNSET(cpu_p, fl) (cpu_p)->debugflags &= ~(1 << (fl))
#define CPU_DEBUG_FLAG_TOGGLE(cpu_p, fl) (cpu_p)->debugflags ^= 1 << (fl)
#define CPU_DEBUG_FLAG_ISSET(cpu_p, fl) ((cpu_p)->debugflags & (1 << (fl)))

enum emu_cpu_debug_flag {
	instruction_string = 0,
	instruction_size = 1,
};

struct emu_cpu
{
	struct emu *emu;
	struct emu_memory *mem;
	
	uint32_t debugflags;

	uint32_t eip;
	uint32_t eflags;
	uint32_t reg[8];
	uint16_t *reg16[8];
	uint8_t *reg8[8];

	struct emu_instruction 			instr;
	struct emu_cpu_instruction_info 	*cpu_instr_info;
	
	uint32_t last_fpu_instr[2];

	char *instr_string;

	bool repeat_current_instr;

	struct emu_track_and_source *tracking;
};


#define MODRM_MOD(x) (((x) >> 6) & 3)
#define MODRM_REGOPC(x) (((x) >> 3) & 7)
#define MODRM_RM(x) ((x) & 7)
    
#define SIB_SCALE(x) (((x) >> 6) & 3)
#define SIB_INDEX(x) (((x) >> 3) & 7)
#define SIB_BASE(x) ((x) & 7)

#define PREFIX_ADSIZE (1 << 0)
#define PREFIX_OPSIZE (1 << 1)
#define PREFIX_LOCK (1 << 2)
#define PREFIX_CS_OVR (1 << 3)
#define PREFIX_DS_OVR (1 << 4)
#define PREFIX_ES_OVR (1 << 5)
#define PREFIX_FS_OVR (1 << 6)
#define PREFIX_GS_OVR (1 << 7)
#define PREFIX_SS_OVR (1 << 8)
#define PREFIX_F2 (1 << 9)
#define PREFIX_F3 (1 << 10)

#define OPSIZE_8 1
#define OPSIZE_16 2
#define OPSIZE_32 3


#define MAX_INT8           127
#define MIN_INT8          -128

#define MAX_UINT8          255
#define MIN_UINT8            0

#define MAX_INT16        32767
#define MIN_INT16       -MAX_INT16 -1

#define MAX_UINT16       65535
#define MIN_UINT16           0


#define MAX_INT32   2147483647
#define MIN_INT32  -MAX_INT32 -1

#define MAX_UINT32	4294967295U
#define MIN_UINT32           0




extern int64_t max_inttype_borders[][2][2];

#define INTOF(bits) int##bits##_t
#define UINTOF(bits) uint##bits##_t

#if !defined(INSTR_CALC)
#if BYTE_ORDER == BIG_ENDIAN 
#define INSTR_CALC(bits, a, b, c, operation)			\
UINTOF(bits) operand_a; \
UINTOF(bits) operand_b; \
bcopy(&(a), &operand_a, bits/8); \
bcopy(&(b), &operand_b, bits/8); \
UINTOF(bits) operation_result = operand_a operation operand_b;    \
bcopy(&operation_result, &(c), bits/8); 
#else // ENDIAN
#define INSTR_CALC(bits, a, b, c, operation)			\
UINTOF(bits) operand_a = a;								\
UINTOF(bits) operand_b = b;								\
UINTOF(bits) operation_result = operand_a operation operand_b;	\
c = operation_result;
#endif // ENDIAN
#endif // INSTR_CALC

#if !defined(INSTR_SET_FLAG_ZF)
#define INSTR_SET_FLAG_ZF(cpu)						\
{															\
	if (operation_result == 0)								\
		CPU_FLAG_SET(cpu, f_zf);								\
	else													\
		CPU_FLAG_UNSET(cpu, f_zf);							\
}
#endif // INSTR_SET_FLAG_ZF

#if !defined(INSTR_SET_FLAG_PF)
#define INSTR_SET_FLAG_PF(cpu)						\
{															\
	int num_p_bits=0;										\
	int i;													\
	for ( i=0;i<8;i++ )										\
		if (operation_result & (1 << i) )					\
			num_p_bits++;									\
															\
	if ((num_p_bits % 2) == 0)								\
		CPU_FLAG_SET(cpu, f_pf);								\
	else													\
		CPU_FLAG_UNSET(cpu, f_pf);							\
}
#endif // INSTR_SET_FLAG_PF

#if !defined(INSTR_SET_FLAG_SF)
#define INSTR_SET_FLAG_SF(cpu)								\
{																	\
	if (operation_result & (1 << (sizeof(operation_result)*8 - 1)))	\
		CPU_FLAG_SET(cpu, f_sf);										\
	else															\
		CPU_FLAG_UNSET(cpu, f_sf);									\
}
#endif // INSTR_SET_FLAG_SF

#if !defined(INSTR_SET_FLAG_OF)
#define INSTR_SET_FLAG_OF(cpu, operand, bits)								\
{																				\
	int64_t sx = (INTOF(bits))operand_a;                                   \
	int64_t sy = (INTOF(bits))operand_b;                                   \
	int64_t sz = 0;                                                             \
																				\
	sz = sx operand sy;															\
																				\
	if (sz < max_inttype_borders[sizeof(operation_result)][0][0] || sz > max_inttype_borders[sizeof(operation_result)][0][1] \
	|| sz != (INTOF(bits))operation_result )									    \
	{                                                                           \
		CPU_FLAG_SET(cpu, f_of);                                                 \
	}else                                                                       \
	{                                                                           \
		CPU_FLAG_UNSET(cpu, f_of);                                               \
	}                                                                           \
}
#endif // INSTR_SET_FLAG_OF


#if !defined(INSTR_SET_FLAG_CF)
#define INSTR_SET_FLAG_CF(cpu, operand)									\
{																				\
	uint64_t ux = (uint64_t)operand_a;                                           \
	uint64_t uy = (uint64_t)operand_b;                                           \
	uint64_t uz = 0;                                                            \
																				\
	uz = ux operand uy;															\
																				\
	if (uz < max_inttype_borders[sizeof(operation_result)][1][0] || uz > max_inttype_borders[sizeof(operation_result)][1][1] \
	|| uz != (uint64_t)operation_result )									    \
	{                                                                           \
		CPU_FLAG_SET(cpu, f_cf);                                                 \
	}else                                                                       \
	{                                                                           \
		CPU_FLAG_UNSET(cpu, f_cf);                                               \
	}                                                                           \
}
#endif // INSTR_SET_FLAG_CF


#include <string.h>
#define WORD_UPPER_TO_BYTE(to,from) \
memcpy(&(to),((uint8_t *)&(from))+1,1);

#define WORD_LOWER_TO_BYTE(to,from) \
memcpy(&(to),&(from),1);

#define DWORD_UPPER_TO_WORD(to,from) \
memcpy(&(to),((uint8_t *)&(from))+2,2);

#define DWORD_LOWER_TO_WORD(to,from) \
memcpy(&(to),&(from),2);

#define QWORD_UPPER_TO_DWORD(to,from) \
memcpy(&(to),((uint8_t *)&(from))+4,4);

#define QWORD_LOWER_TO_DWORD(to,from) \
memcpy(&(to),&(from),4);

#define DWORD_FROM_WORDS(to, upper, lower) \
memcpy(&to,&lower,2); \
memcpy(((char *)&to)+2,&upper,2); 

#define QWORD_FROM_DWORDS(to, upper, lower) \
memcpy(&to,&lower,4); \
memcpy(((char *)&to)+4,&upper,4); 




#define TRACK_INIT_REG32(instruction, reg32) (instruction).track.init.reg[reg32] = 0xffffffff;
#define TRACK_NEED_REG32(instruction, reg32) (instruction).track.need.reg[reg32] = 0xffffffff;

#define TRACK_INIT_REG16(instruction, reg16) (instruction).track.init.reg[reg16] |= 0xffff << 16;
#define TRACK_NEED_REG16(instruction, reg16) (instruction).track.need.reg[reg16] |= 0xffff << 16;

#define TRACK_INIT_REG8(instruction, reg8) (instruction).track.init.reg[reg8] |= 0xff << 24;
#define TRACK_NEED_REG8(instruction, reg8) (instruction).track.need.reg[reg8] |= 0xff << 24;

#define TRACK_INIT_EFLAG(instruction, fl) (instruction).track.init.eflags |= 1 << (fl)
#define TRACK_NEED_EFLAG(instruction, fl) (instruction).track.need.eflags |= 1 << (fl)

#define SOURCE_NORM_POS(instruction, pos) (instruction).source.norm_pos = pos;
#define SOURCE_COND_POS(instruction, pos)	(instruction).source.has_cond_pos = 1; (instruction).source.cond_pos = pos;


#define TRACK_FPU_LAST_INSTRUCTION  0x0

#define TRACK_INIT_FPU(instruction, what) (instruction).track.init.fpu |= 1 << (what);
#define TRACK_NEED_FPU(instruction, what) (instruction).track.need.fpu |= 1 << (what);



#define NNY "no need yet"
#define SST "16bit memory access is unsupported"

#define UNIMPLEMENTED(cpu_p, reason) \
emu_strerror_set((cpu_p)->emu, "The following function is unimplemented %s %s:%i (%s)", __PRETTY_FUNCTION__, __FILE__, __LINE__,  reason); \
return -1; 

#define STUB(cpu_p) \
emu_log((cpu_p)->emu, EMU_LOG_INFO, "The following function is a stub %s %s:%i \n", __PRETTY_FUNCTION__, __FILE__, __LINE__);

#endif /*EMU_CPU_DATA_H_*/
