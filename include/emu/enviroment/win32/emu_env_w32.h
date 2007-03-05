/* @header@ */
#include <stdint.h>

struct emu;
struct emu_env_w32_dll;

struct emu_env_w32
{
	struct emu *emu;
	struct emu_env_w32_dll **loaded_dlls;
	uint32_t	baseaddr;
};

struct emu_env_w32 *emu_env_w32_new(struct emu *e);
void emu_env_w32_free(struct emu_env_w32 *env);
uint32_t emu_env_w32_load_dll(struct emu_env_w32 *env, char *path);
//uint32_t emu_env_w32_export_hook(struct emu_env_w32 *env, const char *exportname, int32_t (*fnhook)(struct emu *e, struct emu_env_w32 *env, struct emu_env_w32_export *ex));

void emu_env_w32_eip_check(struct emu_env_w32 *env);
