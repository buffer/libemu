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


enum emu_instr
{
	EMU_INSTR_AAA = 0,
	EMU_INSTR_AAD,
	EMU_INSTR_AAM,
	EMU_INSTR_AAS,
	EMU_INSTR_ADC,
	EMU_INSTR_ADD,
	EMU_INSTR_AND,
	EMU_INSTR_ARPL,
	EMU_INSTR_BOUND,
	EMU_INSTR_BSF,
	EMU_INSTR_BSR,
	EMU_INSTR_BSWAP,
	EMU_INSTR_BT,
	EMU_INSTR_BTS,
	EMU_INSTR_BTR,
	EMU_INSTR_BTC,
	EMU_INSTR_CALL,
	EMU_INSTR_CBW,
	EMU_INSTR_CLC,
	EMU_INSTR_CLD,
	EMU_INSTR_CLI,
	EMU_INSTR_CLTS,
	EMU_INSTR_CMC,
	EMU_INSTR_CMOVcc,
	EMU_INSTR_CMP,
	EMU_INSTR_CMPS,
	EMU_INSTR_CMPXCHG,
	EMU_INSTR_CMPXCHG8B,
	EMU_INSTR_COMSID,
	EMU_INSTR_COMISS,
	EMU_INSTR_CPUID,
	EMU_INSTR_CWD,
	EMU_INSTR_DAA,
	EMU_INSTR_DAS,
	EMU_INSTR_DEC,
	EMU_INSTR_DIV,
	EMU_INSTR_ENTER,
	EMU_INSTR_ESC,
	EMU_INSTR_FCMOVcc,
	EMU_INSTR_FCOMI,
	EMU_INSTR_FCOMIP,
	EMU_INSTR_FUCOMI,
	EMU_INSTR_FUCOMIP,
	EMU_INSTR_HLT,
	EMU_INSTR_IDIV,
	EMU_INSTR_IMUL,
	EMU_INSTR_IN,
	EMU_INSTR_INC,
	EMU_INSTR_INS,
	EMU_INSTR_INT,
	EMU_INSTR_INTO,
	EMU_INSTR_INVD,
	EMU_INSTR_INVLPG,
	EMU_INSTR_UCOMSID,
	EMU_INSTR_UCOMISS,
	EMU_INSTR_IRET,
	EMU_INSTR_Jcc,
	EMU_INSTR_JCXZ,
	EMU_INSTR_JMP,
	EMU_INSTR_LAHF,
	EMU_INSTR_LAR,
	EMU_INSTR_LDS,
	EMU_INSTR_LES,
	EMU_INSTR_LSS,
	EMU_INSTR_LFS,
	EMU_INSTR_LGS,
	EMU_INSTR_LEA,
	EMU_INSTR_LEAVE,
	EMU_INSTR_LGDT,
	EMU_INSTR_LIDT,
	EMU_INSTR_LLDT,
	EMU_INSTR_LMSW,
	EMU_INSTR_LOCK,
	EMU_INSTR_LODS,
	EMU_INSTR_LOOP,
	EMU_INSTR_LOOPE,
	EMU_INSTR_LOOPNE,
	EMU_INSTR_LSL,
	EMU_INSTR_LTR,
	EMU_INSTR_MONITOR,
	EMU_INSTR_MWAIT,
	EMU_INSTR_MOV,
	EMU_INSTR_MOVS,
	EMU_INSTR_MOVSX,
	EMU_INSTR_MOVZX,
	EMU_INSTR_MUL,
	EMU_INSTR_NEG,
	EMU_INSTR_NOP,
	EMU_INSTR_NOT,
	EMU_INSTR_OR,
	EMU_INSTR_OUT,
	EMU_INSTR_OUTS,
	EMU_INSTR_POP,
	EMU_INSTR_POPA,
	EMU_INSTR_POPF,
	EMU_INSTR_PUSH,
	EMU_INSTR_PUSHA,
	EMU_INSTR_PUSHF,
	EMU_INSTR_RCL_1,
	EMU_INSTR_RCR_1,
	EMU_INSTR_RCL_count,
	EMU_INSTR_RCR_count,
	EMU_INSTR_RDMSR,
	EMU_INSTR_RDPMC,
	EMU_INSTR_RDTSC,
	EMU_INSTR_REP,
	EMU_INSTR_REPE,
	EMU_INSTR_REPNE,
	EMU_INSTR_RET,
	EMU_INSTR_ROL_1,
	EMU_INSTR_ROR_1,
	EMU_INSTR_ROL_count,
	EMU_INSTR_ROR_count,
	EMU_INSTR_RSM,
	EMU_INSTR_SAHF,
	EMU_INSTR_SAL_1,
	EMU_INSTR_SAR_1,
	EMU_INSTR_SHL_1,
	EMU_INSTR_SHR_1,
	EMU_INSTR_SAL_count,
	EMU_INSTR_SAR_count,
	EMU_INSTR_SHL_count,
	EMU_INSTR_SHR_count,
	EMU_INSTR_SBB,
	EMU_INSTR_SCAS,
	EMU_INSTR_SETcc,
	EMU_INSTR_SGDT,
	EMU_INSTR_SIDT,
	EMU_INSTR_SLDT,
	EMU_INSTR_SMSW,
	EMU_INSTR_SHLD,
	EMU_INSTR_SHRD,
	EMU_INSTR_STC,
	EMU_INSTR_STD,
	EMU_INSTR_STI,
	EMU_INSTR_STOS,
	EMU_INSTR_STR,
	EMU_INSTR_SUB,
	EMU_INSTR_TEST,
	EMU_INSTR_UD2,
	EMU_INSTR_VERR,
	EMU_INSTR_VERRW,
	EMU_INSTR_WAIT,
	EMU_INSTR_WBINVD,
	EMU_INSTR_WRMSR,
	EMU_INSTR_XADD,
	EMU_INSTR_XCHG,
	EMU_INSTR_XLAT,
	EMU_INSTR_XOR
};

struct emu_instruction_eflag_effect
{
	const char *instrstr;
	uint32_t instr;
	uint32_t test_flags;
	uint32_t modify_flags;
	uint32_t reset_flags;
	uint32_t set_flags;
	uint32_t restore_flags;
};


extern int64_t max_inttype_borders[][2][2];
extern struct emu_instruction_eflag_effect instruction_flag_sets[];



#define INSTR_CALC_GENERIC(inttype,a,b,c,operation)			\
inttype operand_a = a;										\
inttype operand_b = b;										\
inttype operation_result = operand_a operation operand_b;	\
c = operation_result;										\


#define INSTR_SET_FLAG_ZF_GENERIC(cpu)						\
{															\
	if (operation_result == 0)								\
		CPU_FLAG_SET(cpu,f_zf);								\
	else													\
		CPU_FLAG_UNSET(cpu,f_zf);							\
}															\

#define INSTR_SET_FLAG_PF_GENERIC(cpu)						\
{															\
	int num_p_bits=0;										\
	int i;													\
	for ( i=0;i<8;i++ )										\
		if (operation_result & (1 << i) )					\
			num_p_bits++;									\
															\
	if ((num_p_bits % 2) == 0)								\
		CPU_FLAG_SET(cpu,f_pf);								\
	else													\
		CPU_FLAG_UNSET(cpu,f_pf);							\
}


#define INSTR_SET_FLAG_SF_GENERIC(cpu)								\
{																	\
	if (operation_result & (1 << (sizeof(operation_result) - 1)))	\
		CPU_FLAG_SET(cpu,f_sf);										\
	else															\
		CPU_FLAG_UNSET(cpu,f_sf);									\
}																	\


#define INSTR_SET_FLAG_OF_GENERIC(cpu,operand)									\
{																				\
	int64_t sx = (int64_t)operand_a;                                            \
	int64_t sy = (int64_t)operand_b;                                            \
	int64_t sz = 0;                                                             \
																				\
	sz = sx operand sy;															\
																				\
	if (sz < max_inttype_borders[sizeof(operation_result)][0][0] || sz > max_inttype_borders[sizeof(operation_result)][0][1] \
	|| sz != (int64_t)operation_result )									    \
	{                                                                           \
		CPU_FLAG_SET(cpu,f_of);                                                 \
	}else                                                                       \
	{                                                                           \
		CPU_FLAG_UNSET(cpu,f_of);                                               \
	}                                                                           \
}																				\


#define INSTR_SET_FLAG_CF_GENERIC(cpu,operand)									\
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
		CPU_FLAG_SET(cpu,f_cf);                                                 \
	}else                                                                       \
	{                                                                           \
		CPU_FLAG_UNSET(cpu,f_cf);                                               \
	}                                                                           \
}																				\




#define INSTR_CALC_AND_SET_FLAGS_GENERIC(inttype,cpu,a,b,c,operation,operation_id)	\
INSTR_CALC_GENERIC(inttype,a,b,c,operation)									\
																			\
if (instruction_flag_sets[operation_id].modify_flags & (1 << (f_zf)) )		\
	INSTR_SET_FLAG_ZF_GENERIC(cpu)											\
																			\
if (instruction_flag_sets[operation_id].modify_flags & (1 << (f_pf)) )		\
	INSTR_SET_FLAG_PF_GENERIC(cpu)											\
																			\
if (instruction_flag_sets[operation_id].modify_flags & (1 << (f_sf)) )		\
	INSTR_SET_FLAG_SF_GENERIC(cpu)											\
																			\
if (instruction_flag_sets[operation_id].modify_flags & (1 << (f_cf)) )		\
	INSTR_SET_FLAG_CF_GENERIC(cpu,operation)								\
																			\
if (instruction_flag_sets[operation_id].modify_flags & (1 << (f_of)) )		\
	INSTR_SET_FLAG_OF_GENERIC(cpu,operation)								\



#endif /*EMU_CPU_DATA_H_*/
