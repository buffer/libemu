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

#ifndef HAVE_EMU_ENV_LINUX_H
#define HAVE_EMU_ENV_LINUX_H

#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "emu/emu.h"
#include "emu/emu_cpu.h"
#include "emu/emu_cpu_data.h"
#include "emu/emu_memory.h"
#include "emu/emu_hashtable.h"

struct emu_profile;
struct emu_env;

struct emu_env_linux
{
	struct emu *emu;
	struct emu_hashtable *syscall_hooks_by_name;
	struct emu_env_linux_syscall *syscall_hookx;
	struct emu_env_hook *hooks;
//	struct emu_profile *profile;
};

struct emu_env_linux *emu_env_linux_new(struct emu *e);
void emu_env_linux_free(struct emu_env_linux *eel);

struct emu_env_hook *emu_env_linux_syscall_check(struct emu_env *env);


struct emu_env_linux_syscall_entry
{
	const char *name;
	const char *(*fnhook)(struct emu_env_linux *env);
};


typedef uint32_t (*userhook)(struct emu_env_linux *env, struct emu_env_linux_syscall *syscall, ...);

struct emu_env_linux_syscall
{
	const char *name;
	int32_t		(*fnhook)(struct emu_env *env, struct emu_env_hook *hook);
	void *userdata;
	uint32_t (*userhook)(struct emu_env *env, struct emu_env_hook *hook, ...);
};

int32_t emu_env_linux_syscall_hook(struct emu_env *env, const char *syscallname,
								   uint32_t (*userhook)(struct emu_env *env, struct emu_env_hook *hook, ...),
								   void *userdata);

const char *env_linux_socketcall(struct emu_env_linux *env);

#endif
