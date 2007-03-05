/* @header@ */
#include <stdint.h>


struct emu_env_w32_dll
{
	char 		*dllname;

	char 		*image;
	uint32_t	imagesize;

	uint32_t	baseaddr;

	struct emu_env_w32_dll_hook *exports;
};

struct emu_env_w32_dll *emu_env_w32_dll_new();
void emu_env_w32_dll_free(struct emu_env_w32_dll *dll);

