#ifndef HAVE_EMU_CPU_FUNCTIONS_H
#define HAVE_EMU_CPU_FUNCTIONS_H

#define CPU_REG16_GET(cpu_p, reg) \
 (uint16_t)(((cpu_p)->regs[(reg)]) & 0xffff)

#define CPU_REG16_SET(cpu_p, reg, val) \
 (cpu_p)->regs[(reg)] = (((cpu_p)->regs[(reg)] & 0xffff0000) | ((val) & 0xffff))

#define CPU_REG32_GET(cpu_p, reg) \
 ((cpu_p)->regs[(reg)])

#define CPU_REG32_SET(cpu_p, reg, val) \
 (cpu_p)->regs[(reg)] = (val)

#define CPU_REG8_GET(cpu_p, reg) \
 (uint8_t)((((reg) & 0x4) == 0) ? ((cpu_p)->regs[(reg)] & 0xff) : (((cpu_p)->regs[((reg) & 0x3)] & 0xff00) >> 8))

#define CPU_REG8_SET(cpu_p, reg, val) \
 if( ((reg) & 0x4) == 0 ) \
  (cpu_p)->regs[(reg)] = ((cpu_p)->regs[(reg)] & 0xffffff00) | ((val) & 0xff); \
 else \
  (cpu_p)->regs[((reg) & 0x4)] = ((cpu_p)->regs[((reg) & 0x4)] & 0xffff00ff) | (((val) & 0xff) << 8);

#define MEM_BYTE_READ(cpu_p, addr, data_p) \
 { uint32_t ret = emu_memory_read_byte((cpu_p)->mem, addr, data_p); \
 if( ret != 0 ) \
  return ret; }

#define MEM_BYTE_WRITE(cpu_p, addr, data) \
 { uint32_t ret = emu_memory_write_byte((cpu_p)->mem, addr, data); \
 if( ret != 0 ) \
  return ret; }

uint32_t prefix_fn(struct emu_cpu *c, struct instruction *i)
{
	/* dummy */
	return 0;
}

uint32_t instr_add_00(struct emu_cpu *c, struct instruction *i)
{
	if( i->modrm.mod != 3 )
	{
		uint8_t dst;
		MEM_BYTE_READ(c, i->modrm.ea, &dst);
		uint8_t op = CPU_REG8_GET(c, i->modrm.opc);
		
		dst += op;
		
		MEM_BYTE_WRITE(c, i->modrm.ea, dst);
	}
	else
	{
		uint8_t dst = CPU_REG8_GET(c, i->modrm.rm);
		uint8_t op = CPU_REG8_GET(c, i->modrm.opc);
		
		dst += op;
		
		CPU_REG8_SET(c, i->modrm.rm, dst);
	}
	
	return 0;
}

uint32_t instr_add_04(struct emu_cpu *c, struct instruction *i)
{
	uint8_t byte = CPU_REG8_GET(c, eax);
	byte += (uint8_t)i->imm;
	CPU_REG8_SET(c, eax, byte);
	return 0;
}

uint32_t instr_add_05(struct emu_cpu *c, struct instruction *i)
{
	if( i->prefixes & PREFIX_OPSIZE )
	{
		uint16_t imm16 = (uint16_t)i->imm + (uint16_t)CPU_REG32_GET(c, eax);
		CPU_REG16_SET(c, eax, imm16);
	}
	else
	{
		uint32_t imm32 = i->imm + CPU_REG32_GET(c, eax);
		CPU_REG32_SET(c, eax, imm32);
	}
	
	return 0;
}


#endif /*HAVE_EMU_CPU_FUNCTIONS_H*/
