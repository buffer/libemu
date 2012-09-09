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

int32_t env_w32_hook_CloseHandle(struct emu_env *env, struct emu_env_hook *hook);
int32_t env_w32_hook_CreateFileA(struct emu_env *env, struct emu_env_hook *hook);
int32_t env_w32_hook_CreateFileMapping(struct emu_env *env, struct emu_env_hook *hook);
int32_t env_w32_hook_CreateProcessA(struct emu_env *env, struct emu_env_hook *hook);
int32_t env_w32_hook_CreateProcessInternalA(struct emu_env *env, struct emu_env_hook *hook);
int32_t env_w32_hook_DeleteFileA(struct emu_env *env, struct emu_env_hook *hook);
int32_t env_w32_hook_ExitProcess(struct emu_env *env, struct emu_env_hook *hook);
int32_t env_w32_hook_ExitThread(struct emu_env *env, struct emu_env_hook *hook);
int32_t env_w32_hook_GetFileSize(struct emu_env *env, struct emu_env_hook *hook);
int32_t env_w32_hook_GetModuleHandleA(struct emu_env *env, struct emu_env_hook *hook);
int32_t env_w32_hook_GetProcAddress(struct emu_env *env, struct emu_env_hook *hook);
int32_t env_w32_hook_GetSystemDirectoryA(struct emu_env *env, struct emu_env_hook *hook);
int32_t env_w32_hook_GetTempPathA(struct emu_env *env, struct emu_env_hook *hook);
int32_t env_w32_hook_GetTickCount(struct emu_env *env, struct emu_env_hook *hook);
int32_t env_w32_hook_GetVersion(struct emu_env *env, struct emu_env_hook *hook);
int32_t env_w32_hook__hwrite(struct emu_env *env, struct emu_env_hook *hook);
int32_t env_w32_hook__lclose(struct emu_env *env, struct emu_env_hook *hook);
int32_t env_w32_hook__lcreat(struct emu_env *env, struct emu_env_hook *hook);
int32_t env_w32_hook_LoadLibrayA(struct emu_env *env, struct emu_env_hook *hook);
int32_t env_w32_hook__lwrite(struct emu_env *env, struct emu_env_hook *hook);
int32_t env_w32_hook_malloc(struct emu_env *env, struct emu_env_hook *hook);
int32_t env_w32_hook_memset(struct emu_env *env, struct emu_env_hook *hook);
int32_t env_w32_hook_MapViewOfFile(struct emu_env *env, struct emu_env_hook *hook);
int32_t env_w32_hook_SetFilePointer(struct emu_env *env, struct emu_env_hook *hook);
int32_t env_w32_hook_SetUnhandledExceptionFilter(struct emu_env *env, struct emu_env_hook *hook);
int32_t env_w32_hook_Sleep(struct emu_env *env, struct emu_env_hook *hook);
int32_t env_w32_hook_UnmapViewOfFile(struct emu_env *env, struct emu_env_hook *hook);
int32_t env_w32_hook_WaitForSingleObject(struct emu_env *env, struct emu_env_hook *hook);
int32_t env_w32_hook_WinExec(struct emu_env *env, struct emu_env_hook *hook);
int32_t env_w32_hook_WriteFile(struct emu_env *env, struct emu_env_hook *hook);
int32_t env_w32_hook_VirtualProtect(struct emu_env *env, struct emu_env_hook *hook);
int32_t env_w32_hook_VirtualProtectEx(struct emu_env *env, struct emu_env_hook *hook);
int32_t env_w32_hook_TerminateThread(struct emu_env *env, struct emu_env_hook *hook);

#define HANDLE int32_t
#define DWORD uint32_t
#define  WORD uint16_t

#define LPTSTR char *
#define LPBYTE char *

typedef struct _PROCESS_INFORMATION 
{
	HANDLE hProcess;         /* 00 */
	HANDLE hThread;          /* 01 */
	DWORD dwProcessId;       /* 02 */
	DWORD dwThreadId;        /* 03 */
}PROCESS_INFORMATION, *LPPROCESS_INFORMATION;


typedef struct _STARTUPINFO
{
	DWORD cb;				 /* 00 */
	LPTSTR lpReserved;		 /* 01 */
	LPTSTR lpDesktop;		 /* 02 */
	LPTSTR lpTitle;			 /* 03 */
	DWORD dwX;				 /* 04 */
	DWORD dwY;				 /* 05 */
	DWORD dwXSize;			 /* 06 */
	DWORD dwYSize;			 /* 07 */
	DWORD dwXCountChars;	 /* 08 */
	DWORD dwYCountChars;	 /* 09 */
	DWORD dwFillAttribute;	 /* 10 */
	DWORD dwFlags;			 /* 11 */
	WORD wShowWindow;		 /* 12 */
	WORD cbReserved2;		 /* 13 */
	LPBYTE lpReserved2;		 /* 14 */
	HANDLE hStdInput;		 /* 15 */
	HANDLE hStdOutput;		 /* 16 */
	HANDLE hStdError;		 /* 17 */
} STARTUPINFO, *LPSTARTUPINFO;


