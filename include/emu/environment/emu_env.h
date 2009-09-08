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

struct emu_env_linux;
struct emu_env_linux_syscall;
struct emu_env_w32;
struct emu_env_w32_dll_export;



/*
typedef void *(*new_env)(struct emu *e);
typedef void (*free_env)(void *env);
typedef int32_t (*env_hook)(void *env, const char *syscallname,
							int (*userhook)(void *env, void *syscall, ...),
							void *userdata);

typedef int32_t (*env_pre_check)(void *env);
typedef int32_t (*env_post_check)(void *env);


struct env_helper
{
	void *env;
	void *(*new_env)(struct emu *e);
	void (*free_env)(void *env);

	int32_t (*env_hook)(void *env, const char *syscallname,
						int (*userhook)(void *env, void *syscall, ...),
						void *userdata);

	int32_t (*env_pre_check)(void *env);	
	int32_t (*env_post_check)(void *env);
};


struct env_helper envs[] = 
{
	{
		NULL,
		(new_env)emu_env_w32_new,
		(free_env)emu_env_w32_free,
		(env_hook)NULL,
		(env_pre_check) NULL,		
		(env_post_check) NULL
	},
	{
		NULL,
		(new_env)emu_env_linux_new,
		(free_env)emu_env_linux_free,
		(env_hook)emu_env_linux_syscall_hook,
		(env_pre_check) NULL,		
		(env_post_check) NULL
	}
};

*/

enum emu_env_type
{
	emu_env_type_win32,
	emu_env_type_linux,
};

struct emu_env_hook
{
	enum emu_env_type type;

	union 
	{
		struct emu_env_w32_dll_export *win;
		struct emu_env_linux_syscall  *lin;
	} hook;
};

struct emu_env
{
	struct
	{
		struct emu_env_w32   *win;
		struct emu_env_linux *lin;
	} env;


	struct emu *emu;
//	struct env_helper *envs;
	struct emu_profile *profile;
	void *userdata;
};

struct emu_env *emu_env_new(struct emu *e);
void emu_env_free(struct emu_env *env);
