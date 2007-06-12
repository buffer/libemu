
/* @header@ */
#include <stdlib.h>
#include <string.h>

// for the socket hooks
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <sys/types.h>
#include <unistd.h>


#include "emu/emu.h"
#include "emu/emu_memory.h"
#include "emu/emu_cpu.h"
#include "emu/emu_cpu_data.h"
#include "emu/emu_cpu_stack.h"
#include "emu/emu_hashtable.h"
#include "emu/environment/win32/emu_env_w32.h"
#include "emu/environment/win32/emu_env_w32_dll_export.h"
#include "emu/environment/win32/emu_env_w32_dll.h"
#include "emu/emu_string.h"

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

void emu_env_w32_dll_export_copy(struct emu_env_w32_dll_export *to, struct emu_env_w32_dll_export *from)
{
	to->fnhook = from->fnhook;
	to->fnname = from->fnname;
	to->virtualaddr = from->virtualaddr;
}

int32_t emu_env_w32_hook_GetProcAddress(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex)
{
	printf("Hook me Captain Cook!\n");
	printf("%s:%i %s\n",__FILE__,__LINE__,__FUNCTION__);

	struct emu_cpu *c = emu_cpu_get(env->emu);

	uint32_t eip_save;
	POP_DWORD(c, &eip_save);

/* 
FARPROC WINAPI GetProcAddress(
  HMODULE hModule,
  LPCSTR lpProcName
);
*/


	uint32_t module;// = emu_cpu_reg32_get(c, esp);
	POP_DWORD(c, &module);

	printf("module ptr is %08x\n", module);

	uint32_t p_procname;
	POP_DWORD(c, &p_procname);

	struct emu_string *procname = emu_string_new();
	struct emu_memory *mem = emu_memory_get(env->emu);
	emu_memory_read_string(mem, p_procname, procname, 256);


	printf("procname name is '%s'\n", emu_string_char(procname));

	int i;
	for ( i=0; env->loaded_dlls[i] != NULL; i++ )
	{
		if ( env->loaded_dlls[i]->baseaddr == module )
		{
			printf("dll is %s %08x %08x \n", env->loaded_dlls[i]->dllname, module, env->loaded_dlls[i]->baseaddr);

			struct emu_env_w32_dll *dll = env->loaded_dlls[i];
			struct emu_hashtable_item *ehi = emu_hashtable_search(dll->exports_by_fnname, (void *)emu_string_char(procname));

			struct emu_env_w32_dll_export *ex = (struct emu_env_w32_dll_export *)ehi->value;

			if ( ehi == NULL )
			{
				break;
			}
			else
			{
				printf("found %s at addr %08x\n",emu_string_char(procname), dll->baseaddr + ex->virtualaddr );
				emu_cpu_reg32_set(c, eax, dll->baseaddr + ex->virtualaddr);
				break;
			}
		}
	}

	emu_string_free(procname);


	emu_cpu_eip_set(c, eip_save);
	return 0;
}




int32_t	emu_env_w32_hook_LoadLibrayA(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex)
{
	printf("Hook me Captain Cook!\n");
	printf("%s:%i %s\n",__FILE__,__LINE__,__FUNCTION__);
/* HMODULE WINAPI LoadLibrary(LPCTSTR lpFileName); */

	struct emu_cpu *c = emu_cpu_get(env->emu);

	uint32_t eip_save;
	POP_DWORD(c, &eip_save);

	uint32_t dllname_ptr;// = emu_cpu_reg32_get(c, esp);
	
	POP_DWORD(c, &dllname_ptr);

    struct emu_string *dllstr = emu_string_new();
    struct emu_memory *mem = emu_memory_get(env->emu);
    emu_memory_read_string(mem, dllname_ptr, dllstr, 256);

	char *dllname = emu_string_char(dllstr);

	int i;
	int found_dll = 0;
	for (i=0; env->loaded_dlls[i] != NULL; i++)
	{
		if (strncasecmp(env->loaded_dlls[i]->dllname, dllname, strlen(env->loaded_dlls[i]->dllname)) == 0)
		{
			printf("found dll %s, baseaddr is %08x \n",env->loaded_dlls[i]->dllname,env->loaded_dlls[i]->baseaddr);
			emu_cpu_reg32_set(c, eax, env->loaded_dlls[i]->baseaddr);
			found_dll = 1;
		}
	}
	
	if (found_dll == 0)
	{
        if (emu_env_w32_load_dll(env, dllname) == 0)
        {
            emu_cpu_reg32_set(c, eax, env->loaded_dlls[i]->baseaddr);        
        }
        else
        {
            printf("error could not find %s\n", dllname);
            emu_cpu_reg32_set(c, eax, 0x4711);
        }
	}

	emu_string_free(dllstr);

	emu_cpu_eip_set(c, eip_save);
	return 0;
}

int32_t	emu_env_w32_hook_WSAStartup(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex)
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



int32_t	emu_env_w32_hook_WSASocketA(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex)
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

	int s = socket(af, type, protocol);
	printf("socket %i \n", s);
	emu_cpu_reg32_set(c, eax, s);

	emu_cpu_eip_set(c, eip_save);
	return 0;
}


int32_t	emu_env_w32_hook_socket(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex)
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

	int s = socket(af, type, protocol);
	printf("socket %i \n", s);
	emu_cpu_reg32_set(c, eax, s);

	emu_cpu_eip_set(c, eip_save);
	return 0;
}


int32_t	emu_env_w32_hook_bind(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex)
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



	struct sockaddr sa;
	emu_memory_read_block(emu_memory_get(env->emu), name, &sa, sizeof(struct sockaddr));
	printf("host %s port %i\n", 
		   inet_ntoa(*(struct in_addr *)&((struct sockaddr_in *)&sa)->sin_addr),
		   ntohs(((struct sockaddr_in *)&sa)->sin_port));

	int retval = bind(s, &sa, sizeof(struct sockaddr));
	emu_cpu_reg32_set(c, eax, retval);

	emu_cpu_eip_set(c, eip_save);
	return 0;
}

int32_t	emu_env_w32_hook_listen(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex)
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

	int retval = listen(s, backlog);
	emu_cpu_reg32_set(c, eax, retval);

	emu_cpu_eip_set(c, eip_save);
	return 0;
}

int32_t	emu_env_w32_hook_accept(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex)
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

	struct sockaddr sa;
	socklen_t sasize = sizeof(struct sockaddr);
	int a = accept(s, &sa, &sasize);
	printf("accept %i \n", a);	
	emu_cpu_reg32_set(c, eax, a);

	emu_cpu_eip_set(c, eip_save);
	return 0;
}

int32_t	emu_env_w32_hook_closesocket(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex)
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

	close((int)s);

	emu_cpu_reg32_set(c, eax, 0);

	emu_cpu_eip_set(c, eip_save);
	return 0;
}



int32_t	emu_env_w32_hook_connect(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex)
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

	struct sockaddr_in si;
	si.sin_port = htons(4444);
	si.sin_family = AF_INET;
	si.sin_addr.s_addr = inet_addr("127.0.0.1");

	int retval = connect(s, (struct sockaddr *)&si, sizeof(struct sockaddr_in));
	emu_cpu_reg32_set(c, eax, retval);

	emu_cpu_eip_set(c, eip_save);
	return 0;
}


int32_t	emu_env_w32_hook_recv(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex)
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
	len = recv(s, buffer, len, flags); 
	printf("recv(%i, 0x%08x, %i) == %i \n", s, buf, xlen, (int32_t)len);
	if ((int32_t)len > 0)
		emu_memory_write_block(emu_memory_get(env->emu), buf, buffer, len);
	free(buffer);


	emu_cpu_reg32_set(c, eax, len);

	emu_cpu_eip_set(c, eip_save);
	return 0;
}


int32_t	emu_env_w32_hook_send(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex)
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
	int retval = send(s, buffer, len, flags);
	printf("send %i (of %i) bytes\n", retval,  len);
	emu_cpu_reg32_set(c, eax, retval);
	free(buffer);

	printf("eip_save is %08x\n",  eip_save);
	emu_cpu_eip_set(c, eip_save);
	return 0;
}



int32_t	emu_env_w32_hook_CreateProcessA(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex)
{
	printf("Hook me Captain Cook!\n");
	printf("%s:%i %s\n",__FILE__,__LINE__,__FUNCTION__);

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

	uint32_t environment;
	POP_DWORD(c, &environment);

	uint32_t cwd;
	POP_DWORD(c, &cwd);

	uint32_t p_startinfo;
	POP_DWORD(c, &p_startinfo);

	uint32_t p_procinfo;
	POP_DWORD(c, &p_procinfo);


	printf("CreateProcessA \n");	
	emu_cpu_reg32_set(c, eax, 0);

	struct emu_memory *m = emu_memory_get(env->emu);
    PROCESS_INFORMATION *pi = malloc(sizeof(PROCESS_INFORMATION));
	emu_memory_read_block(m, p_procinfo, pi, sizeof(PROCESS_INFORMATION));

	STARTUPINFO *si = malloc(sizeof(STARTUPINFO));
	emu_memory_read_block(m, p_startinfo, si, sizeof(STARTUPINFO));

	fflush(NULL);

// the code is meant to be an example how one could do it
#if 0
	

	pid_t pid;
	if ((pid = fork()) == 0)
	{ // child

#ifdef UNDEFINED_PROXY_SHELL


		int remote_socket = si->hStdInput;
		int shell_socket = socket(AF_INET, SOCK_STREAM, 0);

		struct sockaddr_in soai;
		soai.sin_family=AF_INET;
		soai.sin_port=htons(atoi("1234"));
		soai.sin_addr.s_addr=inet_addr("127.0.0.1");
		memset(&soai.sin_zero, 0, sizeof(soai.sin_zero));

		if ( connect(shell_socket, (struct sockaddr*)&soai, sizeof(soai)) < 0 )
		{
			perror("connect error");
		}

#define        MAX(a,b) (((a)>(b))?(a):(b))

		while (1)
		{
        		struct timeval timeout = {1,0};
				fd_set r_fds; 
				FD_ZERO(&r_fds);
				FD_SET(shell_socket, &r_fds);
				FD_SET(remote_socket, &r_fds);

				int r_sockets = select(MAX(shell_socket, remote_socket)+1, &r_fds, NULL, NULL, &timeout);

				if (r_sockets != 0)
				{
					int from;
					int to;
					from = to = shell_socket;
					if (FD_ISSET(shell_socket,&r_fds))
                    	to = remote_socket;
					else
					if (FD_ISSET(remote_socket,&r_fds))
						from = remote_socket;

					char rxbuffer[256];
					int rxsize;
					rxsize = recv(from, rxbuffer, 256, 0);
					if (rxsize <= 0)
					{
						exit(EXIT_SUCCESS);
					}
					send(to, rxbuffer, rxsize, 0);
				}
		}
#endif 

#ifdef UNDEFINED_REAL_SHELL
		dup2(si->hStdInput,  fileno(stdin));
		dup2(si->hStdOutput, fileno(stdout));
		dup2(si->hStdError,  fileno(stderr));

        system("/tmp/cmd/cmdexe.pl -p winxp -l /tmp/cmd");
		exit(EXIT_SUCCESS);
#endif
	}else
	{ // parent 
		pi->hThread = pid;
	}

#endif // 0

	emu_cpu_eip_set(c, eip_save);
	return 0;
}


int32_t	emu_env_w32_hook_WinExec(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex)
{
	printf("Hook me Captain Cook!\n");
	printf("%s:%i %s\n",__FILE__,__LINE__,__FUNCTION__);

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


    struct emu_string *cmdstr = emu_string_new();
    emu_memory_read_string(emu_memory_get(env->emu), cmdline_ptr, cmdstr, 256);
	printf("WinExec %s\n", emu_string_char(cmdstr));
	emu_string_free(cmdstr);

	emu_cpu_reg32_set(c, eax, 32);

	emu_cpu_eip_set(c, eip_save);
	return 0;
}
       #include <sys/types.h>
       #include <sys/wait.h>



int32_t	emu_env_w32_hook_WaitForSingleObject(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex)
{
	printf("Hook me Captain Cook!\n");
	printf("%s:%i %s\n",__FILE__,__LINE__,__FUNCTION__);

	struct emu_cpu *c = emu_cpu_get(env->emu);

	uint32_t eip_save;

	POP_DWORD(c, &eip_save);

/*
DWORD WINAPI WaitForSingleObject(
  HANDLE hHandle,
  DWORD dwMilliseconds
);
*/

	uint32_t handle;
	POP_DWORD(c, &handle);

	uint32_t msecs;
	POP_DWORD(c, &msecs);

#if 0
// the code is meant to be an example how one could do it
	int status, options = 0;
	waitpid(handle, &status, options);
#endif 

	emu_cpu_reg32_set(c, eax, 32);

	emu_cpu_eip_set(c, eip_save);
	return 0;
}


int32_t	emu_env_w32_hook_ExitProcess(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex)
{
	printf("Hook me Captain Cook!\n");
	printf("%s:%i %s\n",__FILE__,__LINE__,__FUNCTION__);

	struct emu_cpu *c = emu_cpu_get(env->emu);

	uint32_t eip_save;

	POP_DWORD(c, &eip_save);

/*
VOID WINAPI ExitProcess(
  UINT uExitCode
);
*/

	uint32_t exitcode;
	POP_DWORD(c, &exitcode);


	emu_cpu_eip_set(c, eip_save);
	return 0;
}

int32_t	emu_env_w32_hook_ExitThread(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex)
{
	printf("Hook me Captain Cook!\n");
	printf("%s:%i %s\n",__FILE__,__LINE__,__FUNCTION__);

	struct emu_cpu *c = emu_cpu_get(env->emu);

	uint32_t eip_save;

	POP_DWORD(c, &eip_save);

/*
VOID ExitThread(
  DWORD dwExitCode
);
*/

	uint32_t exitcode;
	POP_DWORD(c, &exitcode);


	emu_cpu_eip_set(c, eip_save);
	return 0;
}


int32_t	emu_env_w32_hook_CreateFileA(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex)
{
	printf("Hook me Captain Cook!\n");
	printf("%s:%i %s\n",__FILE__,__LINE__,__FUNCTION__);

	struct emu_cpu *c = emu_cpu_get(env->emu);

	uint32_t eip_save;

	POP_DWORD(c, &eip_save);

/*
HANDLE CreateFile(
  LPCTSTR lpFileName,
  DWORD dwDesiredAccess,
  DWORD dwShareMode,
  LPSECURITY_ATTRIBUTES lpSecurityAttributes,
  DWORD dwCreationDisposition,
  DWORD dwFlagsAndAttributes,
  HANDLE hTemplateFile
);
*/

	uint32_t filename;
	POP_DWORD(c, &filename);

	uint32_t desiredaccess;
	POP_DWORD(c, &desiredaccess);

	uint32_t sharemode;
	POP_DWORD(c, &sharemode);

	uint32_t securityattr;
	POP_DWORD(c, &securityattr);

	uint32_t createdisp;
	POP_DWORD(c, &createdisp);

	uint32_t flagsandattr;
	POP_DWORD(c, &flagsandattr);

	uint32_t templatefile;
	POP_DWORD(c, &templatefile);

	emu_cpu_eip_set(c, eip_save);
	return 0;
}


int32_t	emu_env_w32_hook_CloseHandle(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex)
{
	printf("Hook me Captain Cook!\n");
	printf("%s:%i %s\n",__FILE__,__LINE__,__FUNCTION__);

	struct emu_cpu *c = emu_cpu_get(env->emu);

	uint32_t eip_save;

	POP_DWORD(c, &eip_save);

/*
BOOL CloseHandle(
  HANDLE hObject
);
);
*/

	uint32_t object;
	POP_DWORD(c, &object);

	emu_cpu_eip_set(c, eip_save);
	return 0;
}

int32_t	emu_env_w32_hook_WriteFile(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex)
{
	printf("Hook me Captain Cook!\n");
	printf("%s:%i %s\n",__FILE__,__LINE__,__FUNCTION__);

	struct emu_cpu *c = emu_cpu_get(env->emu);

	uint32_t eip_save;

	POP_DWORD(c, &eip_save);

/*
BOOL WriteFile(
  HANDLE hFile,
  LPCVOID lpBuffer,
  DWORD nNumberOfBytesToWrite,
  LPDWORD lpNumberOfBytesWritten,
  LPOVERLAPPED lpOverlapped
);
*/
	uint32_t file;
	POP_DWORD(c, &file);

	uint32_t buffer;
	POP_DWORD(c,  &buffer);

	uint32_t bytestowrite;
	POP_DWORD(c,  &bytestowrite);

	uint32_t byteswritten;
	POP_DWORD(c,  &byteswritten);

	uint32_t overlapped;
	POP_DWORD(c,  &overlapped);

	emu_memory_write_dword(emu_memory_get(env->emu), byteswritten, bytestowrite);

	emu_cpu_reg32_set(c, eax, 32);

	emu_cpu_eip_set(c, eip_save);
	return 0;
}




int32_t	emu_env_w32_hook_DeleteFileA(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex)
{
	printf("Hook me Captain Cook!\n");
	printf("%s:%i %s\n",__FILE__,__LINE__,__FUNCTION__);

	struct emu_cpu *c = emu_cpu_get(env->emu);

	uint32_t eip_save;

	POP_DWORD(c, &eip_save);

/*
BOOL DeleteFile(
  LPCTSTR lpFileName
);

*/
	uint32_t filename;
	POP_DWORD(c, &filename);

	emu_cpu_eip_set(c, eip_save);
	return 0;
}

int32_t	emu_env_w32_hook__lcreat(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex)
{
	printf("Hook me Captain Cook!\n");
	printf("%s:%i %s\n",__FILE__,__LINE__,__FUNCTION__);

	struct emu_cpu *c = emu_cpu_get(env->emu);

	uint32_t eip_save;

	POP_DWORD(c, &eip_save);

/*
LONG _lcreat(
  LPCSTR lpszFileName,
  int fnAttribute
);
*/
	uint32_t filename;
	POP_DWORD(c, &filename);

	uint32_t attr;
	POP_DWORD(c, &attr);

	emu_cpu_reg32_set(c, eax, 4711); // filehandle

	emu_cpu_eip_set(c, eip_save);
	return 0;
}


int32_t	emu_env_w32_hook__hwrite(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex)
{
	return emu_env_w32_hook__lwrite(env, ex);
}


int32_t	emu_env_w32_hook__lwrite(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex)
{
	printf("Hook me Captain Cook!\n");
	printf("%s:%i %s\n",__FILE__,__LINE__,__FUNCTION__);

	struct emu_cpu *c = emu_cpu_get(env->emu);

	uint32_t eip_save;

	POP_DWORD(c, &eip_save);

/*
LONG _lwrite(
  HFile hFile,
  LPCSTR lpBuffer,
  UINT cbWrite
);
*/
	uint32_t file;
	POP_DWORD(c, &file);

	uint32_t buffer;
	POP_DWORD(c, &buffer);

	uint32_t size;
	POP_DWORD(c, &size);

	emu_cpu_reg32_set(c, eax, size);

	emu_cpu_eip_set(c, eip_save);
	return 0;
}

int32_t	emu_env_w32_hook__lclose(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex)
{
	printf("Hook me Captain Cook!\n");
	printf("%s:%i %s\n",__FILE__,__LINE__,__FUNCTION__);

	struct emu_cpu *c = emu_cpu_get(env->emu);

	uint32_t eip_save;

	POP_DWORD(c, &eip_save);

/*
FIXME
*/
	uint32_t file;
	POP_DWORD(c, &file);

	emu_cpu_eip_set(c, eip_save);
	return 0;
}

#include <errno.h>

int32_t	emu_env_w32_hook_malloc(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex)
{
	printf("Hook me Captain Cook!\n");
	printf("%s:%i %s\n",__FILE__,__LINE__,__FUNCTION__);

	struct emu_cpu *c = emu_cpu_get(env->emu);

	uint32_t eip_save;

	POP_DWORD(c, &eip_save);

/*
void *malloc( 
   size_t size 
);
*/

	uint32_t size;
	POP_DWORD(c, &size);
	PUSH_DWORD(c, size);

	printf("malloc %i bytes\n", size);

	uint32_t addr;
	if (emu_memory_alloc(c->mem, &addr, size) == -1)
		emu_cpu_reg32_set(c, eax, 0);
	else
		emu_cpu_reg32_set(c, eax, addr);

	
    emu_cpu_eip_set(c, eip_save);
	return 0;
}



int32_t	emu_env_w32_hook_memset(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex)
{
	printf("Hook me Captain Cook!\n");
	printf("%s:%i %s\n",__FILE__,__LINE__,__FUNCTION__);

	struct emu_cpu *c = emu_cpu_get(env->emu);

	uint32_t eip_save;

	POP_DWORD(c, &eip_save);

/*
void *memset(
   void* dest, 
   int c, 
   size_t count 
);
*/
	uint32_t dest;
	POP_DWORD(c, &dest);

	uint32_t writeme;
	POP_DWORD(c, &writeme);

	uint32_t size;
	POP_DWORD(c, &size);

	PUSH_DWORD(c, size);
	PUSH_DWORD(c, writeme);
	PUSH_DWORD(c, dest);


	printf("memset(0x%08x, 0x%08x, %i)\n", dest, writeme, size);
	
	emu_cpu_reg32_set(c, eax, dest);

    emu_cpu_eip_set(c, eip_save);
	return 0;
}


int32_t	emu_env_w32_hook_fopen(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex)
{
	printf("Hook me Captain Cook!\n");
	printf("%s:%i %s\n",__FILE__,__LINE__,__FUNCTION__);

	struct emu_cpu *c = emu_cpu_get(env->emu);

	uint32_t eip_save;

	POP_DWORD(c, &eip_save);

/*
FILE *fopen( const char *filename, const char *mode );
FILE *_wfopen( const wchar_t *filename, const wchar_t *mode );
*/
	uint32_t p_filename;
	MEM_DWORD_READ(c, c->reg[esp], &p_filename);


	uint32_t p_mode;
	MEM_DWORD_READ(c, c->reg[esp]-4, &p_mode);

	struct emu_string *mode = emu_string_new();
	emu_memory_read_string(c->mem, p_mode, mode, 512);

	struct emu_string *filename = emu_string_new();
	emu_memory_read_string(c->mem, p_filename, filename, 512);

	printf("fopen(%s, %s)\n", emu_string_char(filename), (char *)mode->data);
	
	emu_cpu_reg32_set(c, eax, 0x8888);

    emu_cpu_eip_set(c, eip_save);
	return 0;
}

int32_t	emu_env_w32_hook_fwrite(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex)
{
	printf("Hook me Captain Cook!\n");
	printf("%s:%i %s\n",__FILE__,__LINE__,__FUNCTION__);

	struct emu_cpu *c = emu_cpu_get(env->emu);

	uint32_t eip_save;

	POP_DWORD(c, &eip_save);

/*
size_t fwrite( const void *buffer, size_t size, size_t count, FILE *stream );
*/
	uint32_t p_buffer;
	MEM_DWORD_READ(c, c->reg[esp], &p_buffer);


	uint32_t size;
	MEM_DWORD_READ(c, (c->reg[esp]-4), &size);

	uint32_t count;
	MEM_DWORD_READ(c, (c->reg[esp]-8), &count);

	uint32_t p_stream;
	MEM_DWORD_READ(c, c->reg[esp]-12, &p_stream);


	printf("fwrite(0x%08x, %d, %d, 0x%08x)\n", p_buffer, size, count, p_stream);
	
	emu_cpu_reg32_set(c, eax, size*count);

    emu_cpu_eip_set(c, eip_save);
	return 0;
}


int32_t	emu_env_w32_hook_fclose(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex)
{
	printf("Hook me Captain Cook!\n");
	printf("%s:%i %s\n",__FILE__,__LINE__,__FUNCTION__);

	struct emu_cpu *c = emu_cpu_get(env->emu);

	uint32_t eip_save;

	POP_DWORD(c, &eip_save);

/*
int _fcloseall( void );
int fclose( FILE *stream );
*/
	uint32_t p_stream;
	MEM_DWORD_READ(c, c->reg[esp]-8, &p_stream);


	printf("fclose(0x%08x)\n", p_stream);
	
	emu_cpu_reg32_set(c, eax, 0);

    emu_cpu_eip_set(c, eip_save);
	return 0;
}


int32_t	emu_env_w32_hook_URLDownloadToFileA(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex)
{
	printf("Hook me Captain Cook!\n");
	printf("%s:%i %s\n",__FILE__,__LINE__,__FUNCTION__);

	struct emu_cpu *c = emu_cpu_get(env->emu);

	uint32_t eip_save;

	POP_DWORD(c, &eip_save);

/*
HRESULT URLDownloadToFile(
  LPUNKNOWN pCaller,
  LPCTSTR szURL,
  LPCTSTR szFileName,
  DWORD dwReserved,
  LPBINDSTATUSCALLBACK lpfnCB
);
*/
	uint32_t p_caller;
	POP_DWORD(c, &p_caller);

	uint32_t p_url;
	POP_DWORD(c, &p_url);

	uint32_t p_filename;
	POP_DWORD(c, &p_filename);

	uint32_t reserved;
	POP_DWORD(c, &reserved);

	uint32_t statuscallbackfn;
	POP_DWORD(c, &statuscallbackfn);



	struct emu_string *url = emu_string_new();
	emu_memory_read_string(c->mem, p_url, url, 512);

	struct emu_string *filename = emu_string_new();
	emu_memory_read_string(c->mem, p_filename, filename, 512);


	printf(" %s -> %s\n", emu_string_char(url), emu_string_char(filename));

	emu_string_free(url);
	emu_string_free(filename);

    emu_cpu_eip_set(c, eip_save);
	return 0;
}



int32_t emu_env_w32_hook_GetSystemDirectoryA(struct emu_env_w32 *env, struct emu_env_w32_dll_export *ex)
{
	printf("Hook me Captain Cook!\n");
	printf("%s:%i %s\n",__FILE__,__LINE__,__FUNCTION__);

	struct emu_cpu *c = emu_cpu_get(env->emu);

	uint32_t eip_save;

	POP_DWORD(c, &eip_save);

/*
UINT GetSystemDirectory(
  LPTSTR lpBuffer,
  UINT uSize
);
*/
	uint32_t p_buffer;
	POP_DWORD(c, &p_buffer);

	uint32_t size;
	POP_DWORD(c, &size);

	emu_memory_write_block(emu_memory_get(env->emu), p_buffer, "c:\\WINDOWS\\system32\x00", 20);
	emu_cpu_reg32_set(c, eax, 19);

	emu_cpu_eip_set(c, eip_save);
	return 0;
}

