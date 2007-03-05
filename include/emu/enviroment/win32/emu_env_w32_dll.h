/* @header@ */
#include <stdint.h>


struct emu_env_w32_dll
{
	char 		*dllname;
	uint32_t	baseaddr;
	uint32_t 	imagesize;

	struct emu_env_w32_dll_hook *exports;
};

struct emu_env_w32_dll *emu_env_w32_dll_new();
void emu_env_w32_dll_free(struct emu_env_w32_dll *dll);

