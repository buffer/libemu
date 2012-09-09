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

// for the socket hooks
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <stdlib.h>


#include "emu/emu_cpu.h"
#include "emu/emu_cpu_data.h"
#include "emu/emu_memory.h"
#include "emu/emu_string.h"
#include "emu/environment/emu_env.h"
#include "emu/environment/emu_profile.h"
#include "emu/environment/linux/emu_env_linux.h"

int32_t env_linux_hook_exit(struct emu_env *env, struct emu_env_hook *hook)
{
	printf("sys_exit(2)\n");
	struct emu_cpu *c = emu_cpu_get(env->emu);
	emu_profile_function_add(env->profile, "exit");
	emu_profile_argument_add_int(env->profile, "int", "status", c->reg[ebx]);

	if (hook->hook.lin->userhook != NULL)
	{
		uint32_t r = hook->hook.lin->userhook(env, hook, c->reg[ebx]);
		emu_cpu_reg32_set(c, eax, r);
	}else
		emu_cpu_reg32_set(c, eax, 0);

	
	return 0;
}

int32_t env_linux_hook_chmod(struct emu_env *env, struct emu_env_hook *hook)
{
	printf("sys_chmod(2)\n");
	struct emu_cpu *c = emu_cpu_get(env->emu);
	emu_profile_function_add(env->profile, "chmod");

	if (hook->hook.lin->userhook != NULL)
	{
		uint32_t r = hook->hook.lin->userhook(env, hook, c->reg[ebx]);
		emu_cpu_reg32_set(c, eax, r);
	}else
		emu_cpu_reg32_set(c, eax, 0);

	return 0;
}


int32_t env_linux_hook_fork(struct emu_env *env, struct emu_env_hook *hook)
{
	printf("sys_fork(2)\n");
	struct emu_cpu *c = emu_cpu_get(env->emu);
	emu_profile_function_add(env->profile, "fork");

	emu_cpu_reg32_set(c, eax, 4711);
	return 0;
}

int32_t env_linux_hook_execve(struct emu_env *env, struct emu_env_hook *hook)
{
	printf("execve\n");
	struct emu_cpu *c = emu_cpu_get(env->emu);

	struct emu_string *name = emu_string_new();
	emu_memory_read_string(emu_memory_get(c->emu), c->reg[ebx], name, 255);

	if ( env->profile != NULL )
	{
		emu_profile_function_add(env->profile, "execve");
		emu_profile_argument_add_ptr(env->profile, "const char *", "dateiname", c->reg[ebx]);

		emu_profile_argument_add_string(env->profile, "", "", emu_string_char(name));
//	emu_profile_argument_add_ptr(env->profile, "", "", c->reg[ecx]);
		emu_profile_argument_array_start(env->profile, "const char *", "argv[]");
	}

	uint32_t p_array = c->reg[ecx];
	uint32_t p_arg = -1;
	emu_memory_read_dword(emu_memory_get(c->emu), p_array, &p_arg);
	int i=1;
//	char **argv = NULL;
	while (p_arg != 0)
	{
//		argv = realloc(argv, (i+1) * sizeof(char *));
//		argv[i] = NULL;

		struct emu_string *arg = emu_string_new();
		emu_memory_read_string(emu_memory_get(c->emu), p_arg, arg, 128);
//		argv[i-1] = strdup(emu_string_char(arg));

		if( emu_string_char(arg) == NULL )
		{
			emu_string_free(arg);
			break;
		}

		if ( env->profile != NULL )
		{
			emu_profile_argument_add_ptr(env->profile, "", "", p_array+((i-1)*4));
			emu_profile_argument_add_ptr(env->profile, "", "", p_arg);
			emu_profile_argument_add_string(env->profile, "", "", emu_string_char(arg));
		}

		emu_string_free(arg);
		emu_memory_read_dword(emu_memory_get(c->emu), p_array+(i*4), &p_arg);
		i++;
	}

	if ( env->profile != NULL )
	{
		emu_profile_argument_add_ptr(env->profile, "", "", p_arg);
		emu_profile_argument_add_none(env->profile);

//	printf("arg is %s\n", emu_string_char(arg));

		emu_profile_argument_array_end(env->profile);

		emu_profile_argument_add_ptr(env->profile, "const char *", "envp[]", c->reg[edx]);
		emu_profile_argument_add_none(env->profile);

		emu_profile_function_returnvalue_int_set(env->profile, "int", 0);
	}
	printf("int execve (const char *dateiname=%08x={%s}, const char * argv[], const char *envp[]);\n", 
		   c->reg[ebx],
		   emu_string_char(name));

	emu_string_free(name);
	return 0;
}


int32_t env_linux_hook_dup2(struct emu_env *env, struct emu_env_hook *hook)
{
	struct emu_cpu *c = emu_cpu_get(env->emu);

	printf("int dup2(int oldfd=%i, int newfd=%i);\n", c->reg[ebx], c->reg[ecx]);

	if ( env->profile != NULL )
	{
		emu_profile_function_add(env->profile, "dup2");
		emu_profile_argument_add_int(env->profile, "int", "oldfd", c->reg[ebx]);
		emu_profile_argument_add_int(env->profile, "int", "newfd", c->reg[ecx]);

		emu_profile_function_returnvalue_int_set(env->profile, "int", c->reg[ecx]);
	}

	emu_cpu_reg32_set(c, eax, c->reg[ecx]);
	return 0;
}



int32_t env_linux_hook_socketcall(struct emu_env *env, struct emu_env_hook *hook)
{
	struct emu_cpu *c = emu_cpu_get(env->emu);

#define AL(x) (x)
	static unsigned char nargs[18]={AL(0),AL(3),AL(3),AL(3),AL(2),AL(3),
		AL(3),AL(3),AL(4),AL(4),AL(4),AL(6),
		AL(6),AL(2),AL(5),AL(5),AL(3),AL(3)};
#undef AL

	uint32_t a[6];
	int i;
	for ( i=0;i<nargs[c->reg[ebx]];i++ )
	{
		emu_memory_read_dword(emu_memory_get(c->emu),c->reg[ecx]+4*i,a+i);
	}

	uint32_t returnvalue = 0;

	switch ( c->reg[ebx] )
	{
	case 1:	// SYS_SOCKET 
		printf("int socket(int domain=%i, int type=%i, int protocol=%i);\n",
			   a[0],
			   a[1],
			   a[2]);

		if (hook->hook.lin->userhook != NULL)
			returnvalue = hook->hook.lin->userhook(env, hook, a[0], a[1],  a[2]);
		else
			returnvalue = 14;

		if ( env->profile != NULL )
		{
			emu_profile_function_add(env->profile, "socket");
			emu_profile_argument_add_int(env->profile, "int", "domain",     a[0]);
			emu_profile_argument_add_int(env->profile, "int", "type",       a[1]);
			emu_profile_argument_add_int(env->profile, "int", "protocol",   a[2]);

			emu_profile_function_returnvalue_int_set(env->profile, "int", returnvalue);
		}

		emu_cpu_reg32_set(c, eax, returnvalue);

		break;

	case 2:	// SYS_BIND 
		{

/*
			printf("int bind(int sockfd=%i, struct sockaddr *my_addr=%08x={host %s port %i}, int addrlen);\n",
				   a[0],
				   a[1], inet_ntoa(*(struct in_addr *)&((struct sockaddr_in *)&sa)->sin_addr), ntohs(((struct sockaddr_in *)&sa)->sin_port) 
				  );
*/



			struct sockaddr sa;
			memset(&sa, 0, sizeof(struct sockaddr));
			emu_memory_read_block(emu_memory_get(c->emu), a[1], &sa, sizeof(struct sockaddr));


			if (hook->hook.lin->userhook != NULL)
				returnvalue = hook->hook.lin->userhook(env, hook, a[0], &sa,  a[2]);
			else
				returnvalue = 0;

			if (env->profile != NULL)
			{
				emu_profile_function_add(env->profile, "bind");
				emu_profile_argument_add_int(env->profile, "int", "sockfd", a[0]);
				emu_profile_argument_add_sockaddr_ptr(env->profile, "my_addr", a[1], sa);
				emu_profile_argument_add_int(env->profile, "int", "addrlen", a[2]);
				emu_profile_function_returnvalue_int_set(env->profile, "int", returnvalue);
			}
			
			emu_cpu_reg32_set(c, eax, returnvalue);

		}


		break;

	case 3:	// SYS_CONNECT 
		{

			printf("connect\n");

			struct sockaddr sa;
			memset(&sa, 0, sizeof(struct sockaddr));
			emu_memory_read_block(emu_memory_get(c->emu), a[1], &sa, sizeof(struct sockaddr));


			if (hook->hook.lin->userhook != NULL)
				returnvalue = hook->hook.lin->userhook(env, hook, a[0], &sa,  a[2]);
			else
				returnvalue = 0;

			if (env->profile != NULL)
			{
				emu_profile_function_add(env->profile, "connect");
				emu_profile_argument_add_int(env->profile, "int", "sockfd", a[0]);
				emu_profile_argument_add_sockaddr_ptr(env->profile, "serv_addr", a[1], sa);
				emu_profile_argument_add_int(env->profile, "int", "addrlen", a[2]);
				emu_profile_function_returnvalue_int_set(env->profile, "int", returnvalue);
			}

			emu_cpu_reg32_set(c, eax, returnvalue);
		}
		break;

	case 4:	// SYS_LISTEN 
		printf("int listen(int s=%i, int backlog=%i);\n", 
			   a[0], 
			   a[1]);

		if (hook->hook.lin->userhook != NULL)
			returnvalue = hook->hook.lin->userhook(env, hook, a[0], a[1]);
		else
			returnvalue = 0;

		if (env->profile != NULL)
		{
			emu_profile_function_add(env->profile, "listen");
			emu_profile_argument_add_int(env->profile, "int", "s", a[0]);
			emu_profile_argument_add_int(env->profile, "int", "backlog", a[1]);
			emu_profile_function_returnvalue_int_set(env->profile, "int", returnvalue);
		}
		
		emu_cpu_reg32_set(c, eax, returnvalue);

		break;

	case 5:	// SYS_ACCEPT 
		printf("int accept(int s=%i, struct sockaddr *addr=%08x, int *addrlen=%08x);\n",
			   a[0],
			   a[1],
			   a[2]);


		struct sockaddr sa;
		memset(&sa, 0, sizeof(struct sockaddr));
		emu_memory_read_block(emu_memory_get(c->emu), a[1], &sa, sizeof(struct sockaddr));


		if (hook->hook.lin->userhook != NULL)
			returnvalue = hook->hook.lin->userhook(env, hook, a[0], &sa, a[2]);
		else
			returnvalue = 19;


		if (env->profile != NULL)
		{
			emu_profile_function_add(env->profile, "accept");
			emu_profile_argument_add_int(env->profile, "int", "sockfd", a[0]);

			emu_profile_argument_add_ptr(env->profile, "sockaddr_in *", "addr", a[1]);
			emu_profile_argument_add_none(env->profile);

			emu_profile_argument_add_ptr(env->profile, "int", "addrlen", a[2]);
			emu_profile_argument_add_none(env->profile);
		
			emu_profile_function_returnvalue_int_set(env->profile, "int", returnvalue);
		}
		emu_cpu_reg32_set(c, eax, returnvalue);
		break;

	case 6:	// SYS_GETSOCKNAME 
		printf("sys_getsockname(2)\n");
		break;

	case 7:	// SYS_GETPEERNAME 
		printf("sys_getpeername(2)\n");
		break;

	case 8:	// SYS_SOCKETPAIR 
		printf("sys_socketpair(2)\n");
		break;

	case 9:	// SYS_SEND 
		printf("sys_send(2)\n");
		break;

	case 10: // SYS_RECV 
		printf("sys_recv(2)\n");
		break;

	case 11: // SYS_SENDTO 
		printf("sys_sendto(2)\n");
		break;

	case 12: // SYS_RECVFROM 
		printf("sys_recvfrom(2)\n");
		break;

	case 13: // SYS_SHUTDOWN 
		printf("sys_shutdown(2)\n");
		break;

	case 14: // SYS_SETSOCKOPT 
		printf("sys_setsockopt(2)\n");
		break;

	case 15: // SYS_GETSOCKOPT 
		printf("sys_getsockopt(2)\n");
		break;

	case 16: // SYS_SENDMSG 
		printf("sys_sendmsg(2)\n");
		break;

	case 17: // SYS_RECVMSG 
		printf("sys_recvmsg(2)\n");
		break;

	default:
		printf("syscall %i (%x) unknown",  c->reg[ebx], c->reg[ebx]);
	}

	return 0;
}


