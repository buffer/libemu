/* @header@ */
#include <stdlib.h>
#include <string.h>

#include "emu/emu.h"
#include "emu/emu_memory.h"
#include "emu/emu_cpu.h"
#include "emu/emu_cpu_data.h"
#include "emu/emu_cpu_stack.h"
#include "emu/enviroment/win32/emu_env_w32.h"
#include "emu/enviroment/win32/emu_env_w32_dll_export.h"


struct emu_env_w32_dll_export *emu_env_w32_dll_export_new()
{
	struct emu_env_w32_dll_export *exp = (struct emu_env_w32_dll_export *)malloc(sizeof(struct emu_env_w32_dll_export));
	memset(exp,0,sizeof(struct emu_env_w32_dll_export));
	return exp;
}

void emu_env_w32_dll_export_free(struct emu_env_w32_dll_export *exp)
{
	free(exp);
}




int32_t	emu_env_w32_hook_LoadLibrayA(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex)
{
	printf("Hook me Captain Cook!\n");
	printf("%s %s:%i\n",__FUNCTION__,__FILE__,__LINE__);
/* HMODULE WINAPI LoadLibrary(LPCTSTR lpFileName); */

	struct emu_cpu *c = emu_cpu_get(env->emu);

	uint32_t eip_save;
	POP_DWORD(c, &eip_save);

	uint32_t dllname_ptr;// = emu_cpu_reg32_get(c, esp);
	
	POP_DWORD(c, &dllname_ptr);

	uint8_t b=0;
	uint32_t strsize =0;

	printf("esp is 0x%08x\n", dllname_ptr);

	while (emu_memory_read_byte(emu_memory_get(env->emu), dllname_ptr + strsize, &b) == 0 && b != 0)
	{
		printf(" 0x%08x = %02x\n",dllname_ptr + strsize,b);
		strsize++;
	}

	char *dllname = (char *)malloc(strsize+1);
	memset(dllname, 0, strsize+1);
	emu_memory_read_block(emu_memory_get(env->emu), dllname_ptr,dllname, strsize);

	printf("dll name is '%s'\n",dllname);


	if (strcmp(dllname, "ws2_32") == 0)
	{
		emu_cpu_reg32_set(c, eax, 0x71A10000);
		emu_cpu_reg32_set(c, ecx, 0x7C801BF6);
		emu_cpu_reg32_set(c, edx, 0x00150608);

		emu_env_w32_load_dll(env, "ws2_32.dll");
	}else
	{
		printf("unknown dll %s \n", dllname);
	}

	free(dllname);

	emu_cpu_eip_set(c, eip_save);
	return 0;
}

int32_t	emu_env_w32_hook_WSAStartup(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex)
{
	printf("Hook me Captain Cook!\n");
	printf("%s %s:%i\n",__FUNCTION__,__FILE__,__LINE__);

	struct emu_cpu *c = emu_cpu_get(env->emu);

	uint32_t eip_save;

	POP_DWORD(c, &eip_save);

	uint32_t wsaversion;
	POP_DWORD(c, &wsaversion);
	printf("WSAStartup version %x\n", wsaversion);

	emu_cpu_reg32_set(c, eax, 0x0);

	emu_cpu_eip_set(c, eip_save);
	return 0;
}


#include <sys/types.h>
#include <sys/socket.h>

int32_t	emu_env_w32_hook_WSASocketA(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex)
{
	printf("Hook me Captain Cook!\n");
	printf("%s %s:%i\n",__FUNCTION__,__FILE__,__LINE__);

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

	int s = socket(af, type, protocol);
	printf("socket %i \n", s);
	emu_cpu_reg32_set(c, eax, s);

	emu_cpu_eip_set(c, eip_save);
	return 0;
}


