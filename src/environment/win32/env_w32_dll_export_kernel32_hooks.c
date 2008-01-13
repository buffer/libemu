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
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>

#include "../../../config.h"
#include "emu/emu.h"
#include "emu/emu_memory.h"
#include "emu/emu_cpu.h"
#include "emu/emu_cpu_data.h"
#include "emu/emu_cpu_stack.h"
#include "emu/emu_hashtable.h"
#include "emu/emu_string.h"
#include "emu/environment/emu_profile.h"
#include "emu/environment/win32/emu_env_w32.h"
#include "emu/environment/win32/emu_env_w32_dll.h"
#include "emu/environment/win32/emu_env_w32_dll_export.h"
#include "emu/environment/win32/env_w32_dll_export_kernel32_hooks.h"


int32_t	env_w32_hook_CloseHandle(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex)
{
	printf("Hook me Captain Cook!\n");
	printf("%s:%i %s\n",__FILE__,__LINE__,__FUNCTION__);

	struct emu_cpu *c = emu_cpu_get(env->emu);

	uint32_t eip_save;

	POP_DWORD(c, &eip_save);

	
	
/*
BOOL CloseHandle(
  HANDLE hObject
);
*/
	emu_profile_function_add(env->profile, "CloseHandle");

	uint32_t object;
	POP_DWORD(c, &object);
	emu_profile_argument_add_int(env->profile, "Handle", "hObject", object);

	emu_cpu_eip_set(c, eip_save);
	return 0;
}


int32_t	env_w32_hook_CreateFileA(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex)
{
	printf("Hook me Captain Cook!\n");
	printf("%s:%i %s\n",__FILE__,__LINE__,__FUNCTION__);

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
	emu_profile_function_add(env->profile, "CreateFile");

	uint32_t filename;
	POP_DWORD(c, &filename);
	emu_profile_argument_add_ref(env->profile, "LPCTSTR", "lpFileName", filename);
	emu_profile_argument_add_string(env->profile, "", "", "");


	uint32_t desiredaccess;
	POP_DWORD(c, &desiredaccess);
	emu_profile_argument_add_int(env->profile, "DWORD", "dwDesiredAccess", desiredaccess);

	uint32_t sharemode;
	POP_DWORD(c, &sharemode);
	emu_profile_argument_add_int(env->profile, "DWORD", "dwShareMode", sharemode);

	uint32_t securityattr;
	POP_DWORD(c, &securityattr);
	emu_profile_argument_add_ref(env->profile, "LPSECURITY_ATTRIBUTES", "lpSecurityAttributes", securityattr);
	emu_profile_argument_add_int(env->profile, "", "", 0);

    uint32_t createdisp;
	POP_DWORD(c, &createdisp);
	emu_profile_argument_add_int(env->profile, "DWORD", "dwCreationDisposition", createdisp);

	uint32_t flagsandattr;
	POP_DWORD(c, &flagsandattr);
	emu_profile_argument_add_int(env->profile, "DWORD", "dwFlagsAndAttributes", flagsandattr);

	uint32_t templatefile;
	POP_DWORD(c, &templatefile);
	emu_profile_argument_add_int(env->profile, "Handle", "hTemplateFile", templatefile);

	emu_cpu_eip_set(c, eip_save);
	return 0;
}


int32_t	env_w32_hook_CreateProcessA(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex)
{
	printf("Hook me Captain Cook!\n");
	printf("%s:%i %s\n",__FILE__,__LINE__,__FUNCTION__);

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

	emu_profile_function_add(env->profile, "CreateProcess");

	uint32_t p_imagename;
	POP_DWORD(c, &p_imagename);
	emu_profile_argument_add_ref(env->profile, "LPCWSTR", "pszImageName", p_imagename);
	emu_profile_argument_add_string(env->profile, "",  "", "");

	uint32_t p_cmdline;
	POP_DWORD(c, &p_cmdline);
	emu_profile_argument_add_ref(env->profile, "LPCWSTR", "pszCmdLine", p_cmdline);

	struct emu_string *command = emu_string_new();
	emu_memory_read_string(m, p_cmdline, command, 1024);
	emu_profile_argument_add_string(env->profile, "","", emu_string_char(command));
//	emu_string_free(command);


	uint32_t p_process;
	POP_DWORD(c, &p_process);
	emu_profile_argument_add_ref(env->profile, "LPSECURITY_ATTRIBUTES", "psaProcess", p_process);
	emu_profile_argument_add_string(env->profile, "", "", "");


	uint32_t p_thread;
	POP_DWORD(c, &p_thread);
	emu_profile_argument_add_ref(env->profile, "LPSECURITY_ATTRIBUTES", "psaThread", p_thread);
	emu_profile_argument_add_string(env->profile, "", "", "");


	uint32_t inherithandles;
	POP_DWORD(c, &inherithandles);
	emu_profile_argument_add_int(env->profile, "BOOL", "fInheritHandles", inherithandles);


	uint32_t create;
	POP_DWORD(c, &create);
	emu_profile_argument_add_int(env->profile, "DWORD", "fdwCreate", create);

	uint32_t environment;
	POP_DWORD(c, &environment);
	emu_profile_argument_add_ref(env->profile, "LPVOID", "pvEnvironment", environment);
	emu_profile_argument_add_string(env->profile, "", "", "");


	uint32_t cwd;
	POP_DWORD(c, &cwd);
	emu_profile_argument_add_ref(env->profile, "LPWSTR", "pszCurDir", cwd);
	emu_profile_argument_add_string(env->profile, "", "", "");


	uint32_t p_startinfo;
	POP_DWORD(c, &p_startinfo);
	emu_profile_argument_add_ref(env->profile, "LPSTARTUPINFOW", "psiStartInfo", p_startinfo);

	STARTUPINFO *si = malloc(sizeof(STARTUPINFO));
	emu_memory_read_block(m, p_startinfo, si, sizeof(STARTUPINFO));

	emu_profile_argument_start(env->profile, "", "");
	emu_profile_argument_add_int(env->profile, "DWORD",  "cb"				, si->cb);                                                   
	emu_profile_argument_add_int(env->profile, "LPTSTR", "lpReserved"		, (unsigned int)si->lpReserved);
	emu_profile_argument_add_int(env->profile, "LPTSTR", "lpDesktop"		, (unsigned int)si->lpDesktop);
	emu_profile_argument_add_int(env->profile, "LPTSTR", "lpTitle"			, (unsigned int)si->lpTitle);
	emu_profile_argument_add_int(env->profile, "DWORD",  "dwX"				, si->dwX);
	emu_profile_argument_add_int(env->profile, "DWORD",  "dwY"				, si->dwY);
	emu_profile_argument_add_int(env->profile, "DWORD",  "dwXSize"			, si->dwXSize);
	emu_profile_argument_add_int(env->profile, "DWORD",  "dwYSize"			, si->dwYSize);
	emu_profile_argument_add_int(env->profile, "DWORD",  "dwXCountChars"	, si->dwXCountChars);
	emu_profile_argument_add_int(env->profile, "DWORD",  "dwYCountChars"	, si->dwYCountChars);
	emu_profile_argument_add_int(env->profile, "DWORD",  "dwFillAttribute"	, si->dwFillAttribute);
	emu_profile_argument_add_int(env->profile, "DWORD",  "dwFlags"			, si->dwFlags);
	emu_profile_argument_add_int(env->profile, "WORD",   "wShowWindow"		, si->wShowWindow);
	emu_profile_argument_add_int(env->profile, "WORD",   "cbReserved2"		, si->cbReserved2);
	emu_profile_argument_add_int(env->profile, "LPBYTE", "lpReserved2"		, (unsigned int)si->lpReserved2);
	emu_profile_argument_add_int(env->profile, "HANDLE", "hStdInput"		, si->hStdInput);                                            
	emu_profile_argument_add_int(env->profile, "HANDLE", "hStdOutput"		, si->hStdOutput);                                           
	emu_profile_argument_add_int(env->profile, "HANDLE", "hStdError"		, si->hStdError);                                           
	emu_profile_argument_end(env->profile);
	

	uint32_t p_procinfo;
	POP_DWORD(c, &p_procinfo);
	emu_profile_argument_add_ref(env->profile, "PROCESS_INFORMATION", "pProcInfo",0x52f74c);

	PROCESS_INFORMATION *pi = malloc(sizeof(PROCESS_INFORMATION));
	emu_memory_read_block(m, p_procinfo, pi, sizeof(PROCESS_INFORMATION));

	emu_profile_argument_start(env->profile, "", "");
	emu_profile_argument_add_int(env->profile, "DWORD", "hProcess"		,pi->dwProcessId);
	emu_profile_argument_add_int(env->profile, "DWORD", "hThread"			,pi->dwThreadId);
	emu_profile_argument_add_int(env->profile, "HANDLE", "dwProcessId"		,pi->hProcess);
	emu_profile_argument_add_int(env->profile, "HANDLE", "dwThreadId"		,pi->hThread);
	emu_profile_argument_end(env->profile);



	printf("CreateProcessA \n");	
	emu_cpu_reg32_set(c, eax, 0);

	

	pi->dwProcessId = 4711;
	pi->dwThreadId = 4712;
	pi->hProcess = 4713;
	pi->hThread = 4714;

	emu_memory_write_block(m, p_procinfo, pi, sizeof(PROCESS_INFORMATION));


	fflush(NULL);

#ifdef HAVE_INTERACTIVE_HOOKS
	if (emu_string_char(command) != NULL && strncasecmp(emu_string_char(command), "cmd", 3) == 0)
	{
	
	// the code is meant to be an example how one could do it
		pid_t pid;
		if ((pid = fork()) == 0)
		{ // child
	
			dup2(si->hStdInput,  fileno(stdin));
			dup2(si->hStdOutput, fileno(stdout));
			dup2(si->hStdError,  fileno(stderr));
	
			system("/opt/cmd/bin/cmdexe.pl -p winxp -l /opt/cmd/var/log/cmd/");
			exit(EXIT_SUCCESS);
		}else
		{ // parent 
			pi->hProcess = pid;
			emu_memory_write_block(m, p_procinfo, pi, sizeof(PROCESS_INFORMATION));
		}
	}
#endif
	emu_string_free(command);
	free(pi);
	free(si);
	emu_cpu_eip_set(c, eip_save);
	return 0;
}

int32_t	env_w32_hook_DeleteFileA(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex)
{
	printf("Hook me Captain Cook!\n");
	printf("%s:%i %s\n",__FILE__,__LINE__,__FUNCTION__);

	struct emu_cpu *c = emu_cpu_get(env->emu);

	uint32_t eip_save;

	POP_DWORD(c, &eip_save);

/*
BOOL DeleteFile(
  LPCTSTR lpFileName
);

*/
	emu_profile_function_add(env->profile, "DeleteFile");
	uint32_t filename;
	POP_DWORD(c, &filename);
	emu_profile_argument_add_ref(env->profile, "LPCTSTR", "lpFileName", filename);
	emu_profile_argument_add_string(env->profile, "", "", "");

	emu_cpu_eip_set(c, eip_save);
	return 0;
}

int32_t	env_w32_hook_ExitProcess(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex)
{
	printf("Hook me Captain Cook!\n");
	printf("%s:%i %s\n",__FILE__,__LINE__,__FUNCTION__);

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


	emu_cpu_eip_set(c, eip_save);
	return 0;
}

int32_t	env_w32_hook_ExitThread(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex)
{
	printf("Hook me Captain Cook!\n");
	printf("%s:%i %s\n",__FILE__,__LINE__,__FUNCTION__);

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


	emu_cpu_eip_set(c, eip_save);
	return 0;
}

int32_t	env_w32_hook_fclose(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex)
{
	printf("Hook me Captain Cook!\n");
	printf("%s:%i %s\n",__FILE__,__LINE__,__FUNCTION__);

	struct emu_cpu *c = emu_cpu_get(env->emu);

	uint32_t eip_save;

	POP_DWORD(c, &eip_save);

/*
int _fcloseall( void );
int fclose( FILE *stream );
*/
	uint32_t p_stream;
	MEM_DWORD_READ(c, c->reg[esp]-8, &p_stream);


	printf("fclose(0x%08x)\n", p_stream);
	
	emu_cpu_reg32_set(c, eax, 0);

    emu_cpu_eip_set(c, eip_save);
	return 0;
}


int32_t	env_w32_hook_fopen(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex)
{
	printf("Hook me Captain Cook!\n");
	printf("%s:%i %s\n",__FILE__,__LINE__,__FUNCTION__);

	struct emu_cpu *c = emu_cpu_get(env->emu);

	uint32_t eip_save;

	POP_DWORD(c, &eip_save);

/*
FILE *fopen( const char *filename, const char *mode );
FILE *_wfopen( const wchar_t *filename, const wchar_t *mode );
*/
	uint32_t p_filename;
	MEM_DWORD_READ(c, c->reg[esp], &p_filename);


	uint32_t p_mode;
	MEM_DWORD_READ(c, c->reg[esp]-4, &p_mode);

	struct emu_string *mode = emu_string_new();
	emu_memory_read_string(c->mem, p_mode, mode, 512);

	struct emu_string *filename = emu_string_new();
	emu_memory_read_string(c->mem, p_filename, filename, 512);

	printf("fopen(%s, %s)\n", emu_string_char(filename), (char *)mode->data);
	
	emu_cpu_reg32_set(c, eax, 0x8888);

    emu_cpu_eip_set(c, eip_save);
	return 0;
}

int32_t	env_w32_hook_fwrite(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex)
{
	printf("Hook me Captain Cook!\n");
	printf("%s:%i %s\n",__FILE__,__LINE__,__FUNCTION__);

	struct emu_cpu *c = emu_cpu_get(env->emu);

	uint32_t eip_save;

	POP_DWORD(c, &eip_save);

/*
size_t fwrite( const void *buffer, size_t size, size_t count, FILE *stream );
*/
	uint32_t p_buffer;
	MEM_DWORD_READ(c, c->reg[esp], &p_buffer);


	uint32_t size;
	MEM_DWORD_READ(c, (c->reg[esp]-4), &size);

	uint32_t count;
	MEM_DWORD_READ(c, (c->reg[esp]-8), &count);

	uint32_t p_stream;
	MEM_DWORD_READ(c, c->reg[esp]-12, &p_stream);


	printf("fwrite(0x%08x, %d, %d, 0x%08x)\n", p_buffer, size, count, p_stream);
	
	emu_cpu_reg32_set(c, eax, size*count);

    emu_cpu_eip_set(c, eip_save);
	return 0;
}

int32_t env_w32_hook_GetProcAddress(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex)
{
	struct emu_cpu *c = emu_cpu_get(env->emu);
	struct emu_memory *mem = emu_memory_get(env->emu);

	uint32_t eip_save;
	POP_DWORD(c, &eip_save);

/* 
FARPROC WINAPI GetProcAddress(
  HMODULE hModule,
  LPCSTR lpProcName
);
*/
	emu_profile_function_add(env->profile, "GetProcAddress");
	

	uint32_t module;// = emu_cpu_reg32_get(c, esp);
	POP_DWORD(c, &module);
	emu_profile_argument_add_int(env->profile, "HMODULE", "hModule", module);

//	printf("module ptr is %08x\n", module);

	uint32_t p_procname;
	POP_DWORD(c, &p_procname);
	emu_profile_argument_add_ref(env->profile, "LPCSTR", "lpProcName", p_procname);
	

	struct emu_string *procname = emu_string_new();
	emu_memory_read_string(mem, p_procname, procname, 256);
	emu_profile_argument_add_string(env->profile, "", "", emu_string_char(procname));

	printf("procname name is '%s'\n", emu_string_char(procname));

	int i;
	for ( i=0; env->loaded_dlls[i] != NULL; i++ )
	{
		if ( env->loaded_dlls[i]->baseaddr == module )
		{
			printf("dll is %s %08x %08x \n", env->loaded_dlls[i]->dllname, module, env->loaded_dlls[i]->baseaddr);

			struct emu_env_w32_dll *dll = env->loaded_dlls[i];
			struct emu_hashtable_item *ehi = emu_hashtable_search(dll->exports_by_fnname, (void *)emu_string_char(procname));

			struct emu_env_w32_dll_export *ex = (struct emu_env_w32_dll_export *)ehi->value;

			if ( ehi == NULL )
			{
				break;
			}
			else
			{
				printf("found %s at addr %08x\n",emu_string_char(procname), dll->baseaddr + ex->virtualaddr );
				emu_cpu_reg32_set(c, eax, dll->baseaddr + ex->virtualaddr);
				break;
			}
		}
	}

	emu_string_free(procname);


	emu_cpu_eip_set(c, eip_save);
	return 0;
}

int32_t env_w32_hook_GetSystemDirectoryA(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex)
{
	printf("Hook me Captain Cook!\n");
	printf("%s:%i %s\n",__FILE__,__LINE__,__FUNCTION__);

	struct emu_cpu *c = emu_cpu_get(env->emu);

	uint32_t eip_save;

	POP_DWORD(c, &eip_save);

/*
UINT GetSystemDirectory(
  LPTSTR lpBuffer,
  UINT uSize
);
*/
	emu_profile_function_add(env->profile, "GetSystemDirectory");

	uint32_t p_buffer;
	POP_DWORD(c, &p_buffer);
	emu_profile_argument_add_ref(env->profile, "LPTSTR", "lpBuffer", p_buffer);
	emu_profile_argument_add_string(env->profile, "", "", "");

	uint32_t size;
	POP_DWORD(c, &size);
	emu_profile_argument_add_int(env->profile, "UINT", "uSize", size);


	emu_memory_write_block(emu_memory_get(env->emu), p_buffer, "c:\\WINDOWS\\system32\x00", 20);
	emu_cpu_reg32_set(c, eax, 19);

	emu_cpu_eip_set(c, eip_save);
	return 0;
}


int32_t env_w32_hook_GetTickCount(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex)
{
	printf("Hook me Captain Cook!\n");
	printf("%s:%i %s\n",__FILE__,__LINE__,__FUNCTION__);

	struct emu_cpu *c = emu_cpu_get(env->emu);

	uint32_t eip_save;

	POP_DWORD(c, &eip_save);

	emu_cpu_reg32_set(c, eax, (uint32_t)rand());

	emu_cpu_eip_set(c, eip_save);
	return 0;
}

int32_t	env_w32_hook__hwrite(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex)
{
	return env_w32_hook__lwrite(env, ex);
}

int32_t	env_w32_hook__lcreat(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex)
{
	printf("Hook me Captain Cook!\n");
	printf("%s:%i %s\n",__FILE__,__LINE__,__FUNCTION__);

	struct emu_cpu *c = emu_cpu_get(env->emu);

	uint32_t eip_save;

	POP_DWORD(c, &eip_save);

/*
LONG _lcreat(
  LPCSTR lpszFileName,
  int fnAttribute
);
*/
	uint32_t filename;
	POP_DWORD(c, &filename);

	uint32_t attr;
	POP_DWORD(c, &attr);

	emu_cpu_reg32_set(c, eax, 4711); // filehandle

	emu_cpu_eip_set(c, eip_save);
	return 0;
}

int32_t	env_w32_hook__lclose(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex)
{
	printf("Hook me Captain Cook!\n");
	printf("%s:%i %s\n",__FILE__,__LINE__,__FUNCTION__);

	struct emu_cpu *c = emu_cpu_get(env->emu);

	uint32_t eip_save;

	POP_DWORD(c, &eip_save);

/*
FIXME
*/
	uint32_t file;
	POP_DWORD(c, &file);

	emu_cpu_eip_set(c, eip_save);
	return 0;
}

int32_t	env_w32_hook__lwrite(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex)
{
	printf("Hook me Captain Cook!\n");
	printf("%s:%i %s\n",__FILE__,__LINE__,__FUNCTION__);

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

	uint32_t buffer;
	POP_DWORD(c, &buffer);

	uint32_t size;
	POP_DWORD(c, &size);

	emu_cpu_reg32_set(c, eax, size);

	emu_cpu_eip_set(c, eip_save);
	return 0;
}

int32_t	env_w32_hook_LoadLibrayA(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex)
{



	struct emu_cpu *c = emu_cpu_get(env->emu);

	uint32_t eip_save;
	POP_DWORD(c, &eip_save);

/* HMODULE WINAPI LoadLibrary(LPCTSTR lpFileName); */

	emu_profile_function_add(env->profile, "LoadLibraryA");

	uint32_t dllname_ptr;// = emu_cpu_reg32_get(c, esp);
    POP_DWORD(c, &dllname_ptr);
	emu_profile_argument_add_ref(env->profile, "LPCTSTR", "lpFileName", dllname_ptr);
    	

    struct emu_string *dllstr = emu_string_new();
    struct emu_memory *mem = emu_memory_get(env->emu);
    emu_memory_read_string(mem, dllname_ptr, dllstr, 256);


	char *dllname = emu_string_char(dllstr);
	emu_profile_argument_add_string(env->profile, "", "", dllname);


	int i;
	int found_dll = 0;
	for (i=0; env->loaded_dlls[i] != NULL; i++)
	{
		if (strncasecmp(env->loaded_dlls[i]->dllname, dllname, strlen(env->loaded_dlls[i]->dllname)) == 0)
		{
			printf("found dll %s, baseaddr is %08x \n",env->loaded_dlls[i]->dllname,env->loaded_dlls[i]->baseaddr);
			emu_cpu_reg32_set(c, eax, env->loaded_dlls[i]->baseaddr);
			found_dll = 1;
		}
	}
	
	if (found_dll == 0)
	{
        if (emu_env_w32_load_dll(env, dllname) == 0)
        {
            emu_cpu_reg32_set(c, eax, env->loaded_dlls[i]->baseaddr);        
        }
        else
        {
            printf("error could not find %s\n", dllname);
            emu_cpu_reg32_set(c, eax, 0x4711);
        }
	}

	emu_string_free(dllstr);

	emu_cpu_eip_set(c, eip_save);
	return 0;
}

int32_t	env_w32_hook_malloc(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex)
{
	printf("Hook me Captain Cook!\n");
	printf("%s:%i %s\n",__FILE__,__LINE__,__FUNCTION__);

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

	printf("malloc %i bytes\n", size);

	uint32_t addr;
	if (emu_memory_alloc(c->mem, &addr, size) == -1)
		emu_cpu_reg32_set(c, eax, 0);
	else
		emu_cpu_reg32_set(c, eax, addr);

	
    emu_cpu_eip_set(c, eip_save);
	return 0;
}

int32_t	env_w32_hook_memset(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex)
{
	printf("Hook me Captain Cook!\n");
	printf("%s:%i %s\n",__FILE__,__LINE__,__FUNCTION__);

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


	printf("memset(0x%08x, 0x%08x, %i)\n", dest, writeme, size);
	
	emu_cpu_reg32_set(c, eax, dest);

    emu_cpu_eip_set(c, eip_save);
	return 0;

}

int32_t env_w32_hook_SetUnhandledExceptionFilter(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex)
{
	printf("Hook me Captain Cook!\n");
	printf("%s:%i %s\n",__FILE__,__LINE__,__FUNCTION__);

	struct emu_cpu *c = emu_cpu_get(env->emu);

	uint32_t eip_save;

	POP_DWORD(c, &eip_save);

/*LPTOP_LEVEL_EXCEPTION_FILTER WINAPI SetUnhandledExceptionFilter(
  LPTOP_LEVEL_EXCEPTION_FILTER lpTopLevelExceptionFilter
);*/

	uint32_t lpfilter;
	POP_DWORD(c, &lpfilter);

	printf("Exception filter %08x\n", lpfilter);

	emu_cpu_reg32_set(c, eax, 0x7C81CDDA);

	emu_cpu_eip_set(c, eip_save);
	return 0;
}

int32_t	env_w32_hook_WaitForSingleObject(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex)
{
	printf("Hook me Captain Cook!\n");
	printf("%s:%i %s\n",__FILE__,__LINE__,__FUNCTION__);

	struct emu_cpu *c = emu_cpu_get(env->emu);

	uint32_t eip_save;

	POP_DWORD(c, &eip_save);

/*
DWORD WINAPI WaitForSingleObject(
  HANDLE hHandle,
  DWORD dwMilliseconds
);
*/
	emu_profile_function_add(env->profile, "WaitForSingleObject");

	uint32_t handle;
	POP_DWORD(c, &handle);
	emu_profile_argument_add_int(env->profile, "HANDLE", "hHandle", handle);

	uint32_t msecs;
	POP_DWORD(c, &msecs);
	emu_profile_argument_add_int(env->profile, "DWORD", "dwMilliseconds", msecs);

	printf("WaitForSingleObject(hHandle=%i,  dwMilliseconds=%i)\n", handle, msecs);

// the code is meant to be an example how one could do it
#ifdef HAVE_INTERACTIVE_HOOKS
	int status;
	while(1)
	{
		if (waitpid(handle, &status, WNOHANG) != 0)
			break;
		sleep(1);
	}
#endif

	emu_cpu_reg32_set(c, eax, 32);

	emu_cpu_eip_set(c, eip_save);
	return 0;
}

int32_t	env_w32_hook_WinExec(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex)
{
	printf("Hook me Captain Cook!\n");
	printf("%s:%i %s\n",__FILE__,__LINE__,__FUNCTION__);

	struct emu_cpu *c = emu_cpu_get(env->emu);

	uint32_t eip_save;

	POP_DWORD(c, &eip_save);

/*
UINT WINAPI WinExec(
  LPCSTR lpCmdLine,
  UINT uCmdShow
);
*/
	emu_profile_function_add(env->profile, "WinExec");

	uint32_t p_cmdline;
	POP_DWORD(c, &p_cmdline);
	emu_profile_argument_add_ref(env->profile, "LPCSTR", "lpCmdLine", p_cmdline);

	struct emu_string *cmdstr = emu_string_new();
	emu_memory_read_string(emu_memory_get(env->emu), p_cmdline, cmdstr, 256);
	emu_profile_argument_add_string(env->profile, "", "", emu_string_char(cmdstr));
	emu_string_free(cmdstr);


	uint32_t show;
	POP_DWORD(c, &show);
	emu_profile_argument_add_int(env->profile, "UINT", "uCmdShow", show);
	



	emu_cpu_reg32_set(c, eax, 32);

	emu_cpu_eip_set(c, eip_save);
	return 0;
}

int32_t	env_w32_hook_WriteFile(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex)
{
	printf("Hook me Captain Cook!\n");
	printf("%s:%i %s\n",__FILE__,__LINE__,__FUNCTION__);

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

	uint32_t buffer;
	POP_DWORD(c,  &buffer);

	uint32_t bytestowrite;
	POP_DWORD(c,  &bytestowrite);

	uint32_t byteswritten;
	POP_DWORD(c,  &byteswritten);

	uint32_t overlapped;
	POP_DWORD(c,  &overlapped);

	emu_memory_write_dword(emu_memory_get(env->emu), byteswritten, bytestowrite);

	emu_cpu_reg32_set(c, eax, 32);

	emu_cpu_eip_set(c, eip_save);
	return 0;
}


