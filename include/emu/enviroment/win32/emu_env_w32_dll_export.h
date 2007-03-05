/* @header@ */
#include <stdint.h>

struct emu;
struct emu_env_w32;


struct emu_env_w32_dll_export
{
	char 		*fnname;
	uint32_t 	realaddr;
    int32_t		(*fnhook)(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex);
};

struct emu_env_w32_dll_export *emu_env_w32_dll_export_new();
void emu_env_w32_dll_export_free(struct emu_env_w32_dll_export *exp);

int32_t	emu_env_w32_hook_LoadLibrayA(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex);
