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

	int s = 67;//socket(af, type, protocol);
	printf("socket %i \n", s);
	emu_cpu_reg32_set(c, eax, s);

	emu_cpu_eip_set(c, eip_save);
	return 0;
}


int32_t	emu_env_w32_hook_bind(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex)
{
	printf("Hook me Captain Cook!\n");
	printf("%s %s:%i\n",__FUNCTION__,__FILE__,__LINE__);

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

	emu_cpu_reg32_set(c, eax, 0);

	emu_cpu_eip_set(c, eip_save);
	return 0;
}

int32_t	emu_env_w32_hook_listen(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex)
{
	printf("Hook me Captain Cook!\n");
	printf("%s %s:%i\n",__FUNCTION__,__FILE__,__LINE__);

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
	
	emu_cpu_reg32_set(c, eax, 0);

	emu_cpu_eip_set(c, eip_save);
	return 0;
}

int32_t	emu_env_w32_hook_accept(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex)
{
	printf("Hook me Captain Cook!\n");
	printf("%s %s:%i\n",__FUNCTION__,__FILE__,__LINE__);

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

	int a = 89;
	printf("accept %i \n", a);	
	emu_cpu_reg32_set(c, eax, a);

	emu_cpu_eip_set(c, eip_save);
	return 0;
}

int32_t	emu_env_w32_hook_closesocket(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex)
{
	printf("Hook me Captain Cook!\n");
	printf("%s %s:%i\n",__FUNCTION__,__FILE__,__LINE__);

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


	emu_cpu_reg32_set(c, eax, 0);

	emu_cpu_eip_set(c, eip_save);
	return 0;
}



int32_t	emu_env_w32_hook_connect(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex)
{
	printf("Hook me Captain Cook!\n");
	printf("%s %s:%i\n",__FUNCTION__,__FILE__,__LINE__);

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


	emu_cpu_reg32_set(c, eax, 0);

	emu_cpu_eip_set(c, eip_save);
	return 0;
}



int32_t	emu_env_w32_hook_CreateProcessA(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex)
{
	printf("Hook me Captain Cook!\n");
	printf("%s %s:%i\n",__FUNCTION__,__FILE__,__LINE__);

	struct emu_cpu *c = emu_cpu_get(env->emu);

	uint32_t eip_save;

	POP_DWORD(c, &eip_save);

/*BOOL CreateProcess( 
  LPCWSTR pszImageName, 
  LPCWSTR pszCmdLine, 
  LPSECURITY_ATTRIBUTES psaProcess, 
  LPSECURITY_ATTRIBUTES psaThread, 
  BOOL fInheritHandles, 
  DWORD fdwCreate, 
  LPVOID pvEnvironment, 
  LPWSTR pszCurDir, 
  LPSTARTUPINFOW psiStartInfo, 
  LPPROCESS_INFORMATION pProcInfo
);*/

	uint32_t imagename;
	POP_DWORD(c, &imagename);

	uint32_t cmdline;
	POP_DWORD(c, &cmdline);

	uint32_t process;
	POP_DWORD(c, &process);

	uint32_t thread;
	POP_DWORD(c, &thread);

	uint32_t inherithandles;
	POP_DWORD(c, &inherithandles);

	uint32_t create;
	POP_DWORD(c, &create);

	uint32_t enviroment;
	POP_DWORD(c, &enviroment);

	uint32_t cwd;
	POP_DWORD(c, &cwd);

	uint32_t startinfo;
	POP_DWORD(c, &startinfo);

	uint32_t procinfo;
	POP_DWORD(c, &procinfo);


	printf("CreateProcessA \n");	
	emu_cpu_reg32_set(c, eax, 0);

	emu_cpu_eip_set(c, eip_save);
	return 0;
}


int32_t	emu_env_w32_hook_WinExec(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex)
{
	printf("Hook me Captain Cook!\n");
	printf("%s %s:%i\n",__FUNCTION__,__FILE__,__LINE__);

	struct emu_cpu *c = emu_cpu_get(env->emu);

	uint32_t eip_save;

	POP_DWORD(c, &eip_save);

/*
UINT WINAPI WinExec(
  LPCSTR lpCmdLine,
  UINT uCmdShow
);
*/

	uint32_t cmdline_ptr;
	POP_DWORD(c, &cmdline_ptr);

	uint32_t show;
	POP_DWORD(c, &show);


	uint8_t b=0;
	uint32_t strsize =0;

	printf("esp is 0x%08x\n", cmdline_ptr);

	while (emu_memory_read_byte(emu_memory_get(env->emu), cmdline_ptr + strsize, &b) == 0 && b != 0)
	{
//		printf(" 0x%08x = %02x\n",cmdline_ptr + strsize,b);
		strsize++;
	}

	char *cmdline = (char *)malloc(strsize+1);
	memset(cmdline, 0, strsize+1);
	emu_memory_read_block(emu_memory_get(env->emu), cmdline_ptr,cmdline, strsize);

	printf("WinExec %s\n", cmdline);
	free(cmdline);


	emu_cpu_reg32_set(c, eax, 32);

	emu_cpu_eip_set(c, eip_save);
	return 0;
}

