/* @header@ */
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "emu/emu.h"
#include "emu/emu.h"
#include "emu/emu_memory.h"
#include "emu/enviroment/win32/emu_env_w32.h"
#include "emu/enviroment/win32/emu_env_w32_dll.h"



struct emu_env_w32 *emu_env_w32_new(struct emu *e)
{
	struct emu_env_w32 *env = (struct emu_env_w32 *)malloc(sizeof(struct emu_env_w32));
	memset(env,0,sizeof(struct emu_env_w32));
	env->emu = e;
	// write TEB and linklist

	struct emu_memory *mem = emu_memory_get(e);
	enum emu_segment oldseg = emu_memory_segment_get(mem);


//  0041709D   64:8B43 30       MOV EAX,DWORD PTR FS:[EBX+30]
//	7FFDF030  00 40 FD 7F                                      .@ý
    emu_memory_segment_select(mem,s_fs);
	emu_memory_write_dword(mem,0x30,0x7ffd4000);
	emu_memory_segment_select(mem,oldseg);


//  004170A1   8B40 0C          MOV EAX,DWORD PTR DS:[EAX+C]
//  7FFD400C  A0 1E 25 00                                       %.
	emu_memory_write_dword(mem,0x7ffd400c,0x00251ea0);


//  004170A4   8B70 1C          MOV ESI,DWORD PTR DS:[EAX+1C]
//  00251EBC  58 1F 25 00                                      X%.
	emu_memory_write_dword(mem,0x00251ebc,0x00251f58);


//	004170A7   AD               LODS DWORD PTR DS:[ESI]
//  00251F58  20 20 25 00                                        %.
	emu_memory_write_dword(mem,0x00251f58,0x00252020);


//	004170A8   8B40 08          MOV EAX,DWORD PTR DS:[EAX+8]
//  00252028  00 00 80 7C                                      ..€|
	emu_memory_write_dword(mem,0x00252028,0x7C800000);

	// map kernel32.dll to emu's memory at 0x7c800000
	if (emu_env_w32_load_dll(env,"kernel32.dll") == -1 )
    {
		free(env);
		return NULL;
	}

	return env;
}

void emu_env_w32_free(struct emu_env_w32 *env)
{
	// FIXME free the dlls
	free(env);
}

uint32_t emu_env_w32_load_dll(struct emu_env_w32 *env, char *dllname)
{
	printf("loading dll %s\n",dllname);
	// FIXME
	// map the dlls export table to emu's memory
	// calc the exports addresses
	// for each export, get the dll a dll_export
	// if we have a prepared callback for a export (LoadLibraryA GetProcAddress f.e.)
	// set the callback
	// add the dll to our dlls

	FILE *f = fopen(dllname, "r");
	if ( f == NULL )
	{
		emu_errno_set(env->emu,errno);
		emu_strerror_set(env->emu, "Could not fopen %s (%s)\n",dllname,strerror(errno));
		return -1;
	}
	int size;
	if (fseek(f,0,SEEK_END) < 0)
	{
		emu_errno_set(env->emu,errno);
		emu_strerror_set(env->emu, "Could not fseek %s (%s)\n",dllname,strerror(errno));
		return -1;
	}

	size = ftell(f);

	char *image = (char *)malloc(size);
	fread(image,1,size,f);
	fclose(f);

	struct emu_env_w32_dll *dll = emu_env_w32_dll_new();

	dll->dllname = strdup(dllname);

	// get the exports table
	// get the fnptr table

	// add the exports to our dll

	// if we find the end of the exports table
	// we don't have to map the damn whole dll



	

	struct dll_base_addr
	{
		char *name;
		uint32_t baseaddr;
	};

	static struct dll_base_addr base_addresses[]=
	{
		{"kernel32.dll",0x7c800000},
		{0,0}
	};

	int i=0;
	uint32_t base_addr = 0;

	while (base_addresses[i].name != NULL)
	{
		if (strcmp(base_addresses[i].name,dllname) == 0)
		{
			printf("base address for dll %s is 0x%08x\n",
				   base_addresses[i].name,
				   base_addresses[i].baseaddr);

			base_addr = base_addresses[i].baseaddr;

			break;
		}
		i++;
	}

	if (base_addr != 0)
	{
		printf("wrote %s (%i bytes) to 0x%08x\n",dllname,size,base_addr);
		emu_memory_write_block(emu_memory_get(env->emu),base_addr,image,size);
	}else
	{
		printf("no base addr for dll, won't work yet\n");
	}


	return 0;
}



