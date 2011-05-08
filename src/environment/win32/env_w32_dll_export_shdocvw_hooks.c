/********************************************************************************
 *                               libemu
 *
 *                    - x86 shellcode emulation -
 *
 *
 * Copyright (C) 2007-2011  Paul Baecher & Markus Koetter
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


int32_t	env_w32_hook_IEWinMain(struct emu_env *env, struct emu_env_hook *hook)
{
	logDebug(env->emu, "Hook me Captain Cook!\n");
	logDebug(env->emu, "%s:%i %s\n",__FILE__,__LINE__,__FUNCTION__);

	struct emu_cpu *c = emu_cpu_get(env->emu);

	uint32_t eip_save;
	POP_DWORD(c, &eip_save);
/* IEWinMain(LPSTR pszCommandLine, int nShowWindow) */

	uint32_t pszCommandLine;
	POP_DWORD(c, &pszCommandLine);

	uint32_t nShowWindow;
	POP_DWORD(c, &nShowWindow);

	struct emu_string *CommandLine = emu_string_new();
	emu_memory_read_string(c->mem, pszCommandLine, CommandLine, 512);

	uint32_t returnvalue = 0;

	if ( hook->hook.win->userhook != NULL )
	{
		returnvalue = hook->hook.win->userhook(env, hook, 
											   emu_string_char(CommandLine),
											   nShowWindow);
	}else
	{
		returnvalue = 0;
	}

	if (env->profile != NULL)
	{
		emu_profile_function_add(env->profile, "IEWinMain");

		emu_profile_argument_add_ptr(env->profile, "LPSTR", "pszCommandLine", pszCommandLine);
		emu_profile_argument_add_string(env->profile, "", "", emu_string_char(CommandLine)); 
		emu_profile_argument_add_int(env->profile, "int", "nShowWindow", nShowWindow);

		emu_profile_function_returnvalue_int_set(env->profile, "HRESULT", returnvalue);
	}

	emu_cpu_reg32_set(c, eax, returnvalue);
	emu_string_free(CommandLine);
	emu_cpu_eip_set(c, eip_save);
	return 0;
}
