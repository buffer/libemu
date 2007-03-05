/* @header@ */
#include <stdlib.h>
#include <string.h>

#include "emu/emu.h"
#include "emu/enviroment/win32/emu_env_w32.h"





struct emu_env_w32 *emu_env_w32_new(struct emu *e)
{
	struct emu_env_w32 *env = (struct emu_env_w32 *)malloc(sizeof(struct emu_env_w32));
	memset(env,0,sizeof(struct emu_env_w32));
	env->emu = e;
	return env;
}

void emu_env_w32_free(struct emu_env_w32 *env)
{
	// FIXME free the dlls
	free(env);
}

uint32_t emu_env_w32_load_dll(struct emu_env_w32 *env, char *path)
{

	return 0;
}

