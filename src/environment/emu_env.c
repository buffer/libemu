/********************************************************************************
 *                               libemu
 *
 *                    - x86 shellcode emulation -
 *
 *
 * Copyright (C) 2008  Paul Baecher & Markus Koetter
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

#include "emu/environment/emu_env.h"
#include "emu/environment/emu_profile.h"
#include "emu/environment/linux/emu_env_linux.h"
#include "emu/environment/win32/emu_env_w32.h"
#include "emu/environment/win32/emu_env_w32_dll_export.h"

struct emu_env *emu_env_new(struct emu *e)
{
	struct emu_env *env = malloc(sizeof(struct emu_env));
	memset(env, 0, sizeof(struct emu_env));
	env->env.lin = emu_env_linux_new(e);
	env->env.win = emu_env_w32_new(e);
	env->emu = e;
	env->profile = NULL;//emu_profile_new();
	return env;
	
}

void emu_env_free(struct emu_env *env)
{
	emu_env_w32_free(env->env.win);
	emu_env_linux_free(env->env.lin);
	if (env->profile != NULL)
		emu_profile_free(env->profile);
	free(env);
}
