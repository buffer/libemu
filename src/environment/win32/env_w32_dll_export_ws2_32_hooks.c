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

// for the socket hooks
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

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
#include "emu/environment/win32/emu_env_w32.h"
#include "emu/environment/win32/emu_env_w32_dll.h"
#include "emu/environment/win32/emu_env_w32_dll_export.h"
#include "emu/environment/win32/env_w32_dll_export_ws2_32_hooks.h"

int32_t	env_w32_hook_accept(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex)
{
	printf("Hook me Captain Cook!\n");
	printf("%s:%i %s\n",__FILE__,__LINE__,__FUNCTION__);

	struct emu_cpu *c = emu_cpu_get(env->emu);

	uint32_t eip_save;

	POP_DWORD(c, &eip_save);

/*SOCKET accept(
  SOCKET s,
  struct sockaddr* addr,
  int* addrlen
);*/

	uint32_t s;
	POP_DWORD(c, &s);

	uint32_t addr;
	POP_DWORD(c, &addr);

	uint32_t addrlen;
	POP_DWORD(c, &addrlen);

	printf("accept(s=%i, addr=%x, addrlen=%i);\n", s, addr, addrlen);

	int a = 67;

#ifdef HAVE_INTERACTIVE_HOOKS
	struct sockaddr sa;
	socklen_t sasize = sizeof(struct sockaddr);
	a=accept(s, &sa, &sasize);
#endif

	printf("accept %i \n", a);	
	emu_cpu_reg32_set(c, eax, a);

	emu_cpu_eip_set(c, eip_save);
	return 0;
}

int32_t	env_w32_hook_bind(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex)
{
	printf("Hook me Captain Cook!\n");
	printf("%s:%i %s\n",__FILE__,__LINE__,__FUNCTION__);

	struct emu_cpu *c = emu_cpu_get(env->emu);

	uint32_t eip_save;

	POP_DWORD(c, &eip_save);


/*int bind(
  SOCKET s,
  const struct sockaddr* name,
  int namelen
); */

	uint32_t s;
	POP_DWORD(c, &s);

	uint32_t name;
	POP_DWORD(c, &name);
	
	uint32_t namelen;
	POP_DWORD(c, &namelen);

	printf("bind(s=%i, name=%x, namelen=%i\n", s, name, namelen);

	struct sockaddr sa;
	emu_memory_read_block(emu_memory_get(env->emu), name, &sa, sizeof(struct sockaddr));
	if (sa.sa_family == AF_INET)
	{
		printf("host %s port %i\n", 
		   inet_ntoa(*(struct in_addr *)&((struct sockaddr_in *)&sa)->sin_addr),
		   ntohs(((struct sockaddr_in *)&sa)->sin_port));
	}

	int retval = 0;

#ifdef HAVE_INTERACTIVE_HOOKS
	retval = bind(s, &sa, sizeof(struct sockaddr));
#endif 

	emu_cpu_reg32_set(c, eax, retval);

	emu_cpu_eip_set(c, eip_save);
	return 0;
}


int32_t	env_w32_hook_closesocket(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex)
{
	printf("Hook me Captain Cook!\n");
	printf("%s:%i %s\n",__FILE__,__LINE__,__FUNCTION__);

	struct emu_cpu *c = emu_cpu_get(env->emu);

	uint32_t eip_save;

	POP_DWORD(c, &eip_save);
/*
int closesocket(
  SOCKET s
);
*/
	uint32_t s;
	POP_DWORD(c, &s);

#ifdef HAVE_INTERACTIVE_HOOKS
	close((int)s);
#endif 

	emu_cpu_reg32_set(c, eax, 0);

	emu_cpu_eip_set(c, eip_save);
	return 0;
}



int32_t	env_w32_hook_connect(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex)
{
	printf("Hook me Captain Cook!\n");
	printf("%s:%i %s\n",__FILE__,__LINE__,__FUNCTION__);

	struct emu_cpu *c = emu_cpu_get(env->emu);

	uint32_t eip_save;

	POP_DWORD(c, &eip_save);
/*
int connect(
  SOCKET s,
  const struct sockaddr* name,
  int namelen
)
*/
	uint32_t s;
	POP_DWORD(c, &s);

	uint32_t name;
	POP_DWORD(c, &name);

	uint32_t namelen;
	POP_DWORD(c, &namelen);

	struct sockaddr sa;
	emu_memory_read_block(emu_memory_get(env->emu), name, &sa, sizeof(struct sockaddr));
	printf("host %s port %i\n", 
		   inet_ntoa(*(struct in_addr *)&((struct sockaddr_in *)&sa)->sin_addr),
		   ntohs(((struct sockaddr_in *)&sa)->sin_port));


	int retval = 0;

#ifdef HAVE_INTERACTIVE_HOOKS
	struct sockaddr_in si;
	si.sin_port = htons(4444);
	si.sin_family = AF_INET;
	si.sin_addr.s_addr = inet_addr("127.0.0.1");
	connect(s, (struct sockaddr *)&si, sizeof(struct sockaddr_in));
#endif

	emu_cpu_reg32_set(c, eax, retval);

	emu_cpu_eip_set(c, eip_save);
	return 0;
}

int32_t	env_w32_hook_listen(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex)
{
	printf("Hook me Captain Cook!\n");
	printf("%s:%i %s\n",__FILE__,__LINE__,__FUNCTION__);

	struct emu_cpu *c = emu_cpu_get(env->emu);

	uint32_t eip_save;

	POP_DWORD(c, &eip_save);

/*int listen(
  SOCKET s,
  int backlog
);
*/

	uint32_t s;
	POP_DWORD(c, &s);

	uint32_t backlog;
	POP_DWORD(c, &backlog);

	printf("listen(s=%i,  backlog=%i)\n", s,  backlog);

	int retval = 0;

#ifdef HAVE_INTERACTIVE_HOOKS
	retval = listen(s, backlog);
#endif

	emu_cpu_reg32_set(c, eax, retval);

	emu_cpu_eip_set(c, eip_save);
	return 0;
}

int32_t	env_w32_hook_recv(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex)
{
	printf("Hook me Captain Cook!\n");
	printf("%s:%i %s\n",__FILE__,__LINE__,__FUNCTION__);

	struct emu_cpu *c = emu_cpu_get(env->emu);

	uint32_t eip_save;

	POP_DWORD(c, &eip_save);
/*
int recv(
  SOCKET s,
  char* buf,
  int len,
  int flags
);
*/
	uint32_t s;
	POP_DWORD(c, &s);

	uint32_t buf;
	POP_DWORD(c, &buf);

	uint32_t len;
	POP_DWORD(c, &len);

	uint32_t flags;
	POP_DWORD(c, &flags);

	uint32_t xlen = len;
	char *buffer = (char *)malloc(len);
	memset(buffer, 0, len);

#ifdef HAVE_INTERACTIVE_HOOKS
	len = recv(s, buffer, len, flags); 
#endif
	
	printf("recv(%i, 0x%08x, %i) == %i \n", s, buf, xlen, (int32_t)len);
	if ((int32_t)len > 0)
		emu_memory_write_block(emu_memory_get(env->emu), buf, buffer, len);
	free(buffer);


	emu_cpu_reg32_set(c, eax, len);

	emu_cpu_eip_set(c, eip_save);
	return 0;
}


int32_t	env_w32_hook_send(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex)
{
	printf("Hook me Captain Cook!\n");
	printf("%s:%i %s\n",__FILE__,__LINE__,__FUNCTION__);

	struct emu_cpu *c = emu_cpu_get(env->emu);

	uint32_t eip_save;

	POP_DWORD(c, &eip_save);
/*
int send(
  SOCKET s,
  const char* buf,
  int len,
  int flags
);
*/
	uint32_t s;
	POP_DWORD(c, &s);

	uint32_t buf;
	POP_DWORD(c, &buf);

	uint32_t len;
	POP_DWORD(c, &len);

	uint32_t flags;
	POP_DWORD(c, &flags);

	char *buffer = (char *)malloc(len);
	printf("send(%i, 0x%08x, %i,  %i)\n", s, buf, len, flags);
	emu_memory_read_block(emu_memory_get(env->emu), buf, buffer, len);

	int retval = len;

#ifdef HAVE_INTERACTIVE_HOOKS
	len = send(s, buffer, len, flags);
	printf("send %i (of %i) bytes\n", retval,  len);
#endif

	emu_cpu_reg32_set(c, eax, retval);
	free(buffer);

	printf("eip_save is %08x\n",  eip_save);
	emu_cpu_eip_set(c, eip_save);
	return 0;
}


int32_t	env_w32_hook_sendto(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex)
{
	printf("Hook me Captain Cook!\n");
	printf("%s:%i %s\n",__FILE__,__LINE__,__FUNCTION__);

	struct emu_cpu *c = emu_cpu_get(env->emu);

	uint32_t eip_save;

	POP_DWORD(c, &eip_save);
/*
int sendto(
  SOCKET s,
  const char* buf,
  int len,
  int flags,
  const struct sockaddr* to,
  int tolen
);
*/
	uint32_t s;
	POP_DWORD(c, &s);

	uint32_t buf;
	POP_DWORD(c, &buf);

	uint32_t len;
	POP_DWORD(c, &len);

	uint32_t flags;
	POP_DWORD(c, &flags);

	uint32_t p_to;
	POP_DWORD(c, &p_to);

	uint32_t tolen;
	POP_DWORD(c, &tolen);


	char *buffer = (char *)malloc(len);
//	printf("send(%i, 0x%08x, %i,  %i)\n", s, buf, len, flags);
	emu_memory_read_block(emu_memory_get(env->emu), buf, buffer, len);
	int retval = len; //send(s, buffer, len, flags);
//	printf("send %i (of %i) bytes\n", retval,  len);
	emu_cpu_reg32_set(c, eax, retval);
	free(buffer);

	printf("eip_save is %08x\n",  eip_save);
	emu_cpu_eip_set(c, eip_save);
	return 0;
}

int32_t	env_w32_hook_socket(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex)
{
	printf("Hook me Captain Cook!\n");
	printf("%s:%i %s\n",__FILE__,__LINE__,__FUNCTION__);

	struct emu_cpu *c = emu_cpu_get(env->emu);

	uint32_t eip_save;

	POP_DWORD(c, &eip_save);
/*
SOCKET WSAAPI socket(
  int af,
  int type,
  int protocol
);
*/

	uint32_t af;
	POP_DWORD(c, &af);

	uint32_t type;
	POP_DWORD(c, &type);

	uint32_t protocol;
	POP_DWORD(c, &protocol);

	int s = 111;

#ifdef HAVE_INTERACTIVE_HOOKS
	s=socket(af, type, protocol);
#endif

	printf("socket %i \n", s);
	emu_cpu_reg32_set(c, eax, s);

	emu_cpu_eip_set(c, eip_save);
	return 0;
}

int32_t	env_w32_hook_WSASocketA(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex)
{
	printf("Hook me Captain Cook!\n");
	printf("%s:%i %s\n",__FILE__,__LINE__,__FUNCTION__);

	struct emu_cpu *c = emu_cpu_get(env->emu);

	uint32_t eip_save;

	POP_DWORD(c, &eip_save);


/* SOCKET WSASocket(
  int af,
  int type,
  int protocol,
  LPWSAPROTOCOL_INFO lpProtocolInfo,
  GROUP g,
  DWORD dwFlags
); */

	uint32_t af;
	POP_DWORD(c, &af);

	uint32_t type;
	POP_DWORD(c, &type);

	uint32_t protocol;
	POP_DWORD(c, &protocol);

	uint32_t protocolinfo;
	POP_DWORD(c, &protocolinfo);

	uint32_t group;
	POP_DWORD(c, &group);

	uint32_t flags;
	POP_DWORD(c, &flags);

	printf("SOCKET WSASocket(af=%i, type=%i, protocol=%i, lpProtocolInfo=%x, group=%i, dwFlags=%i);\n",
		   af, type, protocol, protocolinfo, group,  flags);

	int s = 113;

#ifdef HAVE_INTERACTIVE_HOOKS
	s=socket(af, type, protocol);
#endif

	printf("socket %i \n", s);
	emu_cpu_reg32_set(c, eax, s);

	emu_cpu_eip_set(c, eip_save);
	return 0;
}


int32_t	env_w32_hook_WSAStartup(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex)
{
	printf("Hook me Captain Cook!\n");
	printf("%s:%i %s\n",__FILE__,__LINE__,__FUNCTION__);

	struct emu_cpu *c = emu_cpu_get(env->emu);

	uint32_t eip_save;

	POP_DWORD(c, &eip_save);

/*
int WSAStartup(
  WORD wVersionRequested,
  LPWSADATA lpWSAData
);
*/

	uint32_t wsaversionreq;
	POP_DWORD(c, &wsaversionreq);
	printf("WSAStartup version %x\n", wsaversionreq);

	uint32_t wsadata;
	POP_DWORD(c, &wsadata);


	emu_cpu_reg32_set(c, eax, 0x0);

	emu_cpu_eip_set(c, eip_save);
	return 0;
}

