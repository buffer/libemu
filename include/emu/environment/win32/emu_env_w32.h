/* @header@ */
#include <stdint.h>

struct emu;
struct emu_env_w32_dll;
struct emu_env_w32_dll_export;

struct emu_env_w32
{
	struct emu *emu;
	struct emu_env_w32_dll **loaded_dlls;
	uint32_t	baseaddr;
};

struct emu_env_w32 *emu_env_w32_new(struct emu *e);
void emu_env_w32_free(struct emu_env_w32 *env);
int32_t emu_env_w32_load_dll(struct emu_env_w32 *env, char *path);

int32_t emu_env_w32_export_hook(struct emu_env_w32 *env,
								const char *dllname,
								const char *exportname, 
								int32_t		(*fnhook)(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex));


struct emu_env_w32_dll_export *emu_env_w32_eip_check(struct emu_env_w32 *env);
