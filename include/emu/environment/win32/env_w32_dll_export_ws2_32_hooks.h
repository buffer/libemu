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

int32_t env_w32_hook_accept(struct emu_env *env, struct emu_env_hook *hook);
int32_t env_w32_hook_bind(struct emu_env *env, struct emu_env_hook *hook);
int32_t env_w32_hook_closesocket(struct emu_env *env, struct emu_env_hook *hook);
int32_t env_w32_hook_connect(struct emu_env *env, struct emu_env_hook *hook);
int32_t env_w32_hook_listen(struct emu_env *env, struct emu_env_hook *hook);
int32_t env_w32_hook_recv(struct emu_env *env, struct emu_env_hook *hook);
int32_t env_w32_hook_send(struct emu_env *env, struct emu_env_hook *hook);
int32_t env_w32_hook_sendto(struct emu_env *env, struct emu_env_hook *hook);
int32_t env_w32_hook_socket(struct emu_env *env, struct emu_env_hook *hook);
int32_t env_w32_hook_WSASocketA(struct emu_env *env, struct emu_env_hook *hook);
int32_t env_w32_hook_WSAStartup(struct emu_env *env, struct emu_env_hook *hook);

