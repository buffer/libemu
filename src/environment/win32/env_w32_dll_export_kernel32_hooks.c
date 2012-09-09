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

#include <stdlib.h>
#include <string.h>

// for the socket hooks
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>

#include "../../../config.h"
#include "emu/emu.h"
#include "emu/emu_log.h"
#include "emu/emu_memory.h"
#include "emu/emu_cpu.h"
#include "emu/emu_cpu_data.h"
#include "emu/emu_cpu_stack.h"
#include "emu/emu_hashtable.h"
#include "emu/emu_string.h"
#include "emu/environment/emu_env.h"
#include "emu/environment/emu_profile.h"
#include "emu/environment/win32/emu_env_w32.h"
#include "emu/environment/win32/emu_env_w32_dll.h"
#include "emu/environment/win32/emu_env_w32_dll_export.h"
#include "emu/environment/win32/env_w32_dll_export_kernel32_hooks.h"


int32_t	env_w32_hook_CloseHandle(struct emu_env *env, struct emu_env_hook *hook)
{
	logDebug(env->emu, "Hook me Captain Cook!\n");
	logDebug(env->emu, "%s:%i %s\n",__FILE__,__LINE__,__FUNCTION__);

	struct emu_cpu *c = emu_cpu_get(env->emu);

	uint32_t eip_save;

	POP_DWORD(c, &eip_save);

	
	
/*
BOOL CloseHandle(
  HANDLE hObject
);
*/


	uint32_t object;
	POP_DWORD(c, &object);



	uint32_t returnvalue;
	if ( hook->hook.win->userhook != NULL )
	{
		returnvalue = hook->hook.win->userhook(env, hook, 
											   object);
	}else
	{
		returnvalue	= 0;
	}

	if ( env->profile != NULL )
	{
		emu_profile_function_add(env->profile, "CloseHandle");
		emu_profile_argument_add_int(env->profile, "HANDLE", "hObject", object);
		emu_profile_function_returnvalue_int_set(env->profile, "BOOL", returnvalue);
	}

	emu_cpu_reg32_set(c, eax, returnvalue);

	

	emu_cpu_eip_set(c, eip_save);
	return 0;
}


int32_t	env_w32_hook_CreateFileA(struct emu_env *env, struct emu_env_hook *hook)
{
	logDebug(env->emu, "Hook me Captain Cook!\n");
	logDebug(env->emu, "%s:%i %s\n",__FILE__,__LINE__,__FUNCTION__);

	struct emu_cpu *c = emu_cpu_get(env->emu);

	uint32_t eip_save;

	POP_DWORD(c, &eip_save);

/*
HANDLE CreateFile(
  LPCTSTR lpFileName,
  DWORD dwDesiredAccess,
  DWORD dwShareMode,
  LPSECURITY_ATTRIBUTES lpSecurityAttributes,
  DWORD dwCreationDisposition,
  DWORD dwFlagsAndAttributes,
  HANDLE hTemplateFile
);
*/

	uint32_t p_filename;
	POP_DWORD(c, &p_filename);
    struct emu_string *filename = emu_string_new();
	emu_memory_read_string(emu_memory_get(env->emu), p_filename, filename, 256);

	uint32_t desiredaccess;
	POP_DWORD(c, &desiredaccess);

	uint32_t sharemode;
	POP_DWORD(c, &sharemode);

	uint32_t securityattr;
	POP_DWORD(c, &securityattr);

    uint32_t createdisp;
	POP_DWORD(c, &createdisp);

	uint32_t flagsandattr;
	POP_DWORD(c, &flagsandattr);

	uint32_t templatefile;
	POP_DWORD(c, &templatefile);



	uint32_t returnvalue;
	if ( hook->hook.win->userhook != NULL )
	{
		returnvalue = hook->hook.win->userhook(env, hook, 
											   emu_string_char(filename),
											   desiredaccess,
											   sharemode,
											   securityattr,
											   createdisp,
											   flagsandattr,
											   templatefile);
	}else
	{
		returnvalue	= 0x8383838;
	}

	if (env->profile != NULL)
	{
		emu_profile_function_add(env->profile, "CreateFile");
		emu_profile_argument_add_ptr(env->profile, "LPCTSTR", "lpFileName", p_filename);
		emu_profile_argument_add_string(env->profile,"",  "",  emu_string_char(filename));
		emu_profile_argument_add_int(env->profile, "DWORD", "dwDesiredAccess", desiredaccess);
		emu_profile_argument_add_int(env->profile, "DWORD", "dwShareMode", sharemode);
		emu_profile_argument_add_ptr(env->profile, "LPSECURITY_ATTRIBUTES", "lpSecurityAttributes", securityattr);
		emu_profile_argument_add_none(env->profile);
		emu_profile_argument_add_int(env->profile, "DWORD", "dwCreationDisposition", createdisp);
		emu_profile_argument_add_int(env->profile, "DWORD", "dwFlagsAndAttributes", flagsandattr);
		emu_profile_argument_add_int(env->profile, "HANDLE", "hTemplateFile", templatefile);
		emu_profile_function_returnvalue_int_set(env->profile, "HANDLE", returnvalue);
	}
	emu_string_free(filename);

	emu_cpu_reg32_set(c, eax, returnvalue);
	emu_cpu_eip_set(c, eip_save);

	return 0;
}

int32_t	env_w32_hook_CreateFileMapping(struct emu_env *env, struct emu_env_hook *hook)
{
	logDebug(env->emu, "Hook me Captain Cook!\n");
	logDebug(env->emu, "%s:%i %s\n",__FILE__,__LINE__,__FUNCTION__);

	struct emu_cpu *c = emu_cpu_get(env->emu);

	uint32_t eip_save;

	POP_DWORD(c, &eip_save);
/*HANDLE WINAPI CreateFileMapping(
  __in      HANDLE hFile,
  __in_opt  LPSECURITY_ATTRIBUTES lpAttributes,
  __in      DWORD flProtect,
  __in      DWORD dwMaximumSizeHigh,
  __in      DWORD dwMaximumSizeLow,
  __in_opt  LPCTSTR lpName
);
*/

	uint32_t hFile;
	POP_DWORD(c, &hFile);

	uint32_t lpAttributes;
	POP_DWORD(c, &lpAttributes);

	uint32_t flProtect;
	POP_DWORD(c, &flProtect);

	uint32_t dwMaximumSizeHigh;
	POP_DWORD(c, &dwMaximumSizeHigh);

	uint32_t dwMaximumSizeLow;
	POP_DWORD(c, &dwMaximumSizeLow);

	uint32_t p_lpName;
	POP_DWORD(c, &p_lpName);
	struct emu_string *Name = emu_string_new();
	emu_memory_read_string(emu_memory_get(env->emu), p_lpName, Name, 256);


	if (env->profile != NULL)
	{
		emu_profile_function_add(env->profile, "CreateFileMapping");
		emu_profile_argument_add_int(env->profile, "HANDLE", "hFile", hFile);
		emu_profile_argument_add_ptr(env->profile, "LPSECURITY_ATTRIBUTES", "lpSecurityAttributes", lpAttributes);
		emu_profile_argument_add_none(env->profile);
		emu_profile_argument_add_int(env->profile, "DWORD", "flProtect", flProtect);
		emu_profile_argument_add_int(env->profile, "DWORD", "dwMaximumSizeHigh", dwMaximumSizeHigh);
		emu_profile_argument_add_int(env->profile, "DWORD", "dwMaximumSizeLow", dwMaximumSizeLow);
		emu_profile_argument_add_ptr(env->profile, "LPCTSTR", "lpName", p_lpName);
		emu_profile_argument_add_string(env->profile,"",  "",  emu_string_char(Name));

		emu_profile_function_returnvalue_int_set(env->profile, "HANDLE", 0x123456);
	}

	emu_string_free(Name);

	emu_cpu_reg32_set(c, eax, 0x123456);
	emu_cpu_eip_set(c, eip_save);
	return 0;
}

int32_t	env_w32_hook_CreateProcessA(struct emu_env *env, struct emu_env_hook *hook)
{
	logDebug(env->emu, "Hook me Captain Cook!\n");
	logDebug(env->emu, "%s:%i %s\n",__FILE__,__LINE__,__FUNCTION__);

	struct emu_cpu *c = emu_cpu_get(env->emu);
	struct emu_memory *m = emu_memory_get(env->emu);

	uint32_t eip_save;

	POP_DWORD(c, &eip_save);

/*BOOL CreateProcess( 
  LPCWSTR pszImageName, 
  LPCWSTR pszCmdLine, 
  LPSECURITY_ATTRIBUTES psaProcess, 
  LPSECURITY_ATTRIBUTES psaThread, 
  BOOL fInheritHandles, 
  DWORD fdwCreate, 
  LPVOID pvEnvironment, 
  LPWSTR pszCurDir, 
  LPSTARTUPINFOW psiStartInfo, 
  LPPROCESS_INFORMATION pProcInfo
);*/
	

	uint32_t p_imagename;
	POP_DWORD(c, &p_imagename);

	struct emu_string *imagename = emu_string_new();
	emu_memory_read_string(m, p_imagename, imagename, 1024);


	uint32_t p_cmdline;
	POP_DWORD(c, &p_cmdline);

	struct emu_string *command = emu_string_new();
	emu_memory_read_string(m, p_cmdline, command, 1024);
//	emu_string_free(command);


	uint32_t p_process;
	POP_DWORD(c, &p_process);


	uint32_t p_thread;
	POP_DWORD(c, &p_thread);


	uint32_t inherithandles;
	POP_DWORD(c, &inherithandles);


	uint32_t create;
	POP_DWORD(c, &create);

	uint32_t environment;
	POP_DWORD(c, &environment);


	uint32_t cwd;
	POP_DWORD(c, &cwd);


	uint32_t p_startinfo;
	POP_DWORD(c, &p_startinfo);


	STARTUPINFO *si = malloc(sizeof(STARTUPINFO));
	memset(si, 0, sizeof(STARTUPINFO));

	emu_memory_read_dword(m, p_startinfo + 14 * 4, (uint32_t *)&si->hStdInput);
	emu_memory_read_dword(m, p_startinfo + 15 * 4, (uint32_t *)&si->hStdOutput);
	emu_memory_read_dword(m, p_startinfo + 16 * 4, (uint32_t *)&si->hStdError);


	uint32_t p_procinfo;
	POP_DWORD(c, &p_procinfo);

	PROCESS_INFORMATION *pi = malloc(sizeof(PROCESS_INFORMATION));
	memset(pi, 0, sizeof(PROCESS_INFORMATION));

	pi->hProcess = 4713;
	pi->hThread = 4714;
	pi->dwProcessId = 4711;
	pi->dwThreadId = 4712;

//	emu_memory_write_block(m, p_procinfo, pi, sizeof(PROCESS_INFORMATION));
	emu_memory_write_dword(m, p_procinfo+0*4, pi->hProcess);
	emu_memory_write_dword(m, p_procinfo+1*4, pi->hThread);
	emu_memory_write_dword(m, p_procinfo+2*4, pi->dwProcessId);
	emu_memory_write_dword(m, p_procinfo+3*4, pi->dwThreadId);

	uint32_t returnvalue;
	if ( hook->hook.win->userhook != NULL )
	{
		returnvalue = hook->hook.win->userhook(env, hook, 
											   /* LPCWSTR pszImageName,             */ NULL,
											   /* LPCWSTR pszCmdLine,               */ emu_string_char(command),
											   /* LPSECURITY_ATTRIBUTES psaProcess, */ NULL,
											   /* LPSECURITY_ATTRIBUTES psaThread,  */ NULL,
											   /* BOOL fInheritHandles,             */ NULL,
											   /* DWORD fdwCreate,                  */ NULL,
											   /* LPVOID pvEnvironment,             */ NULL,
											   /* LPWSTR pszCurDir,                 */ NULL,
											   /* LPSTARTUPINFOW psiStartInfo,      */ si,
											   /* LPPROCESS_INFORMATION pProcInfo   */ pi);
	}else
	{
		pi->hProcess = 4712;
		returnvalue	= -1;
	}

//	emu_memory_write_block(m, p_procinfo, pi, sizeof(PROCESS_INFORMATION));
	emu_memory_write_dword(m, p_procinfo+0*4, pi->hProcess);
	emu_memory_write_dword(m, p_procinfo+1*4, pi->hThread);
	emu_memory_write_dword(m, p_procinfo+2*4, pi->dwProcessId);
	emu_memory_write_dword(m, p_procinfo+3*4, pi->dwThreadId);



	if ( env->profile != NULL )
	{
		emu_profile_function_add(env->profile, "CreateProcess");
		emu_profile_argument_add_ptr(env->profile, "LPCWSTR", "pszImageName", p_imagename);
		emu_profile_argument_add_string(env->profile, "","", emu_string_char(imagename));
		emu_profile_argument_add_ptr(env->profile, "LPCWSTR", "pszCmdLine", p_cmdline);
		emu_profile_argument_add_string(env->profile, "","", emu_string_char(command));
		emu_profile_argument_add_ptr(env->profile, "LPSECURITY_ATTRIBUTES", "psaProcess", p_process);
		emu_profile_argument_add_none(env->profile);
		emu_profile_argument_add_ptr(env->profile, "LPSECURITY_ATTRIBUTES", "psaThread", p_thread);
		emu_profile_argument_add_none(env->profile);
		emu_profile_argument_add_int(env->profile, "BOOL", "fInheritHandles", inherithandles);
		emu_profile_argument_add_int(env->profile, "DWORD", "fdwCreate", create);
		emu_profile_argument_add_ptr(env->profile, "LPVOID", "pvEnvironment", environment);
		emu_profile_argument_add_none(env->profile);
		emu_profile_argument_add_ptr(env->profile, "LPWSTR", "pszCurDir", cwd);
		emu_profile_argument_add_none(env->profile);

		emu_profile_argument_add_ptr(env->profile, "LPSTARTUPINFOW", "psiStartInfo", p_startinfo);
		emu_profile_argument_struct_start(env->profile, "", "");
		emu_profile_argument_add_int(env->profile, "DWORD",  "cb"               , si->cb);                                                   
		emu_profile_argument_add_int(env->profile, "LPTSTR", "lpReserved"       , (uint32_t)(uintptr_t)si->lpReserved);
		emu_profile_argument_add_int(env->profile, "LPTSTR", "lpDesktop"        , (uint32_t)(uintptr_t)si->lpDesktop);
		emu_profile_argument_add_int(env->profile, "LPTSTR", "lpTitle"          , (uint32_t)(uintptr_t)si->lpTitle);
		emu_profile_argument_add_int(env->profile, "DWORD",  "dwX"              , si->dwX);
		emu_profile_argument_add_int(env->profile, "DWORD",  "dwY"              , si->dwY);
		emu_profile_argument_add_int(env->profile, "DWORD",  "dwXSize"          , si->dwXSize);
		emu_profile_argument_add_int(env->profile, "DWORD",  "dwYSize"          , si->dwYSize);
		emu_profile_argument_add_int(env->profile, "DWORD",  "dwXCountChars"    , si->dwXCountChars);
		emu_profile_argument_add_int(env->profile, "DWORD",  "dwYCountChars"    , si->dwYCountChars);
		emu_profile_argument_add_int(env->profile, "DWORD",  "dwFillAttribute"  , si->dwFillAttribute);
		emu_profile_argument_add_int(env->profile, "DWORD",  "dwFlags"          , si->dwFlags);
		emu_profile_argument_add_int(env->profile, "WORD",   "wShowWindow"      , si->wShowWindow);
		emu_profile_argument_add_int(env->profile, "WORD",   "cbReserved2"      , si->cbReserved2);
		emu_profile_argument_add_int(env->profile, "LPBYTE", "lpReserved2"      , (uint32_t)(uintptr_t)si->lpReserved2);
		emu_profile_argument_add_int(env->profile, "HANDLE", "hStdInput"        , si->hStdInput);                                            
		emu_profile_argument_add_int(env->profile, "HANDLE", "hStdOutput"       , si->hStdOutput);                                           
		emu_profile_argument_add_int(env->profile, "HANDLE", "hStdError"        , si->hStdError);                                           
		emu_profile_argument_struct_end(env->profile);

		emu_profile_argument_add_ptr(env->profile, "PROCESS_INFORMATION", "pProcInfo",0x52f74c);
		emu_profile_argument_struct_start(env->profile, "", "");
		emu_profile_argument_add_int(env->profile, "HANDLE", "hProcess"     ,pi->dwProcessId);
		emu_profile_argument_add_int(env->profile, "HANDLE", "hThread"          ,pi->dwThreadId);
		emu_profile_argument_add_int(env->profile, "DWORD", "dwProcessId"       ,pi->hProcess);
		emu_profile_argument_add_int(env->profile, "DWORD", "dwThreadId"        ,pi->hThread);
		emu_profile_argument_struct_end(env->profile);

		emu_profile_function_returnvalue_int_set(env->profile, "BOOL", returnvalue);
	}


	emu_cpu_reg32_set(c, eax, returnvalue);


	emu_string_free(imagename);
	emu_string_free(command);
	free(pi);
	free(si);

	emu_cpu_eip_set(c, eip_save);
	return 0;
}

int32_t env_w32_hook_CreateProcessInternalA(struct emu_env *env, struct emu_env_hook *hook)
{
	struct emu_cpu *c = emu_cpu_get(env->emu);
	struct emu_memory *m = emu_memory_get(env->emu);

	uint32_t eip_save;
	POP_DWORD(c, &eip_save);

/*
 * DWORD WINAPI CreateProcessInternal(  
 * __in         DWORD unknown1,                              // always (?) NULL  
 * __in_opt     LPCTSTR lpApplicationName,  
 * __inout_opt  LPTSTR lpCommandLine,  
 * __in_opt     LPSECURITY_ATTRIBUTES lpProcessAttributes,  
 * __in_opt     LPSECURITY_ATTRIBUTES lpThreadAttributes,  
 * __in         BOOL bInheritHandles,  
 * __in         DWORD dwCreationFlags,  
 * __in_opt     LPVOID lpEnvironment,  
 * __in_opt     LPCTSTR lpCurrentDirectory,  
 * __in         LPSTARTUPINFO lpStartupInfo,  
 * __out        LPPROCESS_INFORMATION lpProcessInformation,  
 * __in         DWORD unknown2                               // always (?) NULL
 * ;
 */
	uint32_t unknown1;
	uint32_t lpApplicationName;
	uint32_t lpCommandLine;
	uint32_t lpProcessAttributes;
	uint32_t lpThreadAttributes;
	uint32_t bInheritHandles;
	uint32_t dwCreationFlags;
	uint32_t lpEnvironment;
	uint32_t lpCurrentDirectory;
	uint32_t lpStartupInfo;
	uint32_t lpProcessInformation;
	uint32_t unknown2;

	POP_DWORD(c, &unknown1);
	POP_DWORD(c, &lpApplicationName);
	POP_DWORD(c, &lpCommandLine);
	POP_DWORD(c, &lpProcessAttributes);
	POP_DWORD(c, &lpThreadAttributes);
	POP_DWORD(c, &bInheritHandles);
	POP_DWORD(c, &dwCreationFlags);
	POP_DWORD(c, &lpEnvironment);
	POP_DWORD(c, &lpCurrentDirectory);
	POP_DWORD(c, &lpStartupInfo);
	POP_DWORD(c, &lpProcessInformation);
	POP_DWORD(c, &unknown2);

	struct emu_string *CommandLine = emu_string_new();
	if( lpCommandLine !=0 )
		emu_memory_read_string(m, lpCommandLine, CommandLine, 255);

	uint32_t returnvalue = 0;
	if ( env->profile != NULL )
	{
		emu_profile_function_add(env->profile, "CreateProcessInternal");
		emu_profile_argument_add_int(env->profile, "DWORD", "unknown1", unknown1);

		emu_profile_argument_add_ptr(env->profile, "LPCTSTR", "lpApplicationName", lpApplicationName);
		emu_profile_argument_add_none(env->profile);
		
		emu_profile_argument_add_ptr(env->profile, "LPTSTR", "lpCommandLine", lpCommandLine);
		if( lpCommandLine != 0 )
			emu_profile_argument_add_string(env->profile, "","", emu_string_char(CommandLine));
		else
			emu_profile_argument_add_none(env->profile);
		
		emu_profile_argument_add_ptr(env->profile, "LPSECURITY_ATTRIBUTES", "lpProcessAttributes", lpProcessAttributes);
		emu_profile_argument_add_none(env->profile);

		emu_profile_argument_add_ptr(env->profile, "LPSECURITY_ATTRIBUTES", "lpThreadAttributes", lpThreadAttributes);
		emu_profile_argument_add_none(env->profile);

		emu_profile_argument_add_int(env->profile, "BOOL", "bInheritHandles", bInheritHandles);
		emu_profile_argument_add_int(env->profile, "DWORD", "dwCreationFlags", dwCreationFlags);
		emu_profile_argument_add_ptr(env->profile, "LPVOID", "lpEnvironment", lpEnvironment);
		emu_profile_argument_add_none(env->profile);
		emu_profile_argument_add_ptr(env->profile, "LPCTSTR", "lpCurrentDirectory", lpCurrentDirectory);
		emu_profile_argument_add_none(env->profile);

		emu_profile_argument_add_ptr(env->profile, "LPSTARTUPINFO", "lpStartupInfo", lpStartupInfo);
		emu_profile_argument_add_none(env->profile);

		emu_profile_argument_add_ptr(env->profile, "PROCESS_INFORMATION", "lpProcessInformation",0x52f74c);
		emu_profile_argument_add_none(env->profile);

		emu_profile_argument_add_int(env->profile, "DWORD", "unknown2", unknown2);
		emu_profile_function_returnvalue_int_set(env->profile, "DWORD WINAPI", returnvalue);
	}

	emu_string_free(CommandLine);
	emu_cpu_reg32_set(c, eax, returnvalue);
	emu_cpu_eip_set(c, eip_save);
	return 1;
}


int32_t	env_w32_hook_DeleteFileA(struct emu_env *env, struct emu_env_hook *hook)
{
	logDebug(env->emu, "Hook me Captain Cook!\n");
	logDebug(env->emu, "%s:%i %s\n",__FILE__,__LINE__,__FUNCTION__);

	struct emu_cpu *c = emu_cpu_get(env->emu);

	uint32_t eip_save;

	POP_DWORD(c, &eip_save);

/*
BOOL DeleteFile(
  LPCTSTR lpFileName
);

*/
	uint32_t filename;
	POP_DWORD(c, &filename);

	if (env->profile != NULL)
	{
		emu_profile_function_add(env->profile, "DeleteFile");
		emu_profile_argument_add_ptr(env->profile, "LPCTSTR", "lpFileName", filename);
		emu_profile_argument_add_none(env->profile);
	}

	emu_cpu_eip_set(c, eip_save);
	return 0;
}

int32_t	env_w32_hook_ExitProcess(struct emu_env *env, struct emu_env_hook *hook)
{
	logDebug(env->emu, "Hook me Captain Cook!\n");
	logDebug(env->emu, "%s:%i %s\n",__FILE__,__LINE__,__FUNCTION__);

	struct emu_cpu *c = emu_cpu_get(env->emu);

	uint32_t eip_save;

	POP_DWORD(c, &eip_save);


/*
VOID WINAPI ExitProcess(
  UINT uExitCode
);
*/

	uint32_t exitcode;
	POP_DWORD(c, &exitcode);


	uint32_t returnvalue;
	if ( hook->hook.win->userhook != NULL )
	{
		returnvalue = hook->hook.win->userhook(env, hook, 
											  exitcode);
	}else
	{
		returnvalue	= 0;
	}

	if ( env->profile != NULL )
	{
		emu_profile_function_add(env->profile, "ExitProcess");
		emu_profile_argument_add_int(env->profile, "UINT", "uExitCode", exitcode);
		emu_profile_function_returnvalue_int_set(env->profile, "void", returnvalue);
	}
	

	emu_cpu_eip_set(c, eip_save);
	return 0;
}

int32_t	env_w32_hook_ExitThread(struct emu_env *env, struct emu_env_hook *hook)
{
	logDebug(env->emu, "Hook me Captain Cook!\n");
	logDebug(env->emu, "%s:%i %s\n",__FILE__,__LINE__,__FUNCTION__);

	struct emu_cpu *c = emu_cpu_get(env->emu);

	uint32_t eip_save;

	POP_DWORD(c, &eip_save);


/*
VOID ExitThread(
  DWORD dwExitCode
);
*/

	uint32_t exitcode;
	POP_DWORD(c, &exitcode);



	uint32_t returnvalue;
	if ( hook->hook.win->userhook != NULL )
	{
		returnvalue = hook->hook.win->userhook(env, hook, 
											  exitcode);
	}else
	{
		returnvalue	= 0;
	}

	if ( env->profile != NULL )
	{
		emu_profile_function_add(env->profile, "ExitThread");
		emu_profile_argument_add_int(env->profile, "DWORD", "dwExitCode", exitcode);
		emu_profile_function_returnvalue_int_set(env->profile, "void", returnvalue);
	}

	
	emu_cpu_eip_set(c, eip_save);
	return 0;
}

int32_t env_w32_hook_GetFileSize(struct emu_env *env, struct emu_env_hook *hook)
{
	struct emu_cpu *c = emu_cpu_get(env->emu);
	struct emu_memory *mem = emu_memory_get(env->emu);
	uint32_t eip_save;
	uint32_t hFile;
	uint32_t lpFileSizeHigh;

/*
 * DWORD WINAPI GetFileSize(
 *   __in       HANDLE hFile,
 *   __out_opt  LPDWORD lpFileSizeHigh
 * );
 */
	POP_DWORD(c, &eip_save);
	POP_DWORD(c, &hFile);
	POP_DWORD(c, &lpFileSizeHigh);
	
	uint32_t filesize = 4711;
	if( lpFileSizeHigh != 0 )
		emu_memory_write_dword(mem, lpFileSizeHigh, 0);

	emu_cpu_reg32_set(c, eax, filesize);

	if ( env->profile != NULL )
	{
		emu_profile_function_add(env->profile, "GetFileSize");
		emu_profile_argument_add_int(env->profile, "HANDLE", "hFile", hFile);
		emu_profile_argument_add_ptr(env->profile, "LPDWORD", "lpFileSizeHigh", lpFileSizeHigh);
		if( lpFileSizeHigh != 0 )
			emu_profile_argument_add_int(env->profile, "", "", 0);
		else
			emu_profile_argument_add_none(env->profile);

		emu_profile_function_returnvalue_int_set(env->profile, "DWORD WINAPI", c->reg[eax]);
	}

	emu_cpu_eip_set(c, eip_save);
	return 0;
}

int32_t env_w32_hook_GetModuleHandleA(struct emu_env *env, struct emu_env_hook *hook)
{
	struct emu_cpu *c = emu_cpu_get(env->emu);
	uint32_t eip_save;
	uint32_t filename;

/*
 * HMODULE WINAPI GetModuleHandle( __in_opt  LPCTSTR lpModuleName);
 */

	POP_DWORD(c, &eip_save);
	POP_DWORD(c, &filename);

	struct emu_memory *mem = emu_memory_get(env->emu);
	struct emu_string *s_filename = emu_string_new();
	emu_memory_read_string(mem, filename, s_filename, 256);

	char *dllname = emu_string_char(s_filename);


	int i=0;
	int found_dll = 0;
	emu_cpu_reg32_set(c, eax, 0); //default = fail

	for( i=0; env->env.win->loaded_dlls[i] != NULL; i++ )
	{
		if( strncasecmp(env->env.win->loaded_dlls[i]->dllname, dllname, strlen(env->env.win->loaded_dlls[i]->dllname)) == 0 )
		{
			emu_cpu_reg32_set(c, eax, env->env.win->loaded_dlls[i]->baseaddr);
			found_dll = 1;
			break;
		}
	}

	if( found_dll == 0 )
	{
		if( emu_env_w32_load_dll(env->env.win, dllname) == 0 )
		{
			emu_cpu_reg32_set(c, eax, env->env.win->loaded_dlls[i]->baseaddr);
			found_dll = 1;
		}
	}

	if ( env->profile != NULL )
	{
		emu_profile_function_add(env->profile, "GetModuleHandleA");
		emu_profile_argument_add_ptr(env->profile, "LPCSTR", "lpModuleName", filename);
		emu_profile_argument_add_string(env->profile, "", "", emu_string_char(s_filename));

		emu_profile_function_returnvalue_ptr_set(env->profile, "HMODULE WINAPI", c->reg[eax]);
		emu_profile_argument_add_none(env->profile);
	}

	emu_string_free(s_filename);
	emu_cpu_eip_set(c, eip_save);
	return 0;
}

int32_t env_w32_hook_GetVersion(struct emu_env *env, struct emu_env_hook *hook)
{
	struct emu_cpu *c = emu_cpu_get(env->emu);

	uint32_t eip_save;
	POP_DWORD(c, &eip_save);
/*
DWORD WINAPI GetVersion(void);
*/
	
	uint32_t version = 0xa280105;
	emu_cpu_reg32_set(c, eax, version);

	if ( env->profile != NULL )
	{
		emu_profile_function_add(env->profile, "GetVersion");
		emu_profile_function_returnvalue_int_set(env->profile, "DWORD WINAPI", version);
	}

	emu_cpu_eip_set(c, eip_save);
	return 0;
}

int32_t env_w32_hook_GetProcAddress(struct emu_env *env, struct emu_env_hook *hook)
{
	struct emu_cpu *c = emu_cpu_get(env->emu);
	struct emu_memory *mem = emu_memory_get(env->emu);

	uint32_t eip_save;
	POP_DWORD(c, &eip_save);

/* 
FFARPROC WINAPI GetProcAddress(
  HMODULE hModule,
  LPCSTR lpProcName
);
*/

	

	uint32_t module;// = emu_cpu_reg32_get(c, esp);
	POP_DWORD(c, &module);
//	printf("module ptr is %08x\n", module);

	uint32_t p_procname;
	POP_DWORD(c, &p_procname);

	

	struct emu_string *procname = emu_string_new();
	if( p_procname >= 0x1000 )
		emu_memory_read_string(mem, p_procname, procname, 256);

	logDebug(env->emu, "procname name is '%s'\n", emu_string_char(procname));

	int i;
	for ( i=0; env->env.win->loaded_dlls[i] != NULL; i++ )
	{
		if( env->env.win->loaded_dlls[i]->baseaddr == module )
		{
			logDebug(env->emu, "dll is %s %08x %08x \n", 
				   env->env.win->loaded_dlls[i]->dllname, 
				   module, 
				   env->env.win->loaded_dlls[i]->baseaddr);
			struct emu_env_w32_dll *dll = env->env.win->loaded_dlls[i];
			if( emu_string_char(procname) != NULL && p_procname >= 0x1000 )
			{ /* 2nd argument is a string */
				struct emu_hashtable_item *ehi = emu_hashtable_search(dll->exports_by_fnname, (void *)emu_string_char(procname));
				if ( ehi != NULL )
				{
					struct emu_env_hook *hook = (struct emu_env_hook *)ehi->value;
					logDebug(env->emu, "found %s at addr %08x\n",emu_string_char(procname), dll->baseaddr + hook->hook.win->virtualaddr );
					emu_cpu_reg32_set(c, eax, dll->baseaddr + hook->hook.win->virtualaddr);
				}
			}else
			{ /* 2nd argument is ordinal */
				int j;
				for( j=0;dll->exportx[j].fnname != NULL; j++ )
				{
					struct emu_env_w32_dll_export *ex = &dll->exportx[j];
					if( ex->ordinal == p_procname )
					{
						struct emu_env_hook *hook = &dll->hooks[j];
//						emu_string_append_char(procname, ex->fnname);
						logDebug(env->emu, "found %s for ordinal %i at addr %08x\n",ex->fnname, p_procname, dll->baseaddr + hook->hook.win->virtualaddr );
						emu_cpu_reg32_set(c, eax, dll->baseaddr + hook->hook.win->virtualaddr);
					}
				}
			}
			break;
		}
	}

	if ( env->profile != NULL )
	{
		emu_profile_function_add(env->profile, "GetProcAddress");
		emu_profile_argument_add_ptr(env->profile, "HMODULE", "hModule", module);
		emu_profile_argument_add_none(env->profile);
		emu_profile_argument_add_ptr(env->profile, "LPCSTR", "lpProcName", p_procname);
		if( emu_string_char(procname) != NULL )
			emu_profile_argument_add_string(env->profile, "", "", emu_string_char(procname));
		else
			emu_profile_argument_add_none(env->profile);
		emu_profile_function_returnvalue_ptr_set(env->profile, "FARPROC WINAPI", c->reg[eax]);
		emu_profile_argument_add_none(env->profile);
	}

	emu_string_free(procname);


	emu_cpu_eip_set(c, eip_save);
	return 0;
}

int32_t env_w32_hook_GetSystemDirectoryA(struct emu_env *env, struct emu_env_hook *hook)
{
	logDebug(env->emu, "Hook me Captain Cook!\n");
	logDebug(env->emu, "%s:%i %s\n",__FILE__,__LINE__,__FUNCTION__);

	struct emu_cpu *c = emu_cpu_get(env->emu);

	uint32_t eip_save;

	POP_DWORD(c, &eip_save);

/*
UINT GetSystemDirectory(
  LPTSTR lpBuffer,
  UINT uSize
);
*/


	uint32_t p_buffer;
	POP_DWORD(c, &p_buffer);
	

	uint32_t size;
	POP_DWORD(c, &size);

	static char *sysdir = "c:\\WINDOWS\\system32";
	emu_memory_write_block(emu_memory_get(env->emu), p_buffer, sysdir, 20);
	emu_cpu_reg32_set(c, eax, 19);

	if ( env->profile != NULL )
	{
		emu_profile_function_add(env->profile, "GetSystemDirectory");
		emu_profile_argument_add_ptr(env->profile, "LPTSTR", "lpBuffer", p_buffer);
		emu_profile_argument_add_string(env->profile, "", "", sysdir);
		emu_profile_argument_add_int(env->profile, "UINT", "uSize", size);
		emu_profile_function_returnvalue_int_set(env->profile, "UINT", 19);
	}

	emu_cpu_eip_set(c, eip_save);
	return 0;
}

int32_t env_w32_hook_GetTempPathA(struct emu_env *env, struct emu_env_hook *hook)
{
	logDebug(env->emu, "Hook me Captain Cook!\n");
	logDebug(env->emu, "%s:%i %s\n",__FILE__,__LINE__,__FUNCTION__);

	struct emu_cpu *c = emu_cpu_get(env->emu);

	uint32_t eip_save;

	POP_DWORD(c, &eip_save);

/*
DWORD WINAPI GetTempPath(
  __in   DWORD nBufferLength,
  __out  LPTSTR lpBuffer
);
*/

	uint32_t bufferlength;
	POP_DWORD(c, &bufferlength);

	uint32_t p_buffer;
	POP_DWORD(c, &p_buffer);

	static char *path = "c:\\tmp\\";

	emu_memory_write_block(emu_memory_get(env->emu), p_buffer, path, 8);
	emu_cpu_reg32_set(c, eax, 7);

	if (env->profile != NULL)
	{
		emu_profile_function_add(env->profile, "GetTempPathA");
		emu_profile_argument_add_int(env->profile, "DWORD", "nBufferLength", bufferlength);
		emu_profile_argument_add_ptr(env->profile, "LPTSTR", "lpBuffer", p_buffer);
		emu_profile_argument_add_string(env->profile, "", "", path);
		emu_profile_function_returnvalue_int_set(env->profile, "DWORD", 7);
	}

	emu_cpu_eip_set(c, eip_save);
	return 0;
}

int32_t env_w32_hook_GetTickCount(struct emu_env *env, struct emu_env_hook *hook)
{
	logDebug(env->emu, "Hook me Captain Cook!\n");
	logDebug(env->emu, "%s:%i %s\n",__FILE__,__LINE__,__FUNCTION__);

	struct emu_cpu *c = emu_cpu_get(env->emu);

	uint32_t eip_save;

	POP_DWORD(c, &eip_save);

	uint32_t tickcount = rand();

	emu_cpu_reg32_set(c, eax, tickcount);


	if (env->profile != NULL)
	{
		emu_profile_function_add(env->profile, "GetTickCount");
		emu_profile_function_returnvalue_int_set(env->profile, "int", tickcount);
	}

	emu_cpu_eip_set(c, eip_save);
	return 0;
}

int32_t	env_w32_hook__hwrite(struct emu_env *env, struct emu_env_hook *hook)
{
	return env_w32_hook__lwrite(env, hook);
}

int32_t	env_w32_hook__lcreat(struct emu_env *env, struct emu_env_hook *hook)
{
	logDebug(env->emu, "Hook me Captain Cook!\n");
	logDebug(env->emu, "%s:%i %s\n",__FILE__,__LINE__,__FUNCTION__);

	struct emu_cpu *c = emu_cpu_get(env->emu);

	uint32_t eip_save;

	POP_DWORD(c, &eip_save);

/*
LONG _lcreat(
  LPCSTR lpszFileName,
  int fnAttribute
);
*/
	uint32_t p_filename;
	POP_DWORD(c, &p_filename);
	struct emu_string *filename = emu_string_new();
	emu_memory_read_string(emu_memory_get(env->emu), p_filename, filename, 256);

	uint32_t fnAttribute;
	POP_DWORD(c, &fnAttribute);

	uint32_t returnvalue;
	if ( hook->hook.win->userhook != NULL )
	{
		returnvalue = hook->hook.win->userhook(env, hook, 
											   emu_string_char(filename),
											   fnAttribute);
	}else
	{
		returnvalue	= 4711;
	}

	if (env->profile != NULL)
	{
		emu_profile_function_add(env->profile, "_lcreat");
		emu_profile_argument_add_ptr(env->profile, "LPCTSTR", "lpFileName", p_filename);
		emu_profile_argument_add_string(env->profile,"",  "",  emu_string_char(filename));
		emu_profile_argument_add_int(env->profile, "int", "fnAttribute", fnAttribute);
		emu_profile_function_returnvalue_int_set(env->profile, "LONG", returnvalue);
	}

	emu_string_free(filename);

	emu_cpu_reg32_set(c, eax, returnvalue);

	emu_cpu_eip_set(c, eip_save);
	return 0;
}

int32_t	env_w32_hook__lclose(struct emu_env *env, struct emu_env_hook *hook)
{
	logDebug(env->emu, "Hook me Captain Cook!\n");
	logDebug(env->emu, "%s:%i %s\n",__FILE__,__LINE__,__FUNCTION__);

	struct emu_cpu *c = emu_cpu_get(env->emu);

	uint32_t eip_save;

	POP_DWORD(c, &eip_save);

/*
HFILE _lclose(
    HFILE hFile	// handle to file to close
   ); 
*/
	uint32_t file;
	POP_DWORD(c, &file);

	uint32_t returnvalue;
	if ( hook->hook.win->userhook != NULL )
	{
		returnvalue = hook->hook.win->userhook(env, hook, 
											   file);
	}else
	{
		returnvalue	= 0;
	}

	if (env->profile != NULL)
	{
		emu_profile_function_add(env->profile, "_lclose");
		emu_profile_argument_add_int(env->profile, "HFile", "hFile", file);
		emu_profile_function_returnvalue_int_set(env->profile, "HFILE", returnvalue);
	}

	emu_cpu_reg32_set(c, eax, returnvalue);

	emu_cpu_eip_set(c, eip_save);
	return 0;
}

int32_t	env_w32_hook__lwrite(struct emu_env *env, struct emu_env_hook *hook)
{
	logDebug(env->emu, "Hook me Captain Cook!\n");
	logDebug(env->emu, "%s:%i %s\n",__FILE__,__LINE__,__FUNCTION__);

	struct emu_cpu *c = emu_cpu_get(env->emu);

	uint32_t eip_save;

	POP_DWORD(c, &eip_save);

/*
LONG _lwrite(
  HFile hFile,
  LPCSTR lpBuffer,
  UINT cbWrite
);
*/
	uint32_t file;
	POP_DWORD(c, &file);

	uint32_t p_buffer;
	POP_DWORD(c, &p_buffer);

	uint32_t size;
	POP_DWORD(c, &size);
	uint32_t returnvalue;
	unsigned char *buffer = NULL;

	if( size < 1*1024*1024 )
	{
		buffer = malloc(size);
		emu_memory_read_block(emu_memory_get(env->emu), p_buffer, buffer, size);
	
		if ( hook->hook.win->userhook != NULL )
		{
			returnvalue = hook->hook.win->userhook(env, hook, 
												   file,
												   buffer,
												   size);
		}else
		{
			returnvalue	= size;
		}
	}else
	{
		returnvalue = -1;
	}

	if (env->profile != NULL)
	{
		emu_profile_function_add(env->profile, "_lwrite");
		emu_profile_argument_add_int(env->profile, "HFile", "hFile", file);
		emu_profile_argument_add_ptr(env->profile, "LPCSTR", "lpBuffer", p_buffer);
		if( size < 1*1024*1024 )
			emu_profile_argument_add_bytea(env->profile, "", "", buffer, size);
		else
			emu_profile_argument_add_bytea(env->profile, "", "", buffer, 0);
		emu_profile_argument_add_int(env->profile, "UINT", "cbWrite", size);
		emu_profile_function_returnvalue_int_set(env->profile, "LONG", returnvalue);
	}

	emu_cpu_reg32_set(c, eax, returnvalue);

	if( buffer )
		free(buffer);

	emu_cpu_eip_set(c, eip_save);
	return 0;
}

int32_t	env_w32_hook_LoadLibrayA(struct emu_env *env, struct emu_env_hook *hook)
{



	struct emu_cpu *c = emu_cpu_get(env->emu);

	uint32_t eip_save;
	POP_DWORD(c, &eip_save);

/* HMODULE WINAPI LoadLibrary(LPCTSTR lpFileName); */


	uint32_t dllname_ptr;// = emu_cpu_reg32_get(c, esp);
    POP_DWORD(c, &dllname_ptr);
    	

    struct emu_string *dllstr = emu_string_new();
    struct emu_memory *mem = emu_memory_get(env->emu);
    emu_memory_read_string(mem, dllname_ptr, dllstr, 256);


	char *dllname = emu_string_char(dllstr);
	

	int i=0;
	int found_dll = 0;
	if ( dllname != NULL )
	{
		for (i=0; env->env.win->loaded_dlls[i] != NULL; i++)
		{
			if (strncasecmp(env->env.win->loaded_dlls[i]->dllname, dllname, strlen(env->env.win->loaded_dlls[i]->dllname)) == 0)
			{
				logDebug(env->emu, "found dll %s, baseaddr is %08x \n",env->env.win->loaded_dlls[i]->dllname,env->env.win->loaded_dlls[i]->baseaddr);
				emu_cpu_reg32_set(c, eax, env->env.win->loaded_dlls[i]->baseaddr);
				found_dll = 1;
				break;
			}
		}
		if (found_dll == 0)
		{
			if (emu_env_w32_load_dll(env->env.win, dllname) == 0)
			{
				emu_cpu_reg32_set(c, eax, env->env.win->loaded_dlls[i]->baseaddr);
				found_dll = 1;
			}
			else
			{
				logDebug(env->emu, "error could not find %s\n", dllname);
				emu_cpu_reg32_set(c, eax, 0x0);
			}
		}
	}

	if (env->profile != NULL)
	{
		emu_profile_function_add(env->profile, "LoadLibraryA");
		emu_profile_argument_add_ptr(env->profile, "LPCTSTR", "lpFileName", dllname_ptr);
		if ( dllname != NULL )
			emu_profile_argument_add_string(env->profile, "", "", dllname);
		else
			emu_profile_argument_add_none(env->profile);
		if (found_dll == 1)
		{
			emu_profile_function_returnvalue_ptr_set(env->profile, "HMODULE", c->reg[eax]);
			emu_profile_argument_add_none(env->profile);
		}else
		{
			emu_profile_function_returnvalue_ptr_set(env->profile, "HMODULE", 0x0);
			emu_profile_argument_add_none(env->profile);
		}
	}

	emu_string_free(dllstr);

	emu_cpu_eip_set(c, eip_save);
	return 0;
}

int32_t	env_w32_hook_malloc(struct emu_env *env, struct emu_env_hook *hook)
{
	logDebug(env->emu, "Hook me Captain Cook!\n");
	logDebug(env->emu, "%s:%i %s\n",__FILE__,__LINE__,__FUNCTION__);

	struct emu_cpu *c = emu_cpu_get(env->emu);

	uint32_t eip_save;

	POP_DWORD(c, &eip_save);

/*
void *malloc( 
   size_t size 
);
*/

	uint32_t size;
	POP_DWORD(c, &size);
	PUSH_DWORD(c, size);

	logDebug(env->emu, "malloc %i bytes\n", size);
	uint32_t addr = 0;

	if( size <= 1*1024*1024 )
	{
		if (emu_memory_alloc(c->mem, &addr, size) == -1)
			emu_cpu_reg32_set(c, eax, 0);
		else
			emu_cpu_reg32_set(c, eax, addr);
	}

	
	if (env->profile != NULL)
	{
		emu_profile_function_add(env->profile, "malloc");
		emu_profile_argument_add_int(env->profile, "size_t", "size", size);
		emu_profile_function_returnvalue_int_set(env->profile, "void *", addr);
	}
    emu_cpu_eip_set(c, eip_save);
	return 0;
}

int32_t	env_w32_hook_memset(struct emu_env *env, struct emu_env_hook *hook)
{
	logDebug(env->emu, "Hook me Captain Cook!\n");
	logDebug(env->emu, "%s:%i %s\n",__FILE__,__LINE__,__FUNCTION__);

	struct emu_cpu *c = emu_cpu_get(env->emu);

	uint32_t eip_save;

	POP_DWORD(c, &eip_save);

/*
void *memset(
   void* dest, 
   int c, 
   size_t count 
);
*/
	uint32_t dest;
	POP_DWORD(c, &dest);

	uint32_t writeme;
	POP_DWORD(c, &writeme);

	uint32_t size;
	POP_DWORD(c, &size);

	PUSH_DWORD(c, size);
	PUSH_DWORD(c, writeme);
	PUSH_DWORD(c, dest);


	logDebug(env->emu, "memset(0x%08x, 0x%08x, %i)\n", dest, writeme, size);
	
	emu_cpu_reg32_set(c, eax, dest);

    emu_cpu_eip_set(c, eip_save);
	return 0;
}

int32_t env_w32_hook_MapViewOfFile(struct emu_env *env, struct emu_env_hook *hook)
{

	struct emu_cpu *c = emu_cpu_get(env->emu);

	uint32_t eip_save;
	POP_DWORD(c, &eip_save);

/*LPVOID WINAPI MapViewOfFile(
  __in  HANDLE hFileMappingObject,
  __in  DWORD dwDesiredAccess,
  __in  DWORD dwFileOffsetHigh,
  __in  DWORD dwFileOffsetLow,
  __in  SIZE_T dwNumberOfBytesToMap
);*/


	uint32_t hFileMappingObject;
	POP_DWORD(c, &hFileMappingObject);
	uint32_t dwDesiredAccess;
	POP_DWORD(c, &dwDesiredAccess);
	uint32_t dwFileOffsetHigh;
	POP_DWORD(c, &dwFileOffsetHigh);
	uint32_t dwFileOffsetLow;
	POP_DWORD(c, &dwFileOffsetLow);
	uint32_t dwNumberOfBytesToMap;
	POP_DWORD(c, &dwNumberOfBytesToMap);

	uint32_t addr;
	emu_memory_alloc(c->mem, &addr, 4711);

	if (env->profile != NULL)
	{
		emu_profile_function_add(env->profile, "MapViewOfFile");
		emu_profile_argument_add_int(env->profile, "HANDLE", "hFileMappingObject  ",hFileMappingObject);  
		emu_profile_argument_add_int(env->profile, "DWORD", "dwDesiredAccess     ",dwDesiredAccess);
		emu_profile_argument_add_int(env->profile, "DWORD", "dwFileOffsetHigh    ",dwFileOffsetHigh);
		emu_profile_argument_add_int(env->profile, "DWORD", "dwFileOffsetLow     ",dwFileOffsetLow);
		emu_profile_argument_add_int(env->profile, "SIZE_T", "dwNumberOfBytesToMap",dwNumberOfBytesToMap);

		emu_profile_function_returnvalue_int_set(env->profile, "LPVOID WINAPI", addr);
	}

	emu_cpu_reg32_set(c, eax, addr);
	emu_cpu_eip_set(c, eip_save);
	return 0;
}


int32_t env_w32_hook_Sleep(struct emu_env *env, struct emu_env_hook *hook)
{
	logDebug(env->emu, "Hook me Captain Cook!\n");
	logDebug(env->emu, "%s:%i %s\n",__FILE__,__LINE__,__FUNCTION__);

	struct emu_cpu *c = emu_cpu_get(env->emu);

	uint32_t eip_save;

	POP_DWORD(c, &eip_save);

/*VOID WINAPI Sleep(
  __in  DWORD dwMilliseconds
);
*/

	uint32_t dwMilliseconds;
	POP_DWORD(c, &dwMilliseconds);

	emu_cpu_reg32_set(c, eax, 0);

	if (env->profile != NULL)
	{
		emu_profile_function_add(env->profile, "Sleep");
		emu_profile_argument_add_int(env->profile, "DWORD", "dwMilliseconds", dwMilliseconds);
		emu_profile_function_returnvalue_int_set(env->profile, "void", 0);
	}

	emu_cpu_eip_set(c, eip_save);
	return 0;
}

int32_t	env_w32_hook_UnmapViewOfFile(struct emu_env *env, struct emu_env_hook *hook)
{
	struct emu_cpu *c = emu_cpu_get(env->emu);

	uint32_t eip_save;
	POP_DWORD(c, &eip_save);

/*
 * BOOL WINAPI UnmapViewOfFile(
 *   __in  LPCVOID lpBaseAddress
 * ); 
 */
	uint32_t lpBaseAddress;

	POP_DWORD(c, &lpBaseAddress);

	emu_cpu_reg32_set(c, eax, 1);
	if (env->profile != NULL)
	{
		emu_profile_function_add(env->profile, "UnmapViewOfFile");
		emu_profile_argument_add_ptr(env->profile, "LPCVOID", "lpBaseAddress", lpBaseAddress);
		emu_profile_argument_add_none(env->profile);
		emu_profile_function_returnvalue_int_set(env->profile, "BOOL WINAPI", 1);
	}
	emu_cpu_eip_set(c, eip_save);
	return 0;
}

int32_t	env_w32_hook_SetFilePointer(struct emu_env *env, struct emu_env_hook *hook)
{
	struct emu_cpu *c = emu_cpu_get(env->emu);

	uint32_t eip_save;
	POP_DWORD(c, &eip_save);

/*
 * DWORD WINAPI SetFilePointer(
 * __in         HANDLE hFile,
 * __in         LONG lDistanceToMove,
 * __inout_opt  PLONG lpDistanceToMoveHigh,
 * __in         DWORD dwMoveMethod
 *);
 */
	uint32_t hFile;
	uint32_t lDistanceToMove;
	uint32_t lpDistanceToMoveHigh;
	uint32_t dwMoveMethod;

	POP_DWORD(c, &hFile);
	POP_DWORD(c, &lDistanceToMove);
	POP_DWORD(c, &lpDistanceToMoveHigh);
	POP_DWORD(c, &dwMoveMethod);

	emu_cpu_reg32_set(c, eax, lDistanceToMove);
	if (env->profile != NULL)
	{
		emu_profile_function_add(env->profile, "SetFilePointer");
		emu_profile_argument_add_int(env->profile, "HANDLE", "hFile", hFile);
		emu_profile_argument_add_int(env->profile, "LONG", "lDistanceToMove", lDistanceToMove);
		emu_profile_argument_add_ptr(env->profile, "PLONG", "lpDistanceToMoveHigh", lpDistanceToMoveHigh);
		emu_profile_argument_add_none(env->profile);
		emu_profile_function_returnvalue_int_set(env->profile, "DWORD WINAPI", lDistanceToMove);
	}
	emu_cpu_eip_set(c, eip_save);
	return 0;
}

int32_t env_w32_hook_SetUnhandledExceptionFilter(struct emu_env *env, struct emu_env_hook *hook)
{
	logDebug(env->emu, "Hook me Captain Cook!\n");
	logDebug(env->emu, "%s:%i %s\n",__FILE__,__LINE__,__FUNCTION__);

	struct emu_cpu *c = emu_cpu_get(env->emu);

	uint32_t eip_save;

	POP_DWORD(c, &eip_save);

/*LPTOP_LEVEL_EXCEPTION_FILTER WINAPI SetUnhandledExceptionFilter(
  LPTOP_LEVEL_EXCEPTION_FILTER lpTopLevelExceptionFilter
);*/

	uint32_t lpfilter;
	POP_DWORD(c, &lpfilter);


	logDebug(env->emu, "Exception filter %08x\n", lpfilter);

	emu_cpu_reg32_set(c, eax, 0x7C81CDDA);

	if (env->profile != NULL)
	{
		emu_profile_function_add(env->profile, "SetUnhandledExceptionFilter");
		emu_profile_argument_add_ptr(env->profile, "LPTOP_LEVEL_EXCEPTION_FILTER", "lpTopLevelExceptionFilter", lpfilter);
		emu_profile_argument_add_none(env->profile);
		emu_profile_function_returnvalue_ptr_set(env->profile, "LPTOP_LEVEL_EXCEPTION_FILTER", 0x7C81CDDA);
		emu_profile_argument_add_none(env->profile);
	}

	emu_cpu_eip_set(c, eip_save);
	return 0;
}

int32_t	env_w32_hook_WaitForSingleObject(struct emu_env *env, struct emu_env_hook *hook)
{
	logDebug(env->emu, "Hook me Captain Cook!\n");
	logDebug(env->emu, "%s:%i %s\n",__FILE__,__LINE__,__FUNCTION__);

	struct emu_cpu *c = emu_cpu_get(env->emu);

	uint32_t eip_save;

	POP_DWORD(c, &eip_save);

/*
DWORD WINAPI WaitForSingleObject(
  HANDLE hHandle,
  DWORD dwMilliseconds
);
*/

	uint32_t handle;
	POP_DWORD(c, &handle);

	uint32_t msecs;
	POP_DWORD(c, &msecs);

	logDebug(env->emu, "WaitForSingleObject(hHandle=%i,  dwMilliseconds=%i)\n", handle, msecs);

	uint32_t returnvalue;
	if ( hook->hook.win->userhook != NULL )
	{
		returnvalue = hook->hook.win->userhook(env, hook, 
											   handle,
											   msecs);
	}else
	{
		returnvalue	= 0;
	}

	if (env->profile != NULL)
	{
		emu_profile_function_add(env->profile, "WaitForSingleObject");
		emu_profile_argument_add_int(env->profile, "HANDLE", "hHandle", handle);
		emu_profile_argument_add_int(env->profile, "DWORD", "dwMilliseconds", msecs);
		emu_profile_function_returnvalue_int_set(env->profile, "DWORD", returnvalue);
	}

	emu_cpu_reg32_set(c, eax, returnvalue);
	emu_cpu_eip_set(c, eip_save);
	return 0;
}

int32_t	env_w32_hook_WinExec(struct emu_env *env, struct emu_env_hook *hook)
{
	logDebug(env->emu, "Hook me Captain Cook!\n");
	logDebug(env->emu, "%s:%i %s\n",__FILE__,__LINE__,__FUNCTION__);

	struct emu_cpu *c = emu_cpu_get(env->emu);

	uint32_t eip_save;

	POP_DWORD(c, &eip_save);

/*
UINT WINAPI WinExec(
  LPCSTR lpCmdLine,
  UINT uCmdShow
);
*/

	uint32_t p_cmdline;
	POP_DWORD(c, &p_cmdline);

	struct emu_string *cmdstr = emu_string_new();
	emu_memory_read_string(emu_memory_get(env->emu), p_cmdline, cmdstr, 256);



	uint32_t show;
	POP_DWORD(c, &show);
	

	uint32_t returnvalue;
	if ( hook->hook.win->userhook != NULL )
	{
		returnvalue = hook->hook.win->userhook(env, hook, 
											   emu_string_char(cmdstr),
											   show);
	}else
	{
		returnvalue	= 32;
	}


	if (env->profile != NULL)
	{
		emu_profile_function_add(env->profile, "WinExec");
		emu_profile_argument_add_ptr(env->profile, "LPCSTR", "lpCmdLine", p_cmdline);
		emu_profile_argument_add_string(env->profile, "", "", emu_string_char(cmdstr));
		emu_profile_argument_add_int(env->profile, "UINT", "uCmdShow", show);
		emu_profile_function_returnvalue_int_set(env->profile, "UINT WINAPI", returnvalue);
	}

	emu_string_free(cmdstr);

	emu_cpu_reg32_set(c, eax, returnvalue);

	emu_cpu_eip_set(c, eip_save);
	return 0;
}

int32_t	env_w32_hook_WriteFile(struct emu_env *env, struct emu_env_hook *hook)
{
	logDebug(env->emu, "Hook me Captain Cook!\n");
	logDebug(env->emu, "%s:%i %s\n",__FILE__,__LINE__,__FUNCTION__);

	struct emu_cpu *c = emu_cpu_get(env->emu);

	uint32_t eip_save;

	POP_DWORD(c, &eip_save);

/*
BOOL WriteFile(
  HANDLE hFile,
  LPCVOID lpBuffer,
  DWORD nNumberOfBytesToWrite,
  LPDWORD lpNumberOfBytesWritten,
  LPOVERLAPPED lpOverlapped
);
*/
	uint32_t file;
	POP_DWORD(c, &file);

	uint32_t p_buffer;
	POP_DWORD(c,  &p_buffer);

	uint32_t bytestowrite;
	POP_DWORD(c,  &bytestowrite);

	unsigned char *buffer = malloc(bytestowrite);
	emu_memory_read_block(emu_memory_get(env->emu), p_buffer, buffer, bytestowrite);


	uint32_t p_byteswritten;
	POP_DWORD(c,  &p_byteswritten);
	
	uint32_t p_overlapped;
	POP_DWORD(c,  &p_overlapped);


	emu_memory_write_dword(emu_memory_get(env->emu), p_byteswritten, bytestowrite);


	uint32_t returnvalue;
	if ( hook->hook.win->userhook != NULL )
	{
		returnvalue = hook->hook.win->userhook(env, hook, 
											   file,
											   buffer,
											   bytestowrite,
											   p_byteswritten,
											   p_overlapped);
	}else
	{
		returnvalue	= 1;
	}

	if (env->profile != NULL)
	{
		emu_profile_function_add(env->profile, "WriteFile");
		emu_profile_argument_add_int(env->profile, "HANDLE", "hFile", file);
		emu_profile_argument_add_ptr(env->profile, "LPCVOID", "lpBuffer", p_buffer);
		emu_profile_argument_add_bytea(env->profile, "", "", buffer, bytestowrite);
		emu_profile_argument_add_int(env->profile, "DWORD", "nNumberOfBytesToWrite", bytestowrite);
		emu_profile_argument_add_ptr(env->profile, "LPDWORD", "lpNumberOfBytesWritten", p_byteswritten);
		emu_profile_argument_add_none(env->profile);
		emu_profile_argument_add_ptr(env->profile, "LPOVERLAPPED", "lpOverlapped", p_overlapped);
		emu_profile_argument_add_none(env->profile);
		emu_profile_function_returnvalue_int_set(env->profile, "BOOL", returnvalue);
	}

	emu_cpu_reg32_set(c, eax, returnvalue);

	free(buffer);

	emu_cpu_eip_set(c, eip_save);
	return 0;
}


int32_t env_w32_hook_VirtualProtect(struct emu_env *env, struct emu_env_hook *hook)
{
	logDebug(env->emu, "Hook me Captain Cook!\n");
	logDebug(env->emu, "%s:%i %s\n",__FILE__,__LINE__,__FUNCTION__);

	struct emu_cpu *c = emu_cpu_get(env->emu);

	uint32_t eip_save;
	POP_DWORD(c, &eip_save);

/*
 * BOOL VirtualProtect( 
 *	LPVOID lpAddress, 
 *	DWORD  dwSize, 
 *      DWORD  flNewProtect, 
 *      PDWORD lpflOldProtect 
 *); 
 */
	uint32_t p_address;
	POP_DWORD(c, &p_address);

	uint32_t size;
	POP_DWORD(c, &size);

	uint32_t newprotect;
	POP_DWORD(c, &newprotect);

	uint32_t oldprotect;
	POP_DWORD(c, &oldprotect);

	uint32_t returnvalue;
	if( hook->hook.win->userhook != NULL )
	{
		returnvalue = hook->hook.win->userhook(env, hook, p_address, size, newprotect, oldprotect);
	} else
	{
		returnvalue = 1;
	}

	if( env->profile != NULL )
	{
		emu_profile_function_add(env->profile, "VirtualProtect");
		emu_profile_argument_add_ptr(env->profile, "LPCVOID", "lpAddress", p_address);
		emu_profile_argument_add_none(env->profile);
		emu_profile_argument_add_int(env->profile, "DWORD"  , "dwSize", size);
		emu_profile_argument_add_int(env->profile, "DWORD"  , "flNewProtect", newprotect);
		emu_profile_argument_add_int(env->profile, "PDWORD" , "lpflOldProtectt", newprotect);
		emu_profile_function_returnvalue_int_set(env->profile, "BOOL", returnvalue);
	}

	emu_cpu_reg32_set(c, eax, returnvalue);
	emu_cpu_eip_set(c, eip_save);
	return 0;
}



int32_t env_w32_hook_VirtualProtectEx(struct emu_env *env, struct emu_env_hook *hook)
{
	logDebug(env->emu, "Hook me Captain Cook!\n");
	logDebug(env->emu, "%s:%i %s\n",__FILE__,__LINE__,__FUNCTION__);

	struct emu_cpu *c = emu_cpu_get(env->emu);

	uint32_t eip_save;
	POP_DWORD(c, &eip_save);


/*
 * BOOL VirtualProtectEx(
 *	HANDLE hProcess,
 *      LPVOID lpAddress,
 *      SIZE_T dwSize,
 *      DWORD  flNewProtect,
 *      PDWORD lpflOldProtect
 *);
 */
	uint32_t h_process;
	POP_DWORD(c, &h_process);

	uint32_t p_address;
	POP_DWORD(c, &p_address);

	uint32_t size;
	POP_DWORD(c, &size);

	uint32_t newprotect;
	POP_DWORD(c, &newprotect);

	uint32_t oldprotect;
	POP_DWORD(c, &oldprotect);

	uint32_t returnvalue;
	if( hook->hook.win->userhook != NULL )
	{
		returnvalue = hook->hook.win->userhook(env, hook, p_address, size, newprotect, oldprotect);
	} else
	{
		returnvalue = 1;
	}

	if( env->profile != NULL )
	{
		emu_profile_function_add(env->profile, "VirtualProtectEx");
		emu_profile_argument_add_ptr(env->profile, "HANDLE" , "hProcess", h_process);
		emu_profile_argument_add_none(env->profile);
		emu_profile_argument_add_ptr(env->profile, "LPCVOID", "lpAddress", p_address);
		emu_profile_argument_add_none(env->profile);
		emu_profile_argument_add_int(env->profile, "DWORD"  , "dwSize", size);
		emu_profile_argument_add_int(env->profile, "DWORD"  , "flNewProtect", newprotect);
		emu_profile_argument_add_int(env->profile, "PDWORD" , "lpflOldProtectt", newprotect);
		emu_profile_function_returnvalue_int_set(env->profile, "BOOL", returnvalue);
	}

	emu_cpu_reg32_set(c, eax, returnvalue);
	emu_cpu_eip_set(c, eip_save);

	return 0;
}

int32_t env_w32_hook_TerminateThread(struct emu_env *env, struct emu_env_hook *hook)
{
        logDebug(env->emu, "Hook me Captain Cook!\n");
        logDebug(env->emu, "%s:%i %s\n",__FILE__,__LINE__,__FUNCTION__);

        struct emu_cpu *c = emu_cpu_get(env->emu);

        uint32_t eip_save;

        POP_DWORD(c, &eip_save);

/*
BOOL WINAPI TerminateThread(
  HANDLE hThread,
  DWORD dwExitCode
);
*/

        uint32_t handle;
        POP_DWORD(c, &handle);

        uint32_t exitcode;
        POP_DWORD(c, &exitcode);

        logDebug(env->emu, "TerminateThread(hThread=%i,  dwExitCode=%i)\n", handle, exitcode);

        uint32_t returnvalue;
        if ( hook->hook.win->userhook != NULL )
        {
                returnvalue = hook->hook.win->userhook(env, hook,
                                                                                           handle,
                                                                                           exitcode);
        }else
        {
                returnvalue     = 0;
        }

        if (env->profile != NULL)
        {
                emu_profile_function_add(env->profile, "TerminateThread");
                emu_profile_argument_add_int(env->profile, "HANDLE", "hThread", handle);
                emu_profile_argument_add_int(env->profile, "DWORD", "dwExitCode", exitcode);
                emu_profile_function_returnvalue_int_set(env->profile, "BOOL", returnvalue);
        }

        emu_cpu_reg32_set(c, eax, returnvalue);
        emu_cpu_eip_set(c, eip_save);
        return 0;
}

