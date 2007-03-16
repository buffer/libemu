/* @header@ */
#include <malloc.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdint.h>

#include <emu/emu_fpu.h>
#include <emu/emu.h>
#include <emu/emu_log.h>

struct emu_fpu *emu_fpu_new(struct emu *e)
{
	struct emu_fpu *f = (struct emu_fpu *)malloc(sizeof(struct emu_fpu));
	memset(f,0,sizeof(struct emu_fpu));
	f->emu = e;
	// FIXME init fpu_instr
	return f;
}

void emu_fpu_free(struct emu_fpu *f)
{
	free(f);
}

int32_t emu_fpu_parse(struct emu_fpu *f)
{
	return 0;
}

int32_t emu_fpu_step(struct emu_fpu *f)
{
	// FIXME
	return f->fpu_instr_info->function(f,&f->fpu_instr);
}

