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
#include <errno.h>

#include "emu/emu.h"
#include "emu/emu_cpu.h"
#include "emu/emu_memory.h"
#include "emu/emu_hashtable.h"
#include "emu/environment/emu_env.h"
#include "emu/environment/emu_profile.h"
#include "emu/environment/win32/emu_env_w32.h"
#include "emu/environment/win32/emu_env_w32_dll.h"
#include "emu/environment/win32/emu_env_w32_dll_export.h"
#include "emu/environment/win32/env_w32_dll_export_hooks.h"

#include "emu/emu_log.h"

extern const char kernel32_dll_7c800000[];
extern const char kernel32_dll_7c801000[];
extern const char ws2_32_71a10000[];
extern const char ws2_32_71a11000[];
extern const char msvcrt_77be0000[];
extern const char msvcrt_77C28970[];
extern const char urlmon_7DF20000[];
extern const char urlmon_7DF21000[];

// added -dzzie 
extern const char user32_7E410000[];  /* pe header and section table */
extern const char user32_7e413900[];  /* export table */
extern const char shell32_7C9C0000[]; /* pe header and section table */
extern const char shell32_7c9e7d50[]; /* export table */
extern const char wininet_3D930000[]; /* pe header and section table 2-2-11*/
extern const char wininet_3d931844[]; /* export table */
extern const char ntdll_7C900000[]; /* pe header and section table 2-5-11*/
extern const char ntdll_7c903400[]; /* export table */
extern const char shlwapi_77F60000[]; /* pe header and section table 3-10-11*/
extern const char shlwapi_77f61820[]; /* export table */
extern const char advapi32_77DD0000[];
extern const char advapi32_77dd16A4[];
extern const char shdocvw_7E290000[];
extern const char shdocvw_7E2A4480[];


struct emu_env_w32_known_dll_segment kernel32_segments[] = 
{
	{
		.address = 0x7c800000,
		.segment = kernel32_dll_7c800000,
		.segment_size = 641,
	},
	{
		.address = 0x7c801000,
		.segment = kernel32_dll_7c801000, /* - 7C808F71 */
		.segment_size = 32625,
	},
	{ 0, NULL, 0 }
};

struct emu_env_w32_known_dll_segment ws2_32_segments[] = 
{
	{
		.address = 0x71a10000,
		.segment = ws2_32_71a10000,
		.segment_size = 786,
	},
	{
		.address = 0x71a11000,
		.segment = ws2_32_71a11000,
		.segment_size = 5634,
	},
	{ 0, NULL, 0 }
};

struct emu_env_w32_known_dll_segment msvcrt_segments[] = 
{
	{
		.address = 0x77be0000,
		.segment = msvcrt_77be0000,
		.segment_size = 5634,
	},
	{
		.address = 0x77C28970,
		.segment = msvcrt_77C28970,
		.segment_size = 17328,
	},
	{ 0, NULL, 0 }
};

struct emu_env_w32_known_dll_segment urlmon_segments[] = 
{
	{
		.address = 0x7DF20000,
		.segment = urlmon_7DF20000,
		.segment_size = 786,
	},
	{
		.address = 0x7DF21000,
		.segment = urlmon_7DF21000,
		.segment_size = 6144,
	},
	{ 0, NULL, 0 }
};

//dzzie 1-26-11
struct emu_env_w32_known_dll_segment user32_segments[] = 
{
	{
		.address = 0x7E410000,
		.segment = user32_7E410000,
		.segment_size = 0x2cf,
	},
	{
		.address = 0x7e413900,
		.segment = user32_7e413900,
		.segment_size = 0x4baf,
	},
	{ 0, NULL, 0 }
};

//dzzie 1-26-11
struct emu_env_w32_known_dll_segment shell32_segments[] = 
{
	{
		.address = 0x7C9C0000,
		.segment = shell32_7C9C0000,
		.segment_size = 0x28f,
	},
	{
		.address = 0x7c9e7d50,
		.segment = shell32_7c9e7d50,
		.segment_size = 0x291f,
	},
	{ 0, NULL, 0 }
};

//dzzie 2-2-11
struct emu_env_w32_known_dll_segment wininet_segments[] = 
{
	{
		.address = 0x3D930000,
		.segment = wininet_3D930000,
		.segment_size = 0x2Af,
	},
	{
		.address = 0x3d931844,
		.segment = wininet_3d931844,
		.segment_size = 0x1d4f,
	},
	{ 0, NULL, 0 }
};

struct emu_env_w32_known_dll_segment ntdll_segments[] = 
{
	{
		.address = 0x7C900000,
		.segment = ntdll_7C900000,
		.segment_size = 0x27f,
	},
	{
		.address = 0x7c903400,
		.segment = ntdll_7c903400,
		.segment_size = 0x9a5f,
	},
	{ 0, NULL, 0 }
};

struct emu_env_w32_known_dll_segment shlwapi_segments[] = 
{
	{
		.address = 0x77F60000,
		.segment = shlwapi_77F60000,
		.segment_size = 0x39f,
	},
	{
		.address = 0x77f61820,
		.segment = shlwapi_77f61820,
		.segment_size = 0x27ff,
	},
	{ 0, NULL, 0 }
};

struct emu_env_w32_known_dll_segment advapi32_segments[] = 
{
	{
		.address = 0x77DD0000,
		.segment = advapi32_77DD0000,
		.segment_size = 0x31f,
	},
	{
		.address = 0x77dd16A4,
		.segment = advapi32_77dd16A4,
		.segment_size = 0x5253,
	},
	{ 0, NULL, 0 }
};

struct emu_env_w32_known_dll_segment shdocvw_segments[] = 
{
	{
		.address = 0x7E290000,
		.segment = shdocvw_7E290000,
		.segment_size = 0x36f,
	},
	{
		.address = 0x7E2A4480,
		.segment = shdocvw_7E2A4480,
		.segment_size = 0x4e0,
	},
	{ 0, NULL, 0 }
};


struct emu_env_w32_known_dll known_dlls[] = 
{
	{ /* dummy entry for the PEB/LDR lists 
	   * shares base address with kernel32
	   * it should be possible to have this working without such
	   * hacks, but ... for now it works
	   */
		.dllname = "self",
		.baseaddress = 0x7C800000,
		.imagesize = 0xf6000, //0x00106000, -changed dz.. 
	},
	{
		.dllname = "kernel32",      //this must always be first dll -dz
		.baseaddress = 0x7C800000, 
		.imagesize = 0xf6000, //0x00106000, //xpsp2 is only f600 and still same base..this way no conflict with ntdll..not sure where org sz came from.. dz
		.exports = kernel32_exports, /* last export = lstrlenW - > 7c809a09 so were good */
		.memory_segments = kernel32_segments,
	}, /* org end at 7C906000 - modded now ends at 7C80f6000 so no collision with ntdll..) */
	{
		.dllname = "ntdll", /* dzzie 2-5-11 */
		.baseaddress = 0x7C900000, 
		.imagesize = 0xB2000,
		.exports = ntdll_exports,
		.memory_segments = ntdll_segments,
	},  
	{
		.dllname = "ws2_32",
		.baseaddress = 0x71A10000,
		.imagesize = 0x17000,
		.exports = ws2_32_exports,
		.memory_segments = ws2_32_segments,
	},
	{
		.dllname = "msvcrt",
		.baseaddress = 0x77BE0000,
		.imagesize = 0x58000,
		.exports = msvcrt_exports,
		.memory_segments = msvcrt_segments,
	},
	{
		.dllname = "shell32", /* dzzie 1-26-11 */
		.baseaddress = 0x7C9C0000,
		.imagesize = 0x817000,
		.exports = shell32_exports,
		.memory_segments = shell32_segments,
	}, /*ends at 7D1D7000 */
	{
		.dllname = "shdocvw", /* dzzie 3.12.11 */
		.baseaddress = 0x7E290000,
		.imagesize = 0x171000,
		.exports = shdocvw_exports,
		.memory_segments = shdocvw_segments,
	}, /*ends at 7E401000 */
	{
		.dllname = "advapi32",
		.baseaddress = 0x77DD0000,
		.imagesize = 0x9B000,
		.exports = advapi32_exports,
		.memory_segments = advapi32_segments,
	}, /*ends at  77E6B000 export table data ends at 77DD68F6. 3.12.11*/
	{
		.dllname = "shlwapi", /* dzzie 3-10-11 */
		.baseaddress = 0x77F60000, 
		.imagesize = 0x76000,
		.exports = shlwapi_exports,
		.memory_segments = shlwapi_segments,
	},  
	{
		.dllname = "urlmon",
		.baseaddress = 0x7DF20000,
		.imagesize = 0xA0000,
		.exports = urlmon_exports,
		.memory_segments = urlmon_segments,
	}, /*ends at 7DFC0000 */
	{
		.dllname = "user32", /* dzzie 1-26-11 */
		.baseaddress = 0x7E410000,
		.imagesize = 0x00091000,
		.exports = user32_exports,
		.memory_segments = user32_segments,
	}, /*ends at 7E4A1000 */
	{
		.dllname = "wininet", /* dzzie 2-2-11 */
		.baseaddress = 0x3D930000,
		.imagesize = 0xD1000,
		.exports = wininet_exports,
		.memory_segments = wininet_segments,
	},  
	{
		.dllname = NULL,
		.baseaddress = 0,
		.imagesize = 0,
		.exports = NULL,
		.memory_segments = NULL,
	},
};


struct emu_env_w32 *emu_env_w32_new(struct emu *e)
{
	struct emu_env_w32 *env = (struct emu_env_w32 *)malloc(sizeof(struct emu_env_w32));
	memset(env,0,sizeof(struct emu_env_w32));
//	env->profile = emu_profile_new();
	env->emu = e;
	// write TEB and linklist

	struct emu_memory *mem = emu_memory_get(e);
	enum emu_segment oldseg = emu_memory_segment_get(mem);


//  0041709D   64:8B43 30       MOV EAX,DWORD PTR FS:[EBX+30]
//	7FFDF030  00 40 FD 7F                                      .@ý
//  7FFDE030  00 F0 FD 7F                                      .ðý


    emu_memory_segment_select(mem,s_fs);
	emu_memory_write_dword(mem,0x30,0x7ffdf000);
	emu_memory_segment_select(mem,oldseg);


//  004170A1   8B40 0C          MOV EAX,DWORD PTR DS:[EAX+C]
//  7FFDF00C  A0 1E 25 00                                       %.
	emu_memory_write_dword(mem,0x7ffdf00c,0x00251ea0);

//  004170A4   8B70 1C          MOV ESI,DWORD PTR DS:[EAX+1C]
//  00251EBC  58 1F 25 00                                      X%.
//	emu_memory_write_dword(mem,0x00251ebc,0x00251f58);


//	004170A7   AD               LODS DWORD PTR DS:[ESI]
//  00251F58  20 20 25 00                                        %.
//	emu_memory_write_dword(mem,0x00251f58,0x00252020);


//	004170A8   8B40 08          MOV EAX,DWORD PTR DS:[EAX+8]             ; kernel32.7C800000
//  00252028  00 00 80 7C     
//	emu_memory_write_dword(mem,0x00252028,0x7C800000);


	// http://www.nirsoft.net/kernel_struct/vista/UNICODE_STRING.html
	typedef struct _UNICODE_STRING
	{
		uint16_t Length;
		uint16_t MaximumLength;
		uint32_t Buffer;
	} UNICODE_STRING, *PUNICODE_STRING;

	// PEB_LDR_DATA Structure
	// http://msdn.microsoft.com/en-us/library/aa813708%28VS.85%29.aspx
	typedef struct _LIST_ENTRY
	{
//		struct _LIST_ENTRY *Flink;
		uint32_t Flink;
		uint32_t Blink;
//		struct _LIST_ENTRY *Blink;
	} LIST_ENTRY, *PLIST_ENTRY; //, *RESTRICTED_POINTER PRLIST_ENTRY;

	typedef uint32_t PVOID;
	typedef unsigned char BYTE;
	typedef uint32_t ULONG;

	typedef struct _LDR_DATA_TABLE_ENTRY
	{
		/* 0x00 */ LIST_ENTRY InLoadOrderLinks;
		/* 0x08 */ LIST_ENTRY InMemoryOrderLinks;
		/* 0x0f */ LIST_ENTRY InInitializationOrderLinks;
		/* 0x18 */ uint32_t DllBase;
		/* 0x1c */ uint32_t EntryPoint;
		/* 0x1f */ uint32_t Reserved3;
		/* 0x24 */ UNICODE_STRING FullDllName;
		/* 0x2c */ UNICODE_STRING BaseDllName;
		/* 0x00 */ uint32_t Reserved5[3];
		union
		{
			ULONG CheckSum;
			PVOID Reserved6;
		};
		uint32_t TimeDateStamp;
	} LDR_DATA_TABLE_ENTRY, *PLDR_DATA_TABLE_ENTRY;


	// http://www.nirsoft.net/kernel_struct/vista/PEB_LDR_DATA.html
	typedef struct _PEB_LDR_DATA
	{
		 /* 0x00 */ uint32_t Length;
		 /* 0x04 */ uint8_t Initialized[4];
		 /* 0x08 */ uint32_t SsHandle;
		 /* 0x0c */ LIST_ENTRY InLoadOrderModuleList;
		 /* 0x14 */ LIST_ENTRY InMemoryOrderModuleList;
		 /* 0x1c */ LIST_ENTRY InInitializationOrderModuleList;
		 /* 0x24 */ uint8_t EntryInProgress;
	} PEB_LDR_DATA, *PPEB_LDR_DATA;

	struct _PEB_LDR_DATA peb_ldr_data;
	peb_ldr_data.InMemoryOrderModuleList.Flink = 0x00251ea0 + 0x1000 + offsetof(struct _LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks);
	peb_ldr_data.InInitializationOrderModuleList.Flink = 0x00251ea0 + 0x1000 + offsetof(struct _LDR_DATA_TABLE_ENTRY, InInitializationOrderLinks);

	emu_memory_write_block(mem, 0x00251ea0, &peb_ldr_data, sizeof(peb_ldr_data));

	uint32_t magic_offset = 0x00251ea0+0x1000;

	struct _LDR_DATA_TABLE_ENTRY tables[16];
	memset(tables, 0, sizeof(tables));

	char names[16][64];
	memset(names, 0, sizeof(names));

	int i;
	for ( i=0; known_dlls[i].dllname != NULL; i++ )
	{
		struct emu_env_w32_known_dll *from = known_dlls+i;
		struct _LDR_DATA_TABLE_ENTRY *to = tables+i;
		
		to->DllBase = from->baseaddress;
		to->BaseDllName.Length = (strlen(from->dllname) + strlen(".dll")) * 2 + 2;
		to->BaseDllName.MaximumLength = to->BaseDllName.Length;// + 2;
		to->BaseDllName.Buffer = magic_offset + sizeof(tables) + i * 64;

		to->InMemoryOrderLinks.Blink = 0xaabbccdd;
		to->InMemoryOrderLinks.Flink = magic_offset + (i+1) * sizeof(struct _LDR_DATA_TABLE_ENTRY) + offsetof(struct _LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks);

		to->InInitializationOrderLinks.Blink = 0xa1b2c3d4;
		to->InInitializationOrderLinks.Flink = magic_offset + (i+1) * sizeof(struct _LDR_DATA_TABLE_ENTRY) + offsetof(struct _LDR_DATA_TABLE_ENTRY, InInitializationOrderLinks);

		int j;		
		for( j=0;j<strlen(from->dllname); j++ )
			names[i][j*2] = from->dllname[j];

		const char *dll = ".dll";
		int k;
		for( k=0;k<strlen(".dll"); k++ )
			names[i][j*2+k*2] = dll[k];
	}
	emu_memory_write_block(mem, magic_offset, tables, sizeof(tables));
	emu_memory_write_block(mem, magic_offset+sizeof(tables), names, sizeof(names));

	// map kernel32.dll to emu's memory at 0x7c800000
	if (emu_env_w32_load_dll(env,"kernel32.dll") == -1 || emu_env_w32_load_dll(env,"ws2_32.dll") == -1 )
    {
		free(env);
		return NULL;
	}


	return env;
}

void emu_env_w32_free(struct emu_env_w32 *env)
{
	int numdlls = 0;
	while (env->loaded_dlls[numdlls] != NULL)
	{
		emu_env_w32_dll_free(env->loaded_dlls[numdlls]);
		numdlls++;
	}
	free(env->loaded_dlls);
//	emu_profile_free(env->profile);
	free(env);

}


int32_t emu_env_w32_load_dll(struct emu_env_w32 *env, char *dllname)
{

	logDebug(env->emu, "trying to load dll %s\n", dllname);

	int i;
	for ( i=1; known_dlls[i].dllname != NULL; i++ )
	{
		
		if ( strncasecmp(dllname, known_dlls[i].dllname, strlen(known_dlls[i].dllname)) == 0 )
		{
			logDebug(env->emu, "loading dll %s\n",dllname);
			struct emu_env_w32_dll *dll = emu_env_w32_dll_new();
			struct emu_memory *mem = emu_memory_get(env->emu);

			dll->dllname = strdup(known_dlls[i].dllname);
			dll->baseaddr = known_dlls[i].baseaddress;
			dll->imagesize = known_dlls[i].imagesize;
			int j;
			for ( j=0; known_dlls[i].memory_segments[j].address != 0; j++ )
			{
				logDebug(env->emu, " 0x%08x %i bytes\n", known_dlls[i].memory_segments[j].address, 
					   known_dlls[i].memory_segments[j].segment_size);
				emu_memory_write_block(mem,
									   known_dlls[i].memory_segments[j].address,
									   (void *)known_dlls[i].memory_segments[j].segment,
									   known_dlls[i].memory_segments[j].segment_size);
			}

			emu_env_w32_dll_exports_copy(dll, known_dlls[i].exports);

			int numdlls=0;
			if ( env->loaded_dlls != NULL )
			{
				while ( env->loaded_dlls[numdlls] != NULL )
					numdlls++;
			}

			env->loaded_dlls = realloc(env->loaded_dlls, sizeof(struct emu_env_w32_dll *) * (numdlls+2));
			env->loaded_dlls[numdlls] = dll;
			env->loaded_dlls[numdlls+1] = NULL;

			return 0;

		}
	}

	return -1;
}


struct emu_env_hook *emu_env_w32_eip_check(struct emu_env *env)
{
	uint32_t eip = emu_cpu_eip_get(emu_cpu_get(env->emu));

	int numdlls=0;
	while ( env->env.win->loaded_dlls[numdlls] != NULL )
	{
/*		printf("0x%08x %s 0x%08x - 0x%08x \n",
			   eip,
			   env->loaded_dlls[numdlls]->dllname,
			   env->loaded_dlls[numdlls]->baseaddr,
			   env->loaded_dlls[numdlls]->baseaddr + env->loaded_dlls[numdlls]->imagesize);
*/
		if ( eip > env->env.win->loaded_dlls[numdlls]->baseaddr && 
			 eip < env->env.win->loaded_dlls[numdlls]->baseaddr + env->env.win->loaded_dlls[numdlls]->imagesize )
		{
			logDebug(env->env.win->emu, "eip %08x is within %s\n",eip, env->env.win->loaded_dlls[numdlls]->dllname);
			struct emu_env_w32_dll *dll = env->env.win->loaded_dlls[numdlls];

			struct emu_hashtable_item *ehi = emu_hashtable_search(dll->exports_by_fnptr, (void *)(uintptr_t)(eip - dll->baseaddr));

			if ( ehi == NULL )
			{
				logDebug(env->emu, "unknown call to %08x\n", eip);
				return NULL;
			}

		
			struct emu_env_hook *hook = (struct emu_env_hook *)ehi->value;

			if ( hook->hook.win->fnhook != NULL )
			{
				hook->hook.win->fnhook(env, hook);
				return hook;
			}
			else
			{
				logDebug(env->emu, "unhooked call to %s\n", hook->hook.win->fnname);
				return hook;
			}
		}
		numdlls++;
	}

	return NULL;
}

int32_t emu_env_w32_export_hook(struct emu_env *env,
								const char *exportname, 
								uint32_t		(*fnhook)(struct emu_env *env, struct emu_env_hook *hook, ...),
								void *userdata)
{
	int numdlls=0;
	while ( env->env.win->loaded_dlls[numdlls] != NULL )
	{
		if (1)//dllname == NULL || strncasecmp(env->loaded_dlls[numdlls]->dllname, dllname, strlen(env->loaded_dlls[numdlls]->dllname)) == 0)
		{
			struct emu_hashtable_item *ehi = emu_hashtable_search(env->env.win->loaded_dlls[numdlls]->exports_by_fnname, (void *)exportname);
			if (ehi != NULL)
			{
#if 0
				printf("hooked %s\n",  exportname);
#endif
				struct emu_env_hook *hook = (struct emu_env_hook *)ehi->value;
				hook->hook.win->userhook = fnhook;
				hook->hook.win->userdata = userdata;
				return 0;
			}
		}
		numdlls++;
	}
#if 0
	printf("hooking %s failed\n", exportname);
#endif
	return -1;
}

int32_t emu_env_w32_step_failed(struct emu_env *env)
{
	uint32_t FS_SEGMENT_DEFAULT_OFFSET = 0x7ffdf000;
	struct emu *e = env->emu;
	int i=0;
	int regs[8];
	uint32_t seh = 0;
	uint32_t seh_handler = 0;
	const int default_handler = 0x7c800abc;
	struct emu_memory *m = emu_memory_get(e);

	//lets check and see if an exception handler has been set
	if( emu_memory_read_dword( m, FS_SEGMENT_DEFAULT_OFFSET, &seh) == -1 )
		return -1;
	if( emu_memory_read_dword( m, seh+4, &seh_handler) == -1 )
		return -1;
	if( seh_handler == 0 )
		return -1; //better to check to see if code section huh?
	if( seh_handler == default_handler )
		return -1;	//not a real one dummy we put in place..

	if( seh_handler == env->env.win->lastExceptionHandler )
	{
		env->env.win->exception_count++; //really here is where we should walk the chain...
		if( env->env.win->exception_count >= 2 )
			return -1;
		//if(seh == 0xFFFFFFFF) return -1;
	} else
	{
		env->env.win->exception_count = 0;
		env->env.win->lastExceptionHandler = seh_handler;
	}

	printf("\n%x\tException caught SEH=0x%x (seh foffset:%x)\n", env->env.win->last_good_eip, seh_handler, seh_handler);// - CODE_OFFSET);

	//now take our saved esp, add two ints to stack (subtract 8) and set org esp pointer there.
	uint32_t cur_esp = emu_cpu_reg32_get(emu_cpu_get(e), esp);
	uint32_t new_esp = cur_esp - 8;

	printf("\tcur_esp=%x new_esp=%x\n\n",cur_esp,new_esp);

	emu_cpu_eip_set(emu_cpu_get(e), seh_handler);

	regs[eax] = 0;
	regs[ebx] = 0;
	regs[esi] = 0;
	regs[edi] = 0;
	regs[ecx] = seh_handler;
	regs[edx] = 0xDEADBEEF;	//unsure what this is was some ntdll addr 0x7C9032BC
	regs[esp] = new_esp;

	//update the registers with our new values
	for( i=0;i<8;i++ )
		emu_cpu_reg32_set( emu_cpu_get(e), i , regs[i]);

	uint32_t write_at  = new_esp + 8;
	emu_memory_write_dword(m, write_at, cur_esp); //write saved esp to stack

	return 0; //dont break in final error test..give it a chance...to work in next step
}

#include "dlls/kernel32dll.c"
#include "dlls/ws2_32dll.c"
#include "dlls/msvcrtdll.c"
#include "dlls/urlmondll.c"
#include "dlls/advapi32dll.c"
#include "dlls/user32dll.c"
#include "dlls/shell32dll.c"
#include "dlls/wininetdll.c"
#include "dlls/shdocvwdll.c"
#include "dlls/ntdll.c"
#include "dlls/shlwapidll.c"

