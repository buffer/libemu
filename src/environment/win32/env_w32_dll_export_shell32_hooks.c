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

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

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
#include "emu/environment/win32/env_w32_dll_export_urlmon_hooks.h"

void GetSHFolderName(int clsid, char* buf255)
{
	const char *FolderNames[0xff] =
	{
		[0x00] = "./DESKTOP",
		[0x01] = "./INTERNET",
		[0x02] = "./PROGRAMS",
		[0x03] = "./CONTROLS",
		[0x04] = "./PRINTERS",
		[0x05] = "./PERSONAL",
		[0x06] = "./FAVORITES",
		[0x07] = "./STARTUP",
		[0x08] = "./RECENT",
		[0x09] = "./SENDTO",
		[0x0A] = "./BITBUCKET",
		[0x0B] = "./STARTMENU",
		[0x10] = "./DESKTOPDIRECTORY",
		[0x11] = "./DRIVES",
		[0x12] = "./NETWORK",
		[0x13] = "./NETHOOD",
		[0x14] = "./FONTS",
		[0x15] = "./TEMPLATES",
		[0x16] = "./COMMON_STARTMENU",
		[0x17] = "./COMMON_PROGRAMS",
		[0x18] = "./COMMON_STARTUP",
		[0x19] = "./COMMON_DESKTOPDIRECTORY",
		[0x1a] = "./APPDATA",
		[0x1b] = "./PRINTHOOD",
		[0x1d] = "./ALTSTARTUP",
		[0x1e] = "./COMMON_ALTSTARTUP",
		[0x1f] = "./COMMON_FAVORITES",
		[0x20] = "./INTERNET_CACHE",
		[0x21] = "./COOKIES",
		[0x22] = "./HISTORY",
	};

	if( clsid < 0x22 && FolderNames[clsid] != 0 )
		strncpy(buf255, FolderNames[clsid], 254);
	else
		strncpy(buf255, "invalid id", 254);
}


int32_t env_hook_SHGetSpecialFolderPathA(struct emu_env *env, struct emu_env_hook *hook)
{

	struct emu_cpu *c = emu_cpu_get(env->emu);
	struct emu_memory *mem = emu_memory_get(env->emu);

	uint32_t eip_save;

	POP_DWORD(c, &eip_save);

/*
   CopyBOOL SHGetSpecialFolderPath(
                 HWND hwndOwner,
   __out  LPTSTR lpszPath,
   __in   int csidl,
   __in   BOOL fCreate
   );

 */
	uint32_t hwnd;
	POP_DWORD(c, &hwnd);

	uint32_t buf;
	POP_DWORD(c, &buf);

	uint32_t csidl;
	POP_DWORD(c, &csidl);

	uint32_t fCreate;
	POP_DWORD(c, &fCreate);

	char buf255[255];
	memset(buf255,0,254);
	GetSHFolderName(csidl, (char*)&buf255);

	emu_memory_write_block(mem,buf,buf255,strlen(buf255));

	emu_cpu_reg32_set(c, eax, 0);

	if ( env->profile != NULL )
	{
		emu_profile_function_add(env->profile, "SHGetSpecialFolderPath");
		emu_profile_argument_add_int(env->profile, "HWND", "hwndOwner", hwnd);
		emu_profile_argument_add_ptr(env->profile, "LPCSTR", "lpszPath", buf);
		emu_profile_argument_add_string(env->profile, "", "", buf255);
		emu_profile_argument_add_int(env->profile, "int", "csidl", csidl);
		emu_profile_argument_add_int(env->profile, "BOOL", "fCreate", fCreate);
		emu_profile_function_returnvalue_int_set(env->profile, "BOOL", c->reg[eax]);
	}

	emu_cpu_eip_set(c, eip_save);
	return 0;
}

