/* @header@ */
#ifndef EMU_CPU_STACK_H_
#define EMU_CPU_STACK_H_

#define PUSH_DWORD(cpu, arg)				\
if (cpu->reg[esp] < 4)								\
{													\
	emu_errno_set((cpu)->emu, ENOMEM);				\
	emu_strerror_set((cpu)->emu,					\
	"ran out of stack space writing a dword\n");	\
	return -1;										\
}													\
cpu->reg[esp]-=4;									\
{																				\
		int32_t memret = emu_memory_write_dword(cpu->mem, cpu->reg[esp], arg);	\
		if (memret != 0)														\
			return memret;														\
}


#define PUSH_WORD(cpu, arg)				\
if (cpu->reg[esp] < 2)								\
{													\
	emu_errno_set((cpu)->emu, ENOMEM);				\
	emu_strerror_set((cpu)->emu,					\
	"ran out of stack space writing a word\n");		\
	return -1;										\
}													\
cpu->reg[esp]-=2;									\
{																				\
		int32_t memret = emu_memory_write_word(cpu->mem, cpu->reg[esp], arg);	\
		if (memret != 0)														\
			return memret;														\
}



#define PUSH_BYTE(cpu, arg)				\
if (cpu->reg[esp] < 1)								\
{													\
	emu_errno_set((cpu)->emu, ENOMEM);				\
	emu_strerror_set((cpu)->emu,					\
	"ran out of stack space writing a byte\n");		\
	return -1;										\
}													\
cpu->reg[esp]-=1;									\
{																				\
		int32_t memret = emu_memory_write_byte(cpu->mem, cpu->reg[esp], arg);	\
		if (memret != 0)														\
			return memret;														\
}


#define POP_DWORD(cpu, dst_p) \
{ int32_t ret = emu_memory_read_dword(cpu->mem, cpu->reg[esp], dst_p); \
if( ret != 0 ) \
	return ret; \
else \
	cpu->reg[esp] += 4; }

#define POP_WORD(cpu, dst_p) \
{ int32_t ret = emu_memory_read_word(cpu->mem, cpu->reg[esp], dst_p); \
if( ret != 0 ) \
	return ret; \
else \
	cpu->reg[esp] += 2; }

#define POP_BYTE(cpu, dst_p) \
{ int32_t ret = emu_memory_read_byte(cpu->mem, cpu->reg[esp], dst_p); \
if( ret != 0 ) \
	return ret; \
else \
	cpu->reg[esp] += 1; }



#endif /*EMU_CPU_STACK_H_*/
