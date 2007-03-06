/* @header@ */
#include <stdint.h>

#ifndef HAVE_EMU_ENV_W32_DLL_EXPORT_H
#define HAVE_EMU_ENV_W32_DLL_EXPORT_H


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
int32_t	emu_env_w32_hook_WSAStartup(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex);
int32_t	emu_env_w32_hook_WSASocketA(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex);
int32_t	emu_env_w32_hook_bind(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex);
int32_t	emu_env_w32_hook_listen(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex);
int32_t	emu_env_w32_hook_accept(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex);
int32_t	emu_env_w32_hook_closesocket(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex);
int32_t	emu_env_w32_hook_connect(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex);
int32_t	emu_env_w32_hook_CreateProcessA(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex);



extern struct emu_env_w32_dll_export kernel32_exports[];
extern struct emu_env_w32_dll_export ws2_32_exports[];
extern struct emu_env_w32_dll_export wininet_exports[];


#endif
