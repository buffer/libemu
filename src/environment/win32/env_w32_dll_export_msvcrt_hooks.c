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
#include "emu/environment/win32/env_w32_dll_export_msvcrt_hooks.h"

int32_t	env_w32_hook__execv(struct emu_env *env, struct emu_env_hook *hook)
{
	printf("Hook me Captain Cook!\n");
	printf("%s:%i %s\n",__FILE__,__LINE__,__FUNCTION__);

	struct emu_cpu *c = emu_cpu_get(env->emu);

	uint32_t eip_save;

	POP_DWORD(c, &eip_save);

/*
intptr_t _execv( 
   const char *cmdname,
   const char *const *argv 
);
intptr_t _wexecv( 
   const wchar_t *cmdname,
   const wchar_t *const *argv 
);
*/
	emu_profile_function_add(env->profile, "_execv");

	uint32_t p_cmdname;
	POP_DWORD(c, &p_cmdname);
	emu_profile_argument_add_ptr(env->profile, "const char *", "cmdname", p_cmdname);

	struct emu_string *cmdname = emu_string_new();
	emu_memory_read_string(c->mem, p_cmdname, cmdname, 512);
	emu_profile_argument_add_string(env->profile, "", "", emu_string_char(cmdname)); 



	uint32_t p_argv;
	POP_DWORD(c, &p_argv);
	emu_profile_argument_add_ptr(env->profile, "const char *const *", "argv", p_argv);
	emu_profile_argument_add_none(env->profile);



	emu_profile_function_returnvalue_int_set(env->profile, "int ", 0);

    emu_cpu_eip_set(c, eip_save);
	return 0;
}

