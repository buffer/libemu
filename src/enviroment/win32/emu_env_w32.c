/* @header@ */
#include "emu/emu.h"
#include "emu/enviroment/win32/emu_env_w32.h"

/*
struct emu_env_w32_dll_export;

struct emu_env_w32_dll
{
	char 		*dllname;
	uint32_t	baseaddr;
	uint32_t 	imagesize;

	struct emu_env_w32_dll_hook *exports;
};

struct emu_env_w32_dll *emu_env_w32_dll_new();
void emu_env_w32_dll_free(struct emu_env_w32_dll *dll);

struct emu_env_w32
{
	struct emu_env_w32_dll *loaded_dlls;
	uint32_t	baseaddr;
};




struct emu_env_w32 *emu_env_w32_new();
void emu_env_w32_free(struct emu_env_w32 *env);

uint32_t emu_env_w32_load_dll(struct emu_env_w32 *env, char *path);
uint32_t emu_env_w32_export_hook(struct emu_env_w32 *env, const char *exportname, int32_t (*fnhook)(struct emu *e, struct emu_env_w32 *env, struct emu_env_w32_export *ex));


struct emu_env_w32_dll_export
{
	char 		*fnname;
	uint32_t 	realaddr;
    int32_t		(*fnhook)(struct emu *e, struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex);
};

struct emu_env_w32_dll_export *emu_env_w32_dll_export_new();
void emu_env_w32_dll_export_free(struct emu_env_w32_dll_export *exp);

*/
