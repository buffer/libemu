#include <emu/emu.h>

int main()
{
	struct emu *e = emu_new();



	emu_free(e);

	return 0;
}
