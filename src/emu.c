#include <emu/emu.h>



struct emu *emu_new()
{
	struct emu *e = (struct emu *)malloc(sizeof(struct emu));
	memset(e,0,sizeof(struct emu));
	return e;
}

