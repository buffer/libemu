#include <stdint.h>

struct instr_test
{
	const char *instr;

	char  *code;
	uint16_t codesize;

	struct 
	{
		uint32_t reg[8];
		uint32_t        mem_state[2];
		uint32_t    eflags;
	} in_state;

	struct 
	{
		uint32_t reg[8];
		uint32_t        mem_state[2];
		uint32_t    eflags;
		uint32_t eip;
	} out_state;
};

#define FLAG(fl) (1 << (fl))


extern struct instr_test tests[];

int numtests(void);
