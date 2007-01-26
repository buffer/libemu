#include <emu/emu_cpu.h>

struct emu_cpu
{
	struct emu *lib;
	
	uint32_t eip;
	uint32_t regs[8];
};
