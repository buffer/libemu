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

#ifndef HAVE_EMU_ENV_W32_DLL_EXPORT_H
#define HAVE_EMU_ENV_W32_DLL_EXPORT_H


struct emu;
struct emu_env_w32;
struct emu_env;
struct emu_env_hook;

typedef uint32_t	(*win32userhook)(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex, ...);

struct emu_env_w32_dll_export
{
	char 		*fnname;
	uint32_t 	virtualaddr;
    int32_t		(*fnhook)(struct emu_env *env, struct emu_env_hook *hook);
	void 		*userdata;
	uint32_t	(*userhook)(struct emu_env *env, struct emu_env_hook *hook, ...);
	uint32_t	ordinal;
};

struct emu_env_w32_dll_export *emu_env_w32_dll_export_new(void);
void emu_env_w32_dll_export_copy(struct emu_env_w32_dll_export *to, struct emu_env_w32_dll_export *from);
void emu_env_w32_dll_export_free(struct emu_env_w32_dll_export *exp);

extern struct emu_env_w32_dll_export kernel32_exports[];
extern struct emu_env_w32_dll_export ws2_32_exports[];
extern struct emu_env_w32_dll_export wininet_exports[];
extern struct emu_env_w32_dll_export urlmon_exports[];


#endif
