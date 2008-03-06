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

#include "emu/emu.h"
#include "emu/emu_cpu.h"
#include "emu/emu_memory.h"


#include "emu/environment/emu_profile.h"
#include "emu/environment/linux/emu_env_linux.h"
#include "emu/environment/linux/env_linux_syscalls.h"

extern uint32_t dll_export_fnname_hash(void *key);
extern bool dll_export_fnname_cmp(void *a, void *b);

struct emu_env_linux *emu_env_linux_new(struct emu *e)
{
	struct emu_env_linux *eel = malloc(sizeof(struct emu_env_linux));
	memset(eel, 0, sizeof(struct emu_env_linux));
	eel->emu = e;

	eel->syscall_hooks_by_name = emu_hashtable_new(256, dll_export_fnname_hash,  dll_export_fnname_cmp);
	int i;
	eel->syscall_hooks = malloc(sizeof(syscall_hooks));
	memcpy(eel->syscall_hooks, syscall_hooks, sizeof(syscall_hooks));
	for (i=0;i<sizeof(syscall_hooks)/sizeof(struct emu_env_linux_syscall);i++)
	{
		emu_hashtable_insert(eel->syscall_hooks_by_name, (void *)eel->syscall_hooks[i].name, (void *)&eel->syscall_hooks[i]);
	}

	eel->profile = emu_profile_new();

	return eel;
}


void emu_env_linux_free(struct emu_env_linux *eel)
{
	emu_hashtable_free(eel->syscall_hooks_by_name);
	free(eel->syscall_hooks);
	emu_profile_free(eel->profile);
	free(eel);
}

struct emu_env_linux_syscall *emu_env_linux_syscall_check(struct emu_env_linux *env)
{
	struct emu_cpu *cpu = emu_cpu_get(env->emu);

	if ( cpu->instr.is_fpu == false 
		 &&  cpu->instr.opc == 0xcd 
		 &&  *cpu->instr.cpu.imm8 == 0x80 )
	{
		uint8_t callnum = *cpu->reg8[al];
		if ( callnum < sizeof(env_linux_syscalls) / sizeof(struct emu_env_linux_syscall_entry) )
		{
			const char *name = NULL ;
			if ( env_linux_syscalls[callnum].name != NULL )
			{
				name = env_linux_syscalls[callnum].name;
			}
			else
			if ( env_linux_syscalls[callnum].fnhook != NULL )
			{
				name = env_linux_syscalls[callnum].fnhook(env);
			}

			if ( name != NULL )
			{
				struct emu_hashtable_item *ehi = emu_hashtable_search(env->syscall_hooks_by_name, (void *)name);
				if ( ehi != NULL )
				{
					return(struct emu_env_linux_syscall *) ehi->value;
				}
			}
		}
	}


	return NULL;
}

int32_t emu_env_linux_syscall_hook(struct emu_env_linux *env, const char *syscallname,
								   int32_t		(*fnhook)(struct emu_env_linux *env, struct emu_env_linux_syscall *syscallfox),
								   void *userdata)
{

	struct emu_hashtable_item *ehi = emu_hashtable_search(env->syscall_hooks_by_name, (void *)syscallname);
	if (ehi != NULL)
	{
		struct emu_env_linux_syscall *syscall = (struct emu_env_linux_syscall *)ehi->value;
		syscall->fnhook = fnhook;
		syscall->userdata = userdata;
		return 0;
	}

	return -1;
}

const char *env_linux_socketcall(struct emu_env_linux *env)
{
	static const char *socketcall_table [] =
	{
		/*  0 */ NULL,
		/*  1 */ "socket",
		/*  2 */ "bind",
		/*  3 */ "connect",
		/*  4 */ "listen",
		/*  5 */ "accept",
		/*  6 */ "getsockname",
		/*  7 */ "getpeername",
		/*  8 */ "socketpair",
		/*  9 */ "send",
		/* 10 */ "recv",
		/* 11 */ "sendto",
		/* 12 */ "recvfrom",
		/* 13 */ "shutdown",
		/* 14 */ "setsockopt",
		/* 15 */ "getsockopt",
		/* 16 */ "sendmsg",
		/* 17 */ "recvmsg",
	};
	struct emu_cpu *cpu = emu_cpu_get(env->emu);

	if (cpu->reg[ebx] == 0 || cpu->reg[ebx] > 17)
		return NULL;

	return socketcall_table[cpu->reg[ebx]];
}
