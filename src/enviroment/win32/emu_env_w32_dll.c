/* @header@ */
#include <stdlib.h>
#include <string.h>

#include "emu/emu.h"
#include "emu/enviroment/win32/emu_env_w32_dll.h"
#include "emu/enviroment/win32/emu_env_w32_dll_export.h"

struct emu_env_w32_dll *emu_env_w32_dll_new()
{
	struct emu_env_w32_dll *dll = (struct emu_env_w32_dll *)malloc(sizeof(struct emu_env_w32_dll));
	memset(dll,0,sizeof(struct emu_env_w32_dll));
	return dll;
}

void emu_env_w32_dll_free(struct emu_env_w32_dll *dll)
{
	free(dll);
}

struct emu_env_w32_dll_export * emu_env_w32_dll_export_add(struct emu_env_w32_dll *dll, const char *fnname, uint32_t addr)
{
	struct emu_env_w32_dll_export *exp = emu_env_w32_dll_export_new();

	uint32_t numexports=0;
	if (dll->exports != NULL)
	{
    	while (dll->exports[numexports] != NULL)
			numexports++;
	}

	dll->exports = realloc(dll->exports, sizeof(struct emu_env_w32_dll_export *) * (numexports+1));
	dll->exports[numexports] = exp;

	return exp;
}
