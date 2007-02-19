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

/* group 1 */
int32_t instr_group_1_80_add(struct emu_cpu *cpu, uint8_t a, uint8_t b, uint8_t *result);
int32_t instr_group_1_80_or (struct emu_cpu *cpu, uint8_t a, uint8_t b, uint8_t *result);
int32_t instr_group_1_80_adc(struct emu_cpu *cpu, uint8_t a, uint8_t b, uint8_t *result);
int32_t instr_group_1_80_sbb(struct emu_cpu *cpu, uint8_t a, uint8_t b, uint8_t *result);
int32_t instr_group_1_80_and(struct emu_cpu *cpu, uint8_t a, uint8_t b, uint8_t *result);
int32_t instr_group_1_80_sub(struct emu_cpu *cpu, uint8_t a, uint8_t b, uint8_t *result);
int32_t instr_group_1_80_xor(struct emu_cpu *cpu, uint8_t a, uint8_t b, uint8_t *result);
int32_t instr_group_1_80_cmp(struct emu_cpu *cpu, uint8_t a, uint8_t b);

/* group 5 */
int32_t instr_group_5_ff(struct emu_cpu *c, struct instruction *i); 

/* group 10 */
/*int32_t instr_group_10_8f_pop(struct emu_cpu *c, struct instruction *i);*/

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
int32_t instr_jcc_0f80(struct emu_cpu *c, struct instruction *i); 
int32_t instr_jcc_0f81(struct emu_cpu *c, struct instruction *i); 
int32_t instr_jcc_0f82(struct emu_cpu *c, struct instruction *i); 
int32_t instr_jcc_0f83(struct emu_cpu *c, struct instruction *i); 
int32_t instr_jcc_0f84(struct emu_cpu *c, struct instruction *i); 
int32_t instr_jcc_0f85(struct emu_cpu *c, struct instruction *i); 
int32_t instr_jcc_0f86(struct emu_cpu *c, struct instruction *i); 
int32_t instr_jcc_0f87(struct emu_cpu *c, struct instruction *i); 
int32_t instr_jcc_0f88(struct emu_cpu *c, struct instruction *i); 
int32_t instr_jcc_0f89(struct emu_cpu *c, struct instruction *i); 
int32_t instr_jcc_0f8a(struct emu_cpu *c, struct instruction *i); 
int32_t instr_jcc_0f8b(struct emu_cpu *c, struct instruction *i); 
int32_t instr_jcc_0f8c(struct emu_cpu *c, struct instruction *i); 
int32_t instr_jcc_0f8d(struct emu_cpu *c, struct instruction *i); 
int32_t instr_jcc_0f8e(struct emu_cpu *c, struct instruction *i); 
int32_t instr_jcc_0f8f(struct emu_cpu *c, struct instruction *i);

/* jump */
int32_t instr_jmp_e9(struct emu_cpu *c, struct instruction *i); 
/*int32_t instr_jmp_ea(struct emu_cpu *c, struct instruction *i);*/ 
int32_t instr_jmp_eb(struct emu_cpu *c, struct instruction *i);

/* mov */
int32_t instr_mov_88(struct emu_cpu *c, struct instruction *i);
int32_t instr_mov_89(struct emu_cpu *c, struct instruction *i);
int32_t instr_mov_8a(struct emu_cpu *c, struct instruction *i);
int32_t instr_mov_8b(struct emu_cpu *c, struct instruction *i);
/*int32_t instr_mov_8c(struct emu_cpu *c, struct instruction *i);
int32_t instr_mov_8e(struct emu_cpu *c, struct instruction *i);
int32_t instr_mov_a0(struct emu_cpu *c, struct instruction *i);
int32_t instr_mov_a1(struct emu_cpu *c, struct instruction *i);
int32_t instr_mov_a2(struct emu_cpu *c, struct instruction *i);
int32_t instr_mov_a3(struct emu_cpu *c, struct instruction *i);*/
int32_t instr_mov_bx_1(struct emu_cpu *c, struct instruction *i);
int32_t instr_mov_bx_2(struct emu_cpu *c, struct instruction *i);
int32_t instr_mov_c6(struct emu_cpu *c, struct instruction *i);
int32_t instr_mov_c7(struct emu_cpu *c, struct instruction *i);
															  
/* pop */
/*int32_t instr_pop_07(struct emu_cpu *c, struct instruction *i);
int32_t instr_pop_17(struct emu_cpu *c, struct instruction *i);
int32_t instr_pop_1f(struct emu_cpu *c, struct instruction *i);*/
int32_t instr_pop_5x(struct emu_cpu *c, struct instruction *i);
int32_t instr_popad_61(struct emu_cpu *c, struct instruction *i);
/*int32_t instr_pop_0fa1(struct emu_cpu *c, struct instruction *i);
int32_t instr_pop_0fa9(struct emu_cpu *c, struct instruction *i);*/
																 
/* push */                                                       
/*int32_t instr_push_06(struct emu_cpu *c, struct instruction *i);  
int32_t instr_push_0e(struct emu_cpu *c, struct instruction *i);  
int32_t instr_push_16(struct emu_cpu *c, struct instruction *i);
int32_t instr_push_1e(struct emu_cpu *c, struct instruction *i);*/
int32_t instr_push_5x(struct emu_cpu *c, struct instruction *i);
int32_t instr_pushad_60(struct emu_cpu *c, struct instruction *i);
int32_t instr_push_68(struct emu_cpu *c, struct instruction *i);
int32_t instr_push_6a(struct emu_cpu *c, struct instruction *i);
/*int32_t instr_push_0fa0(struct emu_cpu *c, struct instruction *i);
int32_t instr_push_0f08(struct emu_cpu *c, struct instruction *i);*/

/* call */
/*int32_t instr_call_9a(struct emu_cpu *c, struct instruction *i);
int32_t instr_call_e8(struct emu_cpu *c, struct instruction *i);*/


/* loop/loopcc*/
/*int32_t instr_loopcc_e0(struct emu_cpu *c, struct instruction *i);
int32_t instr_loopcc_e1(struct emu_cpu *c, struct instruction *i);
int32_t instr_loop_e2(struct emu_cpu *c, struct instruction *i); */

/* ret */
int32_t instr_ret_c2(struct emu_cpu *c, struct instruction *i);
int32_t instr_ret_c3(struct emu_cpu *c, struct instruction *i);

/* stos/stoscc*/
/*int32_t instr_stos_aa(struct emu_cpu *c, struct instruction *i);
int32_t instr_stos_ab(struct emu_cpu *c, struct instruction *i);*/

#endif /*HAVE_EMU_CPU_FUNCTIONS_H*/                           
															  
