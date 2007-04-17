/* @header@ */
#include <stdint.h>

#ifndef HAVE_EMU_ENV_W32_DLL_EXPORT_H
#define HAVE_EMU_ENV_W32_DLL_EXPORT_H


struct emu;
struct emu_env_w32;


struct emu_env_w32_dll_export
{
	char 		*fnname;
	uint32_t 	virtualaddr;
    int32_t		(*fnhook)(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex);
};

struct emu_env_w32_dll_export *emu_env_w32_dll_export_new();
void emu_env_w32_dll_export_free(struct emu_env_w32_dll_export *exp);
void emu_env_w32_dll_export_copy(struct emu_env_w32_dll_export *to, struct emu_env_w32_dll_export *from);

int32_t	emu_env_w32_hook_GetProcAddress(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex);
int32_t	emu_env_w32_hook_LoadLibrayA(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex);
int32_t	emu_env_w32_hook_WSAStartup(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex);
int32_t	emu_env_w32_hook_WSASocketA(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex);
int32_t	emu_env_w32_hook_socket(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex);
int32_t	emu_env_w32_hook_bind(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex);
int32_t	emu_env_w32_hook_listen(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex);
int32_t	emu_env_w32_hook_accept(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex);
int32_t	emu_env_w32_hook_closesocket(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex);
int32_t	emu_env_w32_hook_connect(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex);
int32_t	emu_env_w32_hook_recv(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex);
int32_t	emu_env_w32_hook_send(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex);
int32_t	emu_env_w32_hook_CreateProcessA(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex);
int32_t	emu_env_w32_hook_WinExec(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex);
int32_t	emu_env_w32_hook_WaitForSingleObject(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex);
int32_t	emu_env_w32_hook_ExitProcess(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex);
int32_t	emu_env_w32_hook_ExitThread(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex);

int32_t	emu_env_w32_hook_CreateFileA(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex);
int32_t	emu_env_w32_hook_WriteFile(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex);
int32_t	emu_env_w32_hook_CloseHandle(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex);
int32_t	emu_env_w32_hook_DeleteFileA(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex);

int32_t	emu_env_w32_hook__lcreat(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex);
int32_t	emu_env_w32_hook__hwrite(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex);
int32_t	emu_env_w32_hook__lwrite(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex);
int32_t	emu_env_w32_hook__lclose(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex);


int32_t	emu_env_w32_hook_malloc(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex);
int32_t	emu_env_w32_hook_memset(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex);
int32_t	emu_env_w32_hook_fopen(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex);
int32_t	emu_env_w32_hook_fwrite(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex);
int32_t	emu_env_w32_hook_fclose(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex);

int32_t	emu_env_w32_hook_URLDownloadToFileA(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex);

extern struct emu_env_w32_dll_export kernel32_exports[];
extern struct emu_env_w32_dll_export ws2_32_exports[];
extern struct emu_env_w32_dll_export wininet_exports[];
extern struct emu_env_w32_dll_export urlmon_exports[];


#endif
