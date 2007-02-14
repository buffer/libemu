#ifndef EMU_CPU_DATA_H_
#define EMU_CPU_DATA_H_

#include <stdint.h>

enum emu_cpu_flag {
	f_cf = 0, f_pf = 2, f_af = 4, f_zf = 6, f_sf = 7, f_tf = 8, f_if = 9,
	f_df = 10, f_of = 11
};

#define CPU_FLAG_SET(cpu_p, fl) (cpu_p)->eflags |= 1 << (fl)
#define CPU_FLAG_UNSET(cpu_p, fl) (cpu_p)->eflags &= ~(1 << (fl))
#define CPU_FLAG_TOGGLE(cpu_p, fl) (cpu_p)->eflags ^= 1 << (fl)
#define CPU_FLAG_ISSET(cpu_p, fl) ((cpu_p)->eflags & (1 << (fl)))

struct emu_cpu
{
	struct emu *emu;
	struct emu_memory *mem;
	
	uint32_t eip;
	uint32_t eflags;
	uint32_t reg[8];
	uint16_t *reg16[8];
	uint8_t *reg8[8];
};

struct instruction
{
	uint8_t opc;
	uint8_t opc_2nd;
	uint8_t prefixes;
	uint8_t s_bit : 1;
	uint8_t w_bit : 1;
	uint8_t operand_size : 2;

	struct /* mod r/m data */
	{
		union
		{
			uint8_t mod : 2;
			uint8_t x : 2;
		};

		union
		{
			uint8_t reg1 : 3;
			uint8_t opc : 3;
			uint8_t sreg3 : 3;
			uint8_t y : 3;
		};

		union
		{
			uint8_t reg : 3;
			uint8_t reg2 : 3;
			uint8_t rm : 3;
			uint8_t z : 3;
		};

		struct
		{
			uint8_t scale : 2;
			uint8_t index : 3;
			uint8_t base : 3;
		} sib;

		union
		{
			uint8_t s8;
			uint16_t s16;
			uint32_t s32;
		} disp;
		
		uint32_t ea;
	} modrm;

	uint32_t imm;
	uint16_t *imm16;
	uint8_t *imm8;
	uint32_t disp;
};

#define MODRM_MOD(x) (((x) >> 6) & 3)
#define MODRM_REGOPC(x) (((x) >> 3) & 7)
#define MODRM_RM(x) ((x) & 7)
    
#define SIB_SCALE(x) (((x) >> 6) & 3)
#define SIB_INDEX(x) (((x) >> 3) & 7)
#define SIB_BASE(x) ((x) & 3)

#define PREFIX_ADSIZE (1 << 0)
#define PREFIX_OPSIZE (1 << 1)
#define PREFIX_LOCK (1 << 2)
#define PREFIX_CS_OVR (1 << 3)
#define PREFIX_DS_OVR (1 << 4)
#define PREFIX_ES_OVR (1 << 5)
#define PREFIX_FS_OVR (1 << 6)
#define PREFIX_GS_OVR (1 << 7)
#define PREFIX_SS_OVR (1 << 8)

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


#if !defined(INSTR_CALC)
#define INSTR_CALC(inttype, a, b, c, operation)			\
uint##inttype##_t operand_a = a;								\
uint##inttype##_t operand_b = b;								\
uint##inttype##_t operation_result = operand_a operation operand_b;	\
c = operation_result;
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
#define INSTR_SET_FLAG_OF(cpu, operand, inttype)								\
{																				\
	int64_t sx = (int##inttype##_t)operand_a;                                   \
	int64_t sy = (int##inttype##_t)operand_b;                                   \
	int64_t sz = 0;                                                             \
																				\
	sz = sx operand sy;															\
																				\
	if (sz < max_inttype_borders[sizeof(operation_result)][0][0] || sz > max_inttype_borders[sizeof(operation_result)][0][1] \
	|| sz != (int64_t)operation_result )									    \
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



/*
#define INSTR_CALC_AND_SET_FLAGS(inttype, cpu, a, b, c, operation, operation_id)	\
INSTR_CALC(inttype, a, b, c, operation)									\
																			\
if (instruction_flag_sets[operation_id].modify_flags & (1 << (f_zf)) )		\
	INSTR_SET_FLAG_ZF(cpu)											\
																			\
if (instruction_flag_sets[operation_id].modify_flags & (1 << (f_pf)) )		\
	INSTR_SET_FLAG_PF(cpu)											\
																			\
if (instruction_flag_sets[operation_id].modify_flags & (1 << (f_sf)) )		\
	INSTR_SET_FLAG_SF(cpu)											\
																			\
if (instruction_flag_sets[operation_id].modify_flags & (1 << (f_cf)) )		\
	INSTR_SET_FLAG_CF(cpu, operation)								\
																			\
if (instruction_flag_sets[operation_id].modify_flags & (1 << (f_of)) )		\
	INSTR_SET_FLAG_OF(cpu, operation)								\
*/


#endif /*EMU_CPU_DATA_H_*/
