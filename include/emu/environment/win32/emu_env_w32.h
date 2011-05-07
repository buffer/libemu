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

#ifndef HAVE_EMU_ENV_W32
#define HAVE_EMU_ENV_W32

#include <stdint.h>

struct emu;
struct emu_env_w32_dll;
struct emu_env_w32_dll_export;
struct emu_profile;
struct emu_env;
struct emu_env_hook;

/**
 * the emu win32 enviroment struct
 * 
 * @see emu_env_w32_new
 */
struct emu_env_w32
{
	/**
	 * pointer to the emu
	 */
	struct emu *emu;
	/**
	 * array of pointers to the dlls loaded to memory
	 */
	struct emu_env_w32_dll **loaded_dlls;
	/**
	 * the baseaddress for the env
	 */
	uint32_t	baseaddr;

	/**
	 * for SEH
	 */
	uint32_t last_good_eip;
	uint32_t lastExceptionHandler;
	uint32_t exception_count;
};

/**
 * Create a new emu_env_w32 environment
 * 
 * @param e      the emulation to create the w32 process environment in
 * 
 * @return on success: pointer to the emu_env_w32 create
 *         on failure: NULL
 */
struct emu_env_w32 *emu_env_w32_new(struct emu *e);

/**
 * Free the emu_env_w32, free all dlls etc
 * 
 * @param env    the env to free
 */
void emu_env_w32_free(struct emu_env_w32 *env);

int32_t emu_env_w32_load_dll(struct emu_env_w32 *env, char *path);

/**
 * Hook an dll export from a dll
 * 
 * @param env        the env
 * @param exportname the exportname, f.e. "socket"
 * @param fnhook     pointer to the hook function
 * 
 * @return on success: 0
 *         on failure: -1
 */
int32_t emu_env_w32_export_hook(struct emu_env *env,
								const char *exportname, 
								uint32_t		(*fnhook)(struct emu_env *env, struct emu_env_hook *hook, ...),
								void *userdata);



/**
 * Check if eip is within a loaded dll,
 *  - call the dll's export function
 * 
 * @param env    the env
 * 
 * @return on success: pointer to the dll_export
 *         on failure: NULL
 */
struct emu_env_hook *emu_env_w32_eip_check(struct emu_env *env);


int32_t emu_env_w32_step_failed(struct emu_env *env);
#endif

