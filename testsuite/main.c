#include <emu/emu.h>
#include <emu/log.h>

int main()
{
	struct emu *e = emu_new();
	emu_log_level_set(emu_logging_get(e),EMU_LOG_DEBUG);



	emu_free(e);

	return 0;
}
