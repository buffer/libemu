#include <emu/emu_cpu.h>

struct emu_cpu
{
	struct emu *lib;
	
	uint32_t eip;
	uint32_t regs[8];
};

struct instruction
{
	uint8_t opc;
	uint8_t opc_2nd;
	uint8_t prefixes;

	struct /* mod r/m data */
	{
		union
		{
			uint8_t mod : 2;
			uint8_t x : 2;
		};

		union
		{
			uint8_t reg : 3;
			uint8_t reg1 : 3;
			uint8_t opc : 3;
			uint8_t sreg3 : 3;
			uint8_t y : 3;
		};

		union
		{
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
	} modrm;

	union /* immediate */
    {
		uint8_t s8;
		uint16_t s16;
		uint32_t s32;
	} imm;

	union /* displacement (non-mod-r/m) */
	{
		uint8_t s8;
		uint16_t s16;
		uint32_t s32;
	} disp;
};

#include <emu/emu_cpu_itables.h>
