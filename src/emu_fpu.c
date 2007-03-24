/* @header@ */
#include <malloc.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdint.h>

#include <emu/emu_fpu.h>
#include <emu/emu.h>
#include <emu/emu_cpu_data.h>
#include <emu/emu_log.h>
#include <emu/emu_memory.h>

struct emu_fpu *emu_fpu_new(struct emu_cpu *c)
{
	struct emu_fpu *f = (struct emu_fpu *)malloc(sizeof(struct emu_fpu));
	memset(f,0,sizeof(struct emu_fpu));
	f->cpu = c;
	// FIXME init fpu_instr
	return f;
}

void emu_fpu_free(struct emu_fpu *f)
{
	free(f);
}

int32_t emu_fpu_parse(struct emu_fpu *f)
{
	uint8_t instr[2];
	
	int disp = 0;
	int sib = 0;
	
	int ret;
	
	ret = emu_memory_read_byte(f->mem, f->cpu->eip++, &instr[0]);
	if( ret != 0 )
		return ret;
	
	ret = emu_memory_read_byte(f->mem, f->cpu->eip++, &instr[1]);
	if( ret != 0 )
		return ret;
	
	if( FPU_MOD(instr) != 3 ) /* intel pdf page 36 */
	{
		if( FPU_MOD(instr) == 1 )
		{
			disp = 1;
		}
		else if( FPU_MOD(instr) == 2 )
		{
			disp = 4;
		}
		else /* mod == 0 */
		{
			if( FPU_RM(instr) == 4 )
			{
				sib = 1;
			}
			else if( FPU_RM(instr) == 5 )
			{
				disp = 4;
			}
		}
	}
	
	
	return 0;
}

int32_t emu_fpu_step(struct emu_fpu *f)
{
	// FIXME
	return f->fpu_instr_info->function(f,&f->fpu_instr);
}

