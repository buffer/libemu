/* @header@ */
#include <stdlib.h>
#include <string.h>

#include "emu/emu.h"
#include "emu/enviroment/win32/emu_env_w32_dll_export.h"


struct emu_env_w32_dll_export *emu_env_w32_dll_export_new()
{
	struct emu_env_w32_dll_export *exp = (struct emu_env_w32_dll_export *)malloc(sizeof(struct emu_env_w32_dll_export));
	memset(exp,0,sizeof(struct emu_env_w32_dll_export));
	return exp;
}

void emu_env_w32_dll_export_free(struct emu_env_w32_dll_export *exp)
{
	free(exp);
}

int32_t	emu_env_w32_hook_LoadLibrayA(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex)
{
	printf("Hook me Captain Cook!\n");
	printf("%s %s:%i\n",__FUNCTION__,__FILE__,__LINE__);
	return 0;
}
