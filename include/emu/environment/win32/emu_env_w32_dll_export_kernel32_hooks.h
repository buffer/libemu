/********************************************************************************
 *                               libemu
 *
 *                    - x86 shellcode emulation -
 *
 *
 * Copyright (C) 2007  Paul Baecher & Markus Koetter
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 * 
 * 
 *             contact nepenthesdev@users.sourceforge.net  
 *
 *******************************************************************************/

#include <stdint.h>

int32_t emu_env_w32_hook_CloseHandle(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex);
int32_t emu_env_w32_hook_CreateFileA(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex);
int32_t emu_env_w32_hook_CreateProcessA(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex);
int32_t emu_env_w32_hook_DeleteFileA(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex);
int32_t emu_env_w32_hook_ExitProcess(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex);
int32_t emu_env_w32_hook_ExitThread(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex);
int32_t emu_env_w32_hook_fclose(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex);
int32_t emu_env_w32_hook_fopen(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex);
int32_t emu_env_w32_hook_fwrite(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex);
int32_t emu_env_w32_hook_GetProcAddress(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex);
int32_t emu_env_w32_hook_GetSystemDirectoryA(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex);
int32_t emu_env_w32_hook_GetTickCount(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex);
int32_t emu_env_w32_hook__hwrite(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex);
int32_t emu_env_w32_hook__lclose(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex);
int32_t emu_env_w32_hook__lcreat(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex);
int32_t emu_env_w32_hook__lwrite(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex);
int32_t emu_env_w32_hook_LoadLibrayA(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex);
int32_t emu_env_w32_hook_malloc(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex);
int32_t emu_env_w32_hook_memset(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex);
int32_t emu_env_w32_hook_SetUnhandledExceptionFilter(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex);
int32_t emu_env_w32_hook_WaitForSingleObject(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex);
int32_t emu_env_w32_hook_WinExec(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex);
int32_t emu_env_w32_hook_WriteFile(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex);

#define HANDLE int32_t
#define DWORD uint32_t
#define  WORD uint16_t

#define LPTSTR char *
#define LPBYTE char *

typedef struct _PROCESS_INFORMATION 
{
	HANDLE hProcess;
	HANDLE hThread;
	DWORD dwProcessId;
	DWORD dwThreadId;
}PROCESS_INFORMATION, *LPPROCESS_INFORMATION;


typedef struct _STARTUPINFO {
  DWORD cb;
  LPTSTR lpReserved;
  LPTSTR lpDesktop;
  LPTSTR lpTitle;
  DWORD dwX;
  DWORD dwY;
  DWORD dwXSize;
  DWORD dwYSize;
  DWORD dwXCountChars;
  DWORD dwYCountChars;
  DWORD dwFillAttribute;
  DWORD dwFlags;
  WORD wShowWindow;
  WORD cbReserved2;
  LPBYTE lpReserved2;
  HANDLE hStdInput;
  HANDLE hStdOutput;
  HANDLE hStdError;
} STARTUPINFO, *LPSTARTUPINFO;

