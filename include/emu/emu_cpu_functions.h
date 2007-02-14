/* @header@ */
#ifndef HAVE_EMU_CPU_FUNCTIONS_H
#define HAVE_EMU_CPU_FUNCTIONS_H

#define MEM_BYTE_READ(cpu_p, addr, data_p) \
 { uint32_t ret = emu_memory_read_byte((cpu_p)->mem, addr, data_p); \
 if( ret != 0 ) \
  return ret; }

#define MEM_BYTE_WRITE(cpu_p, addr, data) \
 { uint32_t ret = emu_memory_write_byte((cpu_p)->mem, addr, data); \
 if( ret != 0 ) \
  return ret; }

#define MEM_WORD_READ(cpu_p, addr, data_p) \
 { uint32_t ret = emu_memory_read_word((cpu_p)->mem, addr, data_p); \
 if( ret != 0 ) \
  return ret; }

#define MEM_WORD_WRITE(cpu_p, addr, data) \
 { uint32_t ret = emu_memory_write_word((cpu_p)->mem, addr, data); \
 if( ret != 0 ) \
  return ret; }

#define MEM_DWORD_READ(cpu_p, addr, data_p) \
 { uint32_t ret = emu_memory_read_dword((cpu_p)->mem, addr, data_p); \
 if( ret != 0 ) \
  return ret; }

#define MEM_DWORD_WRITE(cpu_p, addr, data) \
 { uint32_t ret = emu_memory_write_dword((cpu_p)->mem, addr, data); \
 if( ret != 0 ) \
  return ret; }

/* misc */
int32_t prefix_fn(struct emu_cpu *c, struct instruction *i);

/* add */
int32_t instr_add_00(struct emu_cpu *c, struct instruction *i);
int32_t instr_add_01(struct emu_cpu *c, struct instruction *i);
int32_t instr_add_02(struct emu_cpu *c, struct instruction *i);
int32_t instr_add_03(struct emu_cpu *c, struct instruction *i);
int32_t instr_add_04(struct emu_cpu *c, struct instruction *i);
int32_t instr_add_05(struct emu_cpu *c, struct instruction *i);

/* or */
int32_t instr_or_08(struct emu_cpu *c, struct instruction *i);
int32_t instr_or_09(struct emu_cpu *c, struct instruction *i);
int32_t instr_or_0a(struct emu_cpu *c, struct instruction *i);
int32_t instr_or_0b(struct emu_cpu *c, struct instruction *i);
int32_t instr_or_0c(struct emu_cpu *c, struct instruction *i);
int32_t instr_or_0d(struct emu_cpu *c, struct instruction *i);


/* adc */
int32_t instr_adc_10(struct emu_cpu *c, struct instruction *i);
int32_t instr_adc_11(struct emu_cpu *c, struct instruction *i);
int32_t instr_adc_12(struct emu_cpu *c, struct instruction *i);
int32_t instr_adc_13(struct emu_cpu *c, struct instruction *i);
int32_t instr_adc_14(struct emu_cpu *c, struct instruction *i);
int32_t instr_adc_15(struct emu_cpu *c, struct instruction *i);

/* sbb */
int32_t instr_sbb_18(struct emu_cpu *c, struct instruction *i);
int32_t instr_sbb_19(struct emu_cpu *c, struct instruction *i);
int32_t instr_sbb_1a(struct emu_cpu *c, struct instruction *i);
int32_t instr_sbb_1b(struct emu_cpu *c, struct instruction *i);
int32_t instr_sbb_1c(struct emu_cpu *c, struct instruction *i);
int32_t instr_sbb_1d(struct emu_cpu *c, struct instruction *i);


/* and */
int32_t instr_and_20(struct emu_cpu *c, struct instruction *i);
int32_t instr_and_21(struct emu_cpu *c, struct instruction *i);
int32_t instr_and_22(struct emu_cpu *c, struct instruction *i);
int32_t instr_and_23(struct emu_cpu *c, struct instruction *i);
int32_t instr_and_24(struct emu_cpu *c, struct instruction *i);
int32_t instr_and_25(struct emu_cpu *c, struct instruction *i);

/* sub */
int32_t instr_sub_28(struct emu_cpu *c, struct instruction *i);
int32_t instr_sub_29(struct emu_cpu *c, struct instruction *i);
int32_t instr_sub_2a(struct emu_cpu *c, struct instruction *i);
int32_t instr_sub_2b(struct emu_cpu *c, struct instruction *i);
int32_t instr_sub_2c(struct emu_cpu *c, struct instruction *i);
int32_t instr_sub_2d(struct emu_cpu *c, struct instruction *i);


/* xor */
int32_t instr_xor_30(struct emu_cpu *c, struct instruction *i);
int32_t instr_xor_31(struct emu_cpu *c, struct instruction *i);
int32_t instr_xor_32(struct emu_cpu *c, struct instruction *i);
int32_t instr_xor_33(struct emu_cpu *c, struct instruction *i);
int32_t instr_xor_34(struct emu_cpu *c, struct instruction *i);
int32_t instr_xor_35(struct emu_cpu *c, struct instruction *i);

/* cmp */
int32_t instr_cmp_38(struct emu_cpu *c, struct instruction *i);
int32_t instr_cmp_39(struct emu_cpu *c, struct instruction *i);
int32_t instr_cmp_3a(struct emu_cpu *c, struct instruction *i);
int32_t instr_cmp_3b(struct emu_cpu *c, struct instruction *i);
int32_t instr_cmp_3c(struct emu_cpu *c, struct instruction *i);
int32_t instr_cmp_3d(struct emu_cpu *c, struct instruction *i);

/* group 1*/
int32_t instr_group_1_80_add(struct emu_cpu *cpu, uint8_t a, uint8_t b, uint8_t *result);
int32_t instr_group_1_80_or (struct emu_cpu *cpu, uint8_t a, uint8_t b, uint8_t *result);
int32_t instr_group_1_80_adc(struct emu_cpu *cpu, uint8_t a, uint8_t b, uint8_t *result);
int32_t instr_group_1_80_sbb(struct emu_cpu *cpu, uint8_t a, uint8_t b, uint8_t *result);
int32_t instr_group_1_80_and(struct emu_cpu *cpu, uint8_t a, uint8_t b, uint8_t *result);
int32_t instr_group_1_80_sub(struct emu_cpu *cpu, uint8_t a, uint8_t b, uint8_t *result);
int32_t instr_group_1_80_xor(struct emu_cpu *cpu, uint8_t a, uint8_t b, uint8_t *result);
int32_t instr_group_1_80_cmp(struct emu_cpu *cpu, uint8_t a, uint8_t b);

/* inc */
int32_t instr_inc_4x(struct emu_cpu *c, struct instruction *i);

/* dec */
int32_t instr_dec_4x(struct emu_cpu *c, struct instruction *i);

/* jcc */
int32_t instr_jcc_70(struct emu_cpu *c, struct instruction *i);
int32_t instr_jcc_71(struct emu_cpu *c, struct instruction *i);
int32_t instr_jcc_72(struct emu_cpu *c, struct instruction *i);
int32_t instr_jcc_73(struct emu_cpu *c, struct instruction *i);
int32_t instr_jcc_74(struct emu_cpu *c, struct instruction *i);
int32_t instr_jcc_75(struct emu_cpu *c, struct instruction *i);
int32_t instr_jcc_76(struct emu_cpu *c, struct instruction *i);
int32_t instr_jcc_77(struct emu_cpu *c, struct instruction *i);
int32_t instr_jcc_78(struct emu_cpu *c, struct instruction *i);
int32_t instr_jcc_79(struct emu_cpu *c, struct instruction *i);
int32_t instr_jcc_7a(struct emu_cpu *c, struct instruction *i);
int32_t instr_jcc_7b(struct emu_cpu *c, struct instruction *i);
int32_t instr_jcc_7c(struct emu_cpu *c, struct instruction *i);
int32_t instr_jcc_7d(struct emu_cpu *c, struct instruction *i);
int32_t instr_jcc_7e(struct emu_cpu *c, struct instruction *i);
int32_t instr_jcc_7f(struct emu_cpu *c, struct instruction *i);
int32_t instr_jcc_e3(struct emu_cpu *c, struct instruction *i);
int32_t instr_jcc_0f_80(struct emu_cpu *c, struct instruction *i); 
int32_t instr_jcc_0f_81(struct emu_cpu *c, struct instruction *i); 
int32_t instr_jcc_0f_82(struct emu_cpu *c, struct instruction *i); 
int32_t instr_jcc_0f_83(struct emu_cpu *c, struct instruction *i); 
int32_t instr_jcc_0f_84(struct emu_cpu *c, struct instruction *i); 
int32_t instr_jcc_0f_85(struct emu_cpu *c, struct instruction *i); 
int32_t instr_jcc_0f_86(struct emu_cpu *c, struct instruction *i); 
int32_t instr_jcc_0f_87(struct emu_cpu *c, struct instruction *i); 
int32_t instr_jcc_0f_88(struct emu_cpu *c, struct instruction *i); 
int32_t instr_jcc_0f_89(struct emu_cpu *c, struct instruction *i); 
int32_t instr_jcc_0f_8a(struct emu_cpu *c, struct instruction *i); 
int32_t instr_jcc_0f_8b(struct emu_cpu *c, struct instruction *i); 
int32_t instr_jcc_0f_8c(struct emu_cpu *c, struct instruction *i); 
int32_t instr_jcc_0f_8d(struct emu_cpu *c, struct instruction *i); 
int32_t instr_jcc_0f_8e(struct emu_cpu *c, struct instruction *i); 
int32_t instr_jcc_0f_8f(struct emu_cpu *c, struct instruction *i); 

/* jump */
int32_t instr_jmp_e9(struct emu_cpu *c, struct instruction *i); 
int32_t instr_jmp_ea(struct emu_cpu *c, struct instruction *i); 
int32_t instr_jmp_eb(struct emu_cpu *c, struct instruction *i); 
int32_t instr_jmp_ff(struct emu_cpu *c, struct instruction *i); 


#endif /*HAVE_EMU_CPU_FUNCTIONS_H*/
