#ifndef EMU_CPU_DATA_H_
#define EMU_CPU_DATA_H_

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



static int64_t max_inttype_borders[][2][2] =                                            
{                                                                          
	{                                                                      
		{0,0},                                                             
		{0,0},                                                             
	},                                                                     
	{                                                                      
		{MIN_INT8, MAX_INT8},                                              
		{MIN_UINT8, MAX_UINT8},                                            
	},                                                                     
	{                                                                      
		{MIN_INT16, MAX_INT16},                                            
		{MIN_UINT16, MAX_UINT16},                                          
	},                                                                     
	{                                                                      
		{0,0},                                                             
		{0,0},                                                             
	},                                                                     
	{                                                                      
		{MIN_INT32, MAX_INT32},                                            
		{MIN_UINT32, MAX_UINT32},                                          
	}                                                                      
};                                                                          

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

/*
for i in AAA AAD AAM AAS ADC ADD AND ARPL BOUND BSF BSR BSWAP BT BTS BTR BTC CALL CBW CLC CLD CLI CLTS CMC CMOVcc CMP CMPS CMPXCHG CMPXCHG8B COMSID COMISS CPUID CWD DAA D DEC DIV ENTER ESC FCMOVcc FCOMI FCOMIP FUCOMI FUCOMIP HLT IDIV IMUL IN INC INS INT INTO INVD INVLPG UCOMSID UCOMISS IRET Jcc JCXZ JMP LAHF LAR LDS LES LSS LFS LGS LEA LEAVE LGDT LIDT LLDT LMSW LOCK LODS LOOP LOOPE LOOPNE LSL LTR MONITOR MWAIT MOV MOVS MOVSX MOVZX MUL NEG NOP NOT OR OUT OUTS POP POPA POPF PUSH PUSHA PUSHF RCL_1 RCR_1 RCL_count RCR_count RDMSR RDPMC RDTSC REP REPE REPNE RET ROL_1 ROR_1 ROL_count ROR_count RSM SAHF SAL_1 SAR_1 SHL_1 SHR_1 SAL_count SAR_count SHL_count SHR_count SBB SCAS SETcc SGDT SIDT SLDT SMSW SHLD SHRD STC STD STI STOS STR SUB TEST UD2 VERR VERRW WAIT WBINVD WRMSR XADD XCHG XLAT XOR; do  ys=$(echo $i | tr '[A-Z]' '[a-z]');  echo -e -n "
{ // EMU_INSTR_$i
.instrstr      = \"$ys\",
.test_flags    = 0,
.modify_flags  = 0,
.reset_flags   = 0,
.set_flags     = 0,
.restore_flags = 0,
},"; done
*/

#define FLAG_SET(fl) (1 << (fl))

static struct emu_instruction_eflag_effect instruction_flag_sets[] = 
{
	{ // EMU_INSTR_AAA
		.instrstr      = "aaa",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_AAD
		.instrstr      = "aad",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_AAM
		.instrstr      = "aam",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_AAS
		.instrstr      = "aas",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_ADC
		.instrstr      = "adc",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_ADD
		.instrstr      = "add",
		.test_flags    = 0,
		.modify_flags  = ( FLAG_SET(f_of) | FLAG_SET(f_sf) | FLAG_SET(f_zf) | FLAG_SET(f_af) | FLAG_SET(f_pf) | FLAG_SET(f_cf)),
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_AND
		.instrstr      = "and",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_ARPL
		.instrstr      = "arpl",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_BOUND
		.instrstr      = "bound",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_BSF
		.instrstr      = "bsf",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_BSR
		.instrstr      = "bsr",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_BSWAP
		.instrstr      = "bswap",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_BT
		.instrstr      = "bt",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_BTS
		.instrstr      = "bts",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_BTR
		.instrstr      = "btr",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_BTC
		.instrstr      = "btc",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_CALL
		.instrstr      = "call",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_CBW
		.instrstr      = "cbw",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_CLC
		.instrstr      = "clc",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_CLD
		.instrstr      = "cld",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_CLI
		.instrstr      = "cli",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_CLTS
		.instrstr      = "clts",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_CMC
		.instrstr      = "cmc",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_CMOVcc
		.instrstr      = "cmovcc",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_CMP
		.instrstr      = "cmp",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_CMPS
		.instrstr      = "cmps",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_CMPXCHG
		.instrstr      = "cmpxchg",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_CMPXCHG8B
		.instrstr      = "cmpxchg8b",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_COMSID
		.instrstr      = "comsid",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_COMISS
		.instrstr      = "comiss",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_CPUID
		.instrstr      = "cpuid",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_CWD
		.instrstr      = "cwd",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_DAA
		.instrstr      = "daa",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_DAS
		.instrstr      = "das",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_DEC
		.instrstr      = "dec",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_DIV
		.instrstr      = "div",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_ENTER
		.instrstr      = "enter",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_ESC
		.instrstr      = "esc",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_FCMOVcc
		.instrstr      = "fcmovcc",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_FCOMI
		.instrstr      = "fcomi",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_FCOMIP
		.instrstr      = "fcomip",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_FUCOMI
		.instrstr      = "fucomi",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_FUCOMIP
		.instrstr      = "fucomip",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_HLT
		.instrstr      = "hlt",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_IDIV
		.instrstr      = "idiv",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_IMUL
		.instrstr      = "imul",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_IN
		.instrstr      = "in",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_INC
		.instrstr      = "inc",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_INS
		.instrstr      = "ins",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_INT
		.instrstr      = "int",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_INTO
		.instrstr      = "into",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_INVD
		.instrstr      = "invd",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_INVLPG
		.instrstr      = "invlpg",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_UCOMSID
		.instrstr      = "ucomsid",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_UCOMISS
		.instrstr      = "ucomiss",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_IRET
		.instrstr      = "iret",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_Jcc
		.instrstr      = "jcc",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_JCXZ
		.instrstr      = "jcxz",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_JMP
		.instrstr      = "jmp",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_LAHF
		.instrstr      = "lahf",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_LAR
		.instrstr      = "lar",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_LDS
		.instrstr      = "lds",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_LES
		.instrstr      = "les",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_LSS
		.instrstr      = "lss",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_LFS
		.instrstr      = "lfs",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_LGS
		.instrstr      = "lgs",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_LEA
		.instrstr      = "lea",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_LEAVE
		.instrstr      = "leave",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_LGDT
		.instrstr      = "lgdt",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_LIDT
		.instrstr      = "lidt",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_LLDT
		.instrstr      = "lldt",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_LMSW
		.instrstr      = "lmsw",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_LOCK
		.instrstr      = "lock",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_LODS
		.instrstr      = "lods",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_LOOP
		.instrstr      = "loop",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_LOOPE
		.instrstr      = "loope",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_LOOPNE
		.instrstr      = "loopne",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_LSL
		.instrstr      = "lsl",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_LTR
		.instrstr      = "ltr",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_MONITOR
		.instrstr      = "monitor",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_MWAIT
		.instrstr      = "mwait",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_MOV
		.instrstr      = "mov",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_MOVS
		.instrstr      = "movs",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_MOVSX
		.instrstr      = "movsx",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_MOVZX
		.instrstr      = "movzx",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_MUL
		.instrstr      = "mul",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_NEG
		.instrstr      = "neg",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_NOP
		.instrstr      = "nop",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_NOT
		.instrstr      = "not",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_OR
		.instrstr      = "or",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_OUT
		.instrstr      = "out",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_OUTS
		.instrstr      = "outs",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_POP
		.instrstr      = "pop",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_POPA
		.instrstr      = "popa",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_POPF
		.instrstr      = "popf",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_PUSH
		.instrstr      = "push",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_PUSHA
		.instrstr      = "pusha",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_PUSHF
		.instrstr      = "pushf",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_RCL_1
		.instrstr      = "rcl_1",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_RCR_1
		.instrstr      = "rcr_1",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_RCL_count
		.instrstr      = "rcl_count",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_RCR_count
		.instrstr      = "rcr_count",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_RDMSR
		.instrstr      = "rdmsr",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_RDPMC
		.instrstr      = "rdpmc",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_RDTSC
		.instrstr      = "rdtsc",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_REP
		.instrstr      = "rep",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_REPE
		.instrstr      = "repe",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_REPNE
		.instrstr      = "repne",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_RET
		.instrstr      = "ret",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_ROL_1
		.instrstr      = "rol_1",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_ROR_1
		.instrstr      = "ror_1",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_ROL_count
		.instrstr      = "rol_count",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_ROR_count
		.instrstr      = "ror_count",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_RSM
		.instrstr      = "rsm",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_SAHF
		.instrstr      = "sahf",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_SAL_1
		.instrstr      = "sal_1",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_SAR_1
		.instrstr      = "sar_1",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_SHL_1
		.instrstr      = "shl_1",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_SHR_1
		.instrstr      = "shr_1",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_SAL_count
		.instrstr      = "sal_count",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_SAR_count
		.instrstr      = "sar_count",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_SHL_count
		.instrstr      = "shl_count",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_SHR_count
		.instrstr      = "shr_count",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_SBB
		.instrstr      = "sbb",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_SCAS
		.instrstr      = "scas",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_SETcc
		.instrstr      = "setcc",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_SGDT
		.instrstr      = "sgdt",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_SIDT
		.instrstr      = "sidt",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_SLDT
		.instrstr      = "sldt",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_SMSW
		.instrstr      = "smsw",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_SHLD
		.instrstr      = "shld",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_SHRD
		.instrstr      = "shrd",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_STC
		.instrstr      = "stc",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_STD
		.instrstr      = "std",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_STI
		.instrstr      = "sti",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_STOS
		.instrstr      = "stos",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_STR
		.instrstr      = "str",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_SUB
		.instrstr      = "sub",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_TEST
		.instrstr      = "test",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_UD2
		.instrstr      = "ud2",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_VERR
		.instrstr      = "verr",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_VERRW
		.instrstr      = "verrw",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_WAIT
		.instrstr      = "wait",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_WBINVD
		.instrstr      = "wbinvd",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_WRMSR
		.instrstr      = "wrmsr",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_XADD
		.instrstr      = "xadd",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_XCHG
		.instrstr      = "xchg",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_XLAT
		.instrstr      = "xlat",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	},
	{ // EMU_INSTR_XOR
		.instrstr      = "xor",
		.test_flags    = 0,
		.modify_flags  = 0,
		.reset_flags   = 0,
		.set_flags     = 0,
		.restore_flags = 0,
	}
};

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
	if (sz < max_inttype_borders[sizeof(operation_result)/8][0][0] || sz > max_inttype_borders[sizeof(operation_result)/8][0][1] \
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
	uint64_t ux = (int64_t)operand_a;                                           \
	uint64_t uy = (int64_t)operand_b;                                           \
	uint64_t uz = 0;                                                            \
																				\
	uz = ux operand uy;															\
																				\
	if (uz < max_inttype_borders[sizeof(operation_result)/8][0][0] || uz > max_inttype_borders[sizeof(operation_result)/8][0][1] \
	|| uz != (int64_t)operation_result )									    \
	{                                                                           \
		CPU_FLAG_SET(cpu,f_of);                                                 \
	}else                                                                       \
	{                                                                           \
		CPU_FLAG_UNSET(cpu,f_of);                                               \
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
