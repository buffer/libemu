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

#ifndef HAVE_EMU_CPU_FUNCTIONS_H
#define HAVE_EMU_CPU_FUNCTIONS_H


/* misc */
int32_t prefix_fn(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_cpuid_0fa2(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_salc_d6(struct emu_cpu *c, struct emu_cpu_instruction *i);

/* add */
int32_t instr_add_00(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_add_01(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_add_02(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_add_03(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_add_04(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_add_05(struct emu_cpu *c, struct emu_cpu_instruction *i);

/* or */
int32_t instr_or_08(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_or_09(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_or_0a(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_or_0b(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_or_0c(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_or_0d(struct emu_cpu *c, struct emu_cpu_instruction *i);


/* adc */
int32_t instr_adc_10(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_adc_11(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_adc_12(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_adc_13(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_adc_14(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_adc_15(struct emu_cpu *c, struct emu_cpu_instruction *i);

/* sbb */
int32_t instr_sbb_18(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_sbb_19(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_sbb_1a(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_sbb_1b(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_sbb_1c(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_sbb_1d(struct emu_cpu *c, struct emu_cpu_instruction *i);


/* and */
int32_t instr_and_20(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_and_21(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_and_22(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_and_23(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_and_24(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_and_25(struct emu_cpu *c, struct emu_cpu_instruction *i);

/* sub */
int32_t instr_sub_28(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_sub_29(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_sub_2a(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_sub_2b(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_sub_2c(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_sub_2d(struct emu_cpu *c, struct emu_cpu_instruction *i);


/* xor */
int32_t instr_xor_30(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_xor_31(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_xor_32(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_xor_33(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_xor_34(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_xor_35(struct emu_cpu *c, struct emu_cpu_instruction *i);

/* cmp */
int32_t instr_cmp_38(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_cmp_39(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_cmp_3a(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_cmp_3b(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_cmp_3c(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_cmp_3d(struct emu_cpu *c, struct emu_cpu_instruction *i);

/* group 1 */
int32_t instr_group_1_80(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_group_1_80_add(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_group_1_80_or (struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_group_1_80_adc(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_group_1_80_sbb(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_group_1_80_and(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_group_1_80_sub(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_group_1_80_xor(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_group_1_80_cmp(struct emu_cpu *c, struct emu_cpu_instruction *i);

int32_t instr_group_1_81(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_group_1_81_add(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_group_1_81_or(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_group_1_81_adc(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_group_1_81_sbb(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_group_1_81_and(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_group_1_81_sub(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_group_1_81_xor(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_group_1_81_cmp(struct emu_cpu *c, struct emu_cpu_instruction *i);

int32_t instr_group_1_83(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_group_1_83_add(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_group_1_83_or(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_group_1_83_adc(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_group_1_83_sbb(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_group_1_83_and(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_group_1_83_sub(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_group_1_83_xor(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_group_1_83_cmp(struct emu_cpu *c, struct emu_cpu_instruction *i);

/* group 3 */
int32_t instr_group_3_f6(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_group_3_f6_test(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_group_3_f6_test(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_group_3_f6_not(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_group_3_f6_neg(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_group_3_f6_mul(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_group_3_f6_imul(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_group_3_f6_div(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_group_3_f6_idiv(struct emu_cpu *c, struct emu_cpu_instruction *i);

int32_t instr_group_3_f7(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_group_3_f7_test(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_group_3_f7_test(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_group_3_f7_not(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_group_3_f7_neg(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_group_3_f7_mul(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_group_3_f7_imul(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_group_3_f7_div(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_group_3_f7_idiv(struct emu_cpu *c, struct emu_cpu_instruction *i);

/* group 4 */
int32_t instr_group_4_fe(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_group_4_fe_inc(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_group_4_fe_dec(struct emu_cpu *c, struct emu_cpu_instruction *i);




/* group 5 */
int32_t instr_group_5_ff(struct emu_cpu *c, struct emu_cpu_instruction *i); 
int32_t instr_group_5_ff_inc(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_group_5_ff_dec(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_group_5_ff_call(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_group_5_ff_jmp(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_group_5_ff_push(struct emu_cpu *c, struct emu_cpu_instruction *i);

/* group 10 */
int32_t instr_group_10_8f(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_group_10_8f_pop(struct emu_cpu *c, struct emu_cpu_instruction *i);

/* inc */
int32_t instr_inc_4x(struct emu_cpu *c, struct emu_cpu_instruction *i);

/* dec */
int32_t instr_dec_4x(struct emu_cpu *c, struct emu_cpu_instruction *i);

/* jcc */
int32_t instr_jcc_70(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_jcc_71(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_jcc_72(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_jcc_73(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_jcc_74(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_jcc_75(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_jcc_76(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_jcc_77(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_jcc_78(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_jcc_79(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_jcc_7a(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_jcc_7b(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_jcc_7c(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_jcc_7d(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_jcc_7e(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_jcc_7f(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_jcc_e3(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_jcc_0f80(struct emu_cpu *c, struct emu_cpu_instruction *i); 
int32_t instr_jcc_0f81(struct emu_cpu *c, struct emu_cpu_instruction *i); 
int32_t instr_jcc_0f82(struct emu_cpu *c, struct emu_cpu_instruction *i); 
int32_t instr_jcc_0f83(struct emu_cpu *c, struct emu_cpu_instruction *i); 
int32_t instr_jcc_0f84(struct emu_cpu *c, struct emu_cpu_instruction *i); 
int32_t instr_jcc_0f85(struct emu_cpu *c, struct emu_cpu_instruction *i); 
int32_t instr_jcc_0f86(struct emu_cpu *c, struct emu_cpu_instruction *i); 
int32_t instr_jcc_0f87(struct emu_cpu *c, struct emu_cpu_instruction *i); 
int32_t instr_jcc_0f88(struct emu_cpu *c, struct emu_cpu_instruction *i); 
int32_t instr_jcc_0f89(struct emu_cpu *c, struct emu_cpu_instruction *i); 
int32_t instr_jcc_0f8a(struct emu_cpu *c, struct emu_cpu_instruction *i); 
int32_t instr_jcc_0f8b(struct emu_cpu *c, struct emu_cpu_instruction *i); 
int32_t instr_jcc_0f8c(struct emu_cpu *c, struct emu_cpu_instruction *i); 
int32_t instr_jcc_0f8d(struct emu_cpu *c, struct emu_cpu_instruction *i); 
int32_t instr_jcc_0f8e(struct emu_cpu *c, struct emu_cpu_instruction *i); 
int32_t instr_jcc_0f8f(struct emu_cpu *c, struct emu_cpu_instruction *i);

int32_t instr_setcc_0f94(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_setcc_0f95(struct emu_cpu *c, struct emu_cpu_instruction *i);

/* jump */
int32_t instr_jmp_e9(struct emu_cpu *c, struct emu_cpu_instruction *i); 
/*int32_t instr_jmp_ea(struct emu_cpu *c, struct instruction *i);*/ 
int32_t instr_jmp_eb(struct emu_cpu *c, struct emu_cpu_instruction *i);

/* mov */
int32_t instr_mov_88(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_mov_89(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_mov_8a(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_mov_8b(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_mov_8c(struct emu_cpu *c, struct emu_cpu_instruction *i);
/*int32_t instr_mov_8e(struct emu_cpu *c, struct instruction *i);*/
int32_t instr_mov_a0(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_mov_a1(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_mov_a2(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_mov_a3(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_movsb(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_mov_bx_1(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_mov_bx_2(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_mov_c6(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_mov_c7(struct emu_cpu *c, struct emu_cpu_instruction *i);

int32_t instr_leave(struct emu_cpu *c, struct emu_cpu_instruction *i);
															  
/* pop */
/*int32_t instr_pop_07(struct emu_cpu *c, struct instruction *i);
int32_t instr_pop_17(struct emu_cpu *c, struct instruction *i);
int32_t instr_pop_1f(struct emu_cpu *c, struct instruction *i);*/
int32_t instr_pop_5x(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_popad_61(struct emu_cpu *c, struct emu_cpu_instruction *i);
/*int32_t instr_pop_0fa1(struct emu_cpu *c, struct instruction *i);
int32_t instr_pop_0fa9(struct emu_cpu *c, struct instruction *i);*/
																 
/* push */                                                       
/*int32_t instr_push_06(struct emu_cpu *c, struct instruction *i);  
int32_t instr_push_0e(struct emu_cpu *c, struct instruction *i);  
int32_t instr_push_16(struct emu_cpu *c, struct instruction *i);
int32_t instr_push_1e(struct emu_cpu *c, struct instruction *i);*/
int32_t instr_push_5x(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_pushad_60(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_push_68(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_push_6a(struct emu_cpu *c, struct emu_cpu_instruction *i);
/*int32_t instr_push_0fa0(struct emu_cpu *c, struct instruction *i);
int32_t instr_push_0f08(struct emu_cpu *c, struct instruction *i);*/

/* call */
/*int32_t instr_call_9a(struct emu_cpu *c, struct instruction *i);*/
int32_t instr_call_e8(struct emu_cpu *c, struct emu_cpu_instruction *i);


/* loop/loopcc*/
int32_t instr_loopcc_e0(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_loopcc_e1(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_loop_e2(struct emu_cpu *c, struct emu_cpu_instruction *i); 

/* ret */
int32_t instr_ret_c2(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_ret_c3(struct emu_cpu *c, struct emu_cpu_instruction *i);

/* stos/stoscc*/
int32_t instr_stos_aa(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_stos_ab(struct emu_cpu *c, struct emu_cpu_instruction *i);

/* lods/lodscc*/
int32_t instr_lods_ac(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_lods_ad(struct emu_cpu *c, struct emu_cpu_instruction *i);

/* aaa */
int32_t instr_aaa_37(struct emu_cpu *c, struct emu_cpu_instruction *i);


/* imul */
/*int32_t instr_imul_69(struct emu_cpu *c, struct instruction *i);*/
int32_t instr_imul_6b(struct emu_cpu *c, struct emu_cpu_instruction *i);
/*int32_t instr_imul_0f_af(struct emu_cpu *c, struct instruction *i);*/

/* misc */
int32_t instr_daa_27(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_das_2f(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_aas_3f(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_lea_8d(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_cbw_98(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_cwd_99(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_wait_9b(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_pushf_9c(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_popf_9d(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_sahf_9e(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_lahf_9f(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_cmc_f5(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_clc_f8(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_stc_f9(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_cld_fc(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_std_fd(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_sldt_0f00(struct emu_cpu *c, struct emu_cpu_instruction *i);

/* group 2*/
int32_t instr_group_2_c0(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_group_2_c0_ror(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_group_2_c0_rol(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_group_2_c0_rcr(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_group_2_c0_rcl(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_group_2_c0_sal(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_group_2_c0_sar(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_group_2_c0_shr(struct emu_cpu *c, struct emu_cpu_instruction *i);


int32_t instr_group_2_c1(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_group_2_c1_ror(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_group_2_c1_rol(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_group_2_c1_rcr(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_group_2_c1_rcl(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_group_2_c1_sal(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_group_2_c1_sar(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_group_2_c1_shr(struct emu_cpu *c, struct emu_cpu_instruction *i);


int32_t instr_group_2_d0(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_group_2_d0_ror(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_group_2_d0_rol(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_group_2_d0_rcr(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_group_2_d0_rcl(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_group_2_d0_sal(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_group_2_d0_sar(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_group_2_d0_shr(struct emu_cpu *c, struct emu_cpu_instruction *i);


int32_t instr_group_2_d1(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_group_2_d1_ror(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_group_2_d1_rol(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_group_2_d1_rcr(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_group_2_d1_rcl(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_group_2_d1_sal(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_group_2_d1_sar(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_group_2_d1_shr(struct emu_cpu *c, struct emu_cpu_instruction *i);


int32_t instr_group_2_d2(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_group_2_d2_ror(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_group_2_d2_rol(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_group_2_d2_rcr(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_group_2_d2_rcl(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_group_2_d2_sal(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_group_2_d2_sar(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_group_2_d2_shr(struct emu_cpu *c, struct emu_cpu_instruction *i);


int32_t instr_group_2_d3(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_group_2_d3_ror(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_group_2_d3_rol(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_group_2_d3_rcr(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_group_2_d3_rcl(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_group_2_d3_sal(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_group_2_d3_sar(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_group_2_d3_shr(struct emu_cpu *c, struct emu_cpu_instruction *i);


/* repcc */
/*int32_t instr_repcc_f2a6(struct emu_cpu *c, struct instruction *i);
int32_t instr_repcc_f2a7(struct emu_cpu *c, struct instruction *i);
int32_t instr_repcc_f2ae(struct emu_cpu *c, struct instruction *i);
int32_t instr_repcc_f2af(struct emu_cpu *c, struct instruction *i);
int32_t instr_repcc_f36c(struct emu_cpu *c, struct instruction *i);
int32_t instr_repcc_f36d(struct emu_cpu *c, struct instruction *i);
int32_t instr_repcc_f36e(struct emu_cpu *c, struct instruction *i);
int32_t instr_repcc_f36f(struct emu_cpu *c, struct instruction *i);
int32_t instr_repcc_f3a4(struct emu_cpu *c, struct instruction *i);
int32_t instr_repcc_f3a5(struct emu_cpu *c, struct instruction *i);
int32_t instr_repcc_f3aa(struct emu_cpu *c, struct instruction *i);
int32_t instr_repcc_f3ab(struct emu_cpu *c, struct instruction *i);
int32_t instr_repcc_f3ac(struct emu_cpu *c, struct instruction *i);
int32_t instr_repcc_f3ad(struct emu_cpu *c, struct instruction *i);
int32_t instr_repcc_f3a6(struct emu_cpu *c, struct instruction *i);
int32_t instr_repcc_f3a7(struct emu_cpu *c, struct instruction *i);
int32_t instr_repcc_f3ae(struct emu_cpu *c, struct instruction *i);
int32_t instr_repcc_f3af(struct emu_cpu *c, struct instruction *i); */

/* test */
int32_t instr_test_84(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_test_85(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_test_a8(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_test_a9(struct emu_cpu *c, struct emu_cpu_instruction *i);

/* scas */
int32_t instr_scas_ae(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_scas_af(struct emu_cpu *c, struct emu_cpu_instruction *i);

/* cmps */
int32_t instr_cmps_a6(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_cmps_a7(struct emu_cpu *c, struct emu_cpu_instruction *i); 

/* xchg */
int32_t instr_xchg_86(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_xchg_87(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_xchg_9x(struct emu_cpu *c, struct emu_cpu_instruction *i);

/* movsx */
int32_t instr_movsx_0fbe(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_movsx_0fbf(struct emu_cpu *c, struct emu_cpu_instruction *i);

/* movzx */
int32_t instr_movzx_0fb6(struct emu_cpu *c, struct emu_cpu_instruction *i);
int32_t instr_movzx_0fb7(struct emu_cpu *c, struct emu_cpu_instruction *i);

/* fpu esc */
int32_t instr_esc_fpu_dx(struct emu_cpu *c, struct emu_cpu_instruction *i);

/* int */
int32_t instr_int_cd(struct emu_cpu *c, struct emu_cpu_instruction *i);

#endif /*HAVE_EMU_CPU_FUNCTIONS_H*/                           
															  
