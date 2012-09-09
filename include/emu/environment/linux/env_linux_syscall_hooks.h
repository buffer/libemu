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

/* 1 exit */
int32_t env_linux_hook_exit(struct emu_env *env, struct emu_env_hook *hook);

/* 2 fork */
int32_t env_linux_hook_fork(struct emu_env *env, struct emu_env_hook *hook);

/* 11 execve */
int32_t env_linux_hook_execve(struct emu_env *env, struct emu_env_hook *hook);

/* 15 chmod */
int32_t env_linux_hook_chmod(struct emu_env *env, struct emu_env_hook *hook);

/* 63 dup2 */
int32_t env_linux_hook_dup2(struct emu_env *env, struct emu_env_hook *hook);

/* 102 socketcall */
/*
	accept
	bind
	connect
	getpeername - missing 
	getsockname - missing
	getsockopt - missing
	listen
	recv       - missing
	recvfrom   - missing
	recvmsg    - missing
	send       - missing
	sendmsg    - missing
	sendto     - missing
	setsockopt - missing
	shutdown   - missing
	socket     
	socketpair - missing
*/
int32_t	env_linux_hook_socketcall(struct emu_env *env, struct emu_env_hook *hook);





