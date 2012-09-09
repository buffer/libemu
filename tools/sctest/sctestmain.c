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


#include "../config.h"

#define HAVE_GETOPT_H
#ifdef HAVE_GETOPT_H
# include <getopt.h>
#endif

#ifdef HAVE_STDLIB_H
# include <stdlib.h>
#endif


#include <stdint.h>

#define HAVE_UNISTD
#ifdef HAVE_UNISTD
# include <unistd.h>
#endif
#include <stdio.h>

#include <stdarg.h>


#include <errno.h>

#ifdef HAVE_LIBCARGOS
#include <cargos-lib.h>
#endif


#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <sys/select.h>

#include <sys/wait.h>


#include "emu/emu.h"
#include "emu/emu_memory.h"
#include "emu/emu_cpu.h"
#include "emu/emu_log.h"
#include "emu/emu_cpu_data.h"
#include "emu/emu_cpu_stack.h"
#include "emu/environment/emu_profile.h"
#include "emu/environment/emu_env.h"
#include "emu/environment/win32/emu_env_w32.h"
#include "emu/environment/win32/emu_env_w32_dll.h"
#include "emu/environment/win32/emu_env_w32_dll_export.h"
#include "emu/environment/win32/env_w32_dll_export_kernel32_hooks.h"
#include "emu/environment/linux/emu_env_linux.h"
#include "emu/emu_getpc.h"
#include "emu/emu_graph.h"
#include "emu/emu_string.h"
#include "emu/emu_hashtable.h"

#include "emu/emu_shellcode.h"


#define CODE_OFFSET 0x417000

#define FAILED "\033[31;1mfailed\033[0m"
#define SUCCESS "\033[32;1msuccess\033[0m"

#define F(x) (1 << (x))


#include "userhooks.h"
#include "options.h"
#include "dot.h"
#include "tests.h"
#include "nanny.h"


struct run_time_options opts;

/*
static const char *regm[] = {
	"eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi"
};


// 0     1     2     3      4       5       6     7 
static const char *flags[] = { "CF", "  ", "PF", "  " , "AF"  , "    ", "ZF", "SF", 
	"TF", "IF", "DF", "OF" , "IOPL", "IOPL", "NT", "  ",
	"RF", "VM", "AC", "VIF", "RIP" , "ID"  , "  ", "  ",
	"  ", "  ", "  ", "   ", "    ", "    ", "  ", "  "};
*/







int graph_draw(struct emu_graph *graph);

int test(struct emu *e)
{
//	int i=0;
	struct emu_cpu *cpu = emu_cpu_get(e);
	struct emu_memory *mem = emu_memory_get(e);

	struct emu_env *env = emu_env_new(e);
	env->profile = emu_profile_new();

	struct nanny *na = nanny_new();

	/* IAT for sqlslammer */
	emu_memory_write_dword(mem, 0x42AE1018, 0x7c801D77);
	emu_memory_write_dword(mem, 0x42ae1010, 0x7c80ADA0);
	emu_memory_write_dword(mem, 0x7c80ADA0, 0x51EC8B55);
	emu_memory_write_byte(mem,  0x7c814eeb, 0xc3);

	/* Export Address Table Access Filtering */
	emu_memory_write_dword(mem, 0x7c80ffec, 0xc330408b);

	if ( env == NULL )
	{
		printf("%s \n", emu_strerror(e));
		printf("%s \n", strerror(emu_errno(e)));
		return -1;
	}


	emu_env_w32_export_hook(env, "ExitProcess", user_hook_ExitProcess, NULL);
	emu_env_w32_export_hook(env, "ExitThread", user_hook_ExitThread, NULL);

	emu_env_w32_load_dll(env->env.win,"shdocvw.dll");
	emu_env_w32_export_hook(env, "IEWinMain", user_hook_IEWinMain, NULL);

	if ( opts.interactive == true )
	{

		emu_env_w32_load_dll(env->env.win,"msvcrt.dll");
		emu_env_w32_export_hook(env, "fclose", user_hook_fclose, na);
		emu_env_w32_export_hook(env, "fopen", user_hook_fopen, na);
		emu_env_w32_export_hook(env, "fwrite", user_hook_fwrite, na);

		emu_env_w32_export_hook(env, "CreateProcessA", user_hook_CreateProcess, NULL);
		emu_env_w32_export_hook(env, "WaitForSingleObject", user_hook_WaitForSingleObject, NULL);
		emu_env_w32_export_hook(env, "CreateFileA", user_hook_CreateFile, na);
		emu_env_w32_export_hook(env, "WriteFile", user_hook_WriteFile, na);
		emu_env_w32_export_hook(env, "CloseHandle", user_hook_CloseHandle, na);


		emu_env_w32_load_dll(env->env.win,"ws2_32.dll");
		emu_env_w32_export_hook(env, "accept", user_hook_accept, NULL);
		emu_env_w32_export_hook(env, "bind", user_hook_bind, NULL);
		emu_env_w32_export_hook(env, "closesocket", user_hook_closesocket, NULL);
		emu_env_w32_export_hook(env, "connect", user_hook_connect, NULL);

		emu_env_w32_export_hook(env, "listen", user_hook_listen, NULL);
		emu_env_w32_export_hook(env, "recv", user_hook_recv, NULL);
		emu_env_w32_export_hook(env, "send", user_hook_send, NULL);
		emu_env_w32_export_hook(env, "socket", user_hook_socket, NULL);
		emu_env_w32_export_hook(env, "WSASocketA", user_hook_WSASocket, NULL);

		emu_env_w32_load_dll(env->env.win,"urlmon.dll");
		emu_env_w32_export_hook(env, "URLDownloadToFileA", user_hook_URLDownloadToFile, NULL);


		emu_env_linux_syscall_hook(env, "exit", user_hook_exit, NULL);
		emu_env_linux_syscall_hook(env, "socket", user_hook_socket, NULL);
	}

/*	uint32_t x;
	for (x=0x7c800000;x<0x7c902400;x++)
	{
		uint8_t b;
		emu_memory_read_byte(mem,x,&b);
		printf("%02x ",b);
		if (x % 16 == 0)
		{
			printf("\n");
		}
	}
	return 0;
*/


	int j=0;


	/* run the code */
	if( opts.verbose != 0 )
		emu_cpu_debugflag_set(cpu, instruction_string);

	if ( opts.verbose >= 2 )
	{
		emu_log_level_set(emu_logging_get(e),EMU_LOG_DEBUG);
		emu_cpu_debug_print(cpu);
		emu_log_level_set(emu_logging_get(e),EMU_LOG_NONE);
	}


	struct emu_vertex *last_vertex = NULL;
	struct emu_graph *graph = NULL;
	struct emu_hashtable *eh = NULL;
	struct emu_hashtable_item *ehi = NULL;

	if ( opts.graphfile != NULL )
	{
		graph = emu_graph_new();
		eh = emu_hashtable_new(2047, emu_hashtable_ptr_hash, emu_hashtable_ptr_cmp);
	}


	int ret; //= emu_cpu_run(emu_cpu_get(e));



	uint32_t eipsave = 0;
	for ( j=0;j<opts.steps;j++ )
	{

		if ( opts.verbose > 2 )
		{
			emu_log_level_set(emu_logging_get(e),EMU_LOG_DEBUG);
			emu_cpu_debug_print(cpu);
			emu_log_level_set(emu_logging_get(e),EMU_LOG_NONE);
		}



		if ( cpu->repeat_current_instr == false )
			eipsave = emu_cpu_eip_get(emu_cpu_get(e));

		struct emu_env_hook *hook = NULL;
		struct emu_vertex *ev = NULL;
		struct instr_vertex *iv = NULL;



		ret = 0;

		if ( opts.graphfile != NULL )
		{

			ehi = emu_hashtable_search(eh, (void *)(uintptr_t)eipsave);
			if ( ehi != NULL )
				ev = (struct emu_vertex *)ehi->value;

			if ( ev == NULL )
			{
				ev = emu_vertex_new();
				emu_graph_vertex_add(graph, ev);

				emu_hashtable_insert(eh, (void *)(uintptr_t)eipsave, ev);
			}
		}

		hook = emu_env_w32_eip_check(env);

		if ( hook != NULL )
		{
			 
			if ( opts.graphfile != NULL )
			{
				if ( ev->data != NULL && strcmp(hook->hook.win->fnname, "CreateProcessA") == 0)
				{
					ev = emu_vertex_new();
					emu_graph_vertex_add(graph, ev);
				}

//				fnname_from_profile(env->profile, dllhook->fnname);
				iv = instr_vertex_new(eipsave,hook->hook.win->fnname);
				emu_vertex_data_set(ev, iv);

				// get the dll
				int numdlls=0;
				while ( env->env.win->loaded_dlls[numdlls] != NULL )
				{
					if ( eipsave > env->env.win->loaded_dlls[numdlls]->baseaddr && 
						 eipsave < env->env.win->loaded_dlls[numdlls]->baseaddr + env->env.win->loaded_dlls[numdlls]->imagesize )
					{
						iv->dll = env->env.win->loaded_dlls[numdlls];
					}
					numdlls++;
				}

			}

			if ( hook->hook.win->fnhook == NULL )
			{
				printf("unhooked call to %s\n", hook->hook.win->fnname);
				break;
			}

		}
		else
		{

			ret = emu_cpu_parse(emu_cpu_get(e));

			if ( opts.verbose > 1 )
			{
				emu_log_level_set(emu_logging_get(e),EMU_LOG_DEBUG);
				logDebug(e, "%s\n", cpu->instr_string);
				emu_log_level_set(emu_logging_get(e),EMU_LOG_NONE);
			}

			struct emu_env_hook *hook =NULL;
			if ( ret != -1 )
			{

				if ( ( hook = emu_env_linux_syscall_check(env)) != NULL )
				{
					if ( opts.graphfile != NULL && ev->data == NULL )
					{
						iv = instr_vertex_new(eipsave, hook->hook.lin->name);
						emu_vertex_data_set(ev, iv);
						iv->syscall = hook->hook.lin;
					}
				}
				else
				{

					if ( opts.graphfile != NULL && ev->data == NULL )
					{
						iv = instr_vertex_new(eipsave, emu_cpu_get(e)->instr_string);
						emu_vertex_data_set(ev, iv);
					}
				}
			}
			else
			{
				if ( opts.graphfile != NULL && ev->data == NULL )
				{
					iv = instr_vertex_new(eipsave, "ERROR");
					emu_vertex_data_set(ev, iv);
				}
			}

			if ( ret != -1 )
			{
				if ( hook == NULL )
				{
					if ( opts.verbose == 1 )
					{
						emu_log_level_set(emu_logging_get(e),EMU_LOG_INFO);
						ret = emu_cpu_step(emu_cpu_get(e));
						emu_log_level_set(emu_logging_get(e),EMU_LOG_NONE);
					}else
					if ( opts.verbose >= 2 )
					{
						emu_log_level_set(emu_logging_get(e),EMU_LOG_DEBUG);
						ret = emu_cpu_step(emu_cpu_get(e));
						emu_log_level_set(emu_logging_get(e),EMU_LOG_NONE);
					}
					else
					{
						ret = emu_cpu_step(emu_cpu_get(e));
					}
				}
				else
				{
					if ( hook->hook.lin->fnhook != NULL )
						hook->hook.lin->fnhook(env, hook);
					else
						break;
				}
			}

			if ( ret == -1 )
			{
				/* step failed - maybe SEH */
				if( emu_env_w32_step_failed(env) != 0 )
				{
					printf("cpu error %s\n", emu_strerror(e));
					break;
				}
			}else
				env->env.win->last_good_eip = emu_cpu_eip_get(emu_cpu_get(e)); //used in case of seh exception
		}
		if ( opts.graphfile != NULL )
		{
			if ( last_vertex != NULL )
			{
				struct emu_edge *ee = emu_vertex_edge_add(last_vertex, ev);
				struct emu_cpu *cpu = emu_cpu_get(e);
				if ( cpu->instr.is_fpu == 0 && cpu->instr.source.cond_pos == eipsave && cpu->instr.source.has_cond_pos == 1 )
					ee->data = (void *)0x1;
			}

			last_vertex = ev;
		}

//			printf("\n");
	}

	printf("stepcount %i\n",j);


	if ( opts.graphfile != NULL )
	{
		graph_draw(graph);
	}
	if ( opts.verbose >= 2 )
	{
		emu_log_level_set(emu_logging_get(e),EMU_LOG_DEBUG);
		emu_cpu_debug_print(cpu);
		emu_log_level_set(emu_logging_get(e),EMU_LOG_NONE);
	}


	if ( opts.verbose >= 1 )
	{
		emu_profile_debug(env->profile);
	}

	if (opts.profile_file)
		emu_profile_dump(env->profile, opts.profile_file);

	if (eh != NULL)
		emu_hashtable_free(eh);

	if (graph != NULL)
		emu_graph_free(graph);

	emu_env_free(env);
	return 0;
}




int getpctest(void)
{
	struct emu *e = emu_new();

	if ( opts.verbose > 1 )
	{
		emu_cpu_debugflag_set(emu_cpu_get(e), instruction_string);
		emu_log_level_set(emu_logging_get(e),EMU_LOG_DEBUG);
	}

	if ( (opts.offset = emu_shellcode_test(e, (uint8_t *)opts.scode, opts.size)) >= 0 )
		printf("%s offset = 0x%08x\n",SUCCESS, opts.offset);
	else
		printf(FAILED"\n");

	emu_free(e);

	return 0;
}


void dump(int n)
{
	if ( n >= numtests() || n < 0 )
		return;

	int i;
	for ( i=0; i<tests[n].codesize;i++ )
		printf("%c", tests[n].code[i]);
}

void cleanup(void)
{
	return;

	int i;
	for ( i=0;i<numtests();i++ )
		if ( tests[i].code != NULL )
			free(tests[i].code);

}

void list_tests(void)
{
	int i;
	for ( i=0;i<numtests();i++ )
		printf("%-2i) %s\n", i, tests[i].instr);
}


int prepare_from_stdin_read(void)
{
	unsigned buffer[BUFSIZ];
	int ret, eof=0;
	int16_t bytes_read=0;
	uint32_t len=0;
	fd_set read_fds;
	struct timeval st;

	while ( !eof )
	{
		FD_ZERO(&read_fds);
		FD_SET(STDIN_FILENO, &read_fds);

		st.tv_sec  = 10;
		st.tv_usec = 0;

		switch ( ret = select(FD_SETSIZE, &read_fds, NULL, NULL, &st) )
		{
		case -1:
			fprintf(stderr, "Error with select(): %s.\n", strerror(errno));
			exit(1);
		case  0:
			break;
		default:
			if ( FD_ISSET(STDIN_FILENO, &read_fds) )
			{
				if ( (bytes_read = read(STDIN_FILENO, buffer, BUFSIZ)) <= 0 )
				{
					if ( bytes_read == 0 ) eof = 1;
					else
					{
						fprintf(stderr, "Error while reading data: %s.\n", strerror(errno));
						exit(1);
					}
				}
				if ( !eof )
				{
					if ( (opts.scode = (unsigned char *) realloc(opts.scode, len+bytes_read)) == NULL )
					{
						fprintf(stderr, "Error while allocating memory: %s.\n", strerror(errno));
						exit(1);
					}
					memcpy(opts.scode+len, buffer, bytes_read);
					len += bytes_read;
				}
			}
		}
	}
	opts.size = len;

	return 0;
}


int prepare_from_stdin_write(struct emu *e)
{
	/* set the registers to the initial values */
	struct emu_cpu *cpu = emu_cpu_get(e);
	struct emu_memory *mem = emu_memory_get(e);

	int j;
	for ( j=0;j<8;j++ )
	{
		emu_cpu_reg32_set(cpu,j , 0);
	}

	emu_memory_write_dword(mem, 0xef787c3c,  4711);
//	emu_memory_write_dword(mem, 0x0,  4711);
	emu_memory_write_dword(mem, 0x00416f9a,  4711);
	emu_memory_write_dword(mem, 0x0044fcf7, 4711);
	emu_memory_write_dword(mem, 0x00001265, 4711);
	emu_memory_write_dword(mem, 0x00002583, 4711);
	emu_memory_write_dword(mem, 0x00e000de, 4711);
	emu_memory_write_dword(mem, 0x01001265, 4711);
	emu_memory_write_dword(mem, 0x8a000066, 4711);

	/* set the flags */
	emu_cpu_eflags_set(cpu, 0);


	/* write the code to the offset */
	int static_offset = CODE_OFFSET;
	emu_memory_write_block(mem, static_offset, opts.scode,  opts.size);



	/* set eip to the code */
	emu_cpu_eip_set(emu_cpu_get(e), static_offset + opts.offset);

	emu_memory_write_block(mem, 0x0012fe98, opts.scode,  opts.size);
	emu_cpu_reg32_set(emu_cpu_get(e), esp, CODE_OFFSET-50); //0x0012fe98);

	emu_memory_write_dword(mem, 0x7df7b0bb, 0x00000000); //UrldownloadToFile
//	free(opts.scode);

	return 0;
}

int prepare_from_stdin(struct emu *e)
{
	if (opts.size == 0)
		prepare_from_stdin_read();

	prepare_from_stdin_write(e);


	return 0;

}



int prepare_testnumber(struct emu *e)
{
	struct emu_cpu *cpu = emu_cpu_get(e);
	struct emu_memory *mem = emu_memory_get(e);
	int i;
	int n = opts.testnumber;
	for ( i=0; i < numtests();i++ )
	{
		if ( n != -1 && i != n )
			continue;

		printf("testing (#%d) '%s' \t", i, tests[i].instr);

		int j=0;

		/* set the registers to the initial values */
		for ( j=0;j<8;j++ )
		{
			emu_cpu_reg32_set(cpu,j ,tests[i].in_state.reg[j]);
		}


		/* set the flags */
		emu_cpu_eflags_set(cpu,tests[i].in_state.eflags);


		/* write the code to the offset */
		int static_offset = CODE_OFFSET;
		emu_memory_write_block(mem, static_offset, tests[i].code,  tests[i].codesize);



		/* set eip to the code */
		emu_cpu_eip_set(emu_cpu_get(e), static_offset + opts.offset);

		if ( opts.scode == 0 )
		{
			opts.scode = malloc(tests[i].codesize);
			memcpy(opts.scode, tests[i].code, tests[i].codesize);
			opts.size = tests[i].codesize;
		}
		return 0;
	}
	return -1;

}

int prepare_argos(struct emu *e)
{
#ifdef HAVE_LIBCARGOS
	cargos_lib_t *calib;

	if (!(calib = cargos_lib_create()))
	{
		perror("Could not allocate log instance");
		return -1;
	}

	if (cargos_lib_csi_open(calib, opts.from_argos_csi) != 0)
	{
		fprintf(stderr, "%s\n", cargos_lib_error(calib));
		return -1;
	}

	struct emu_memory *mem = emu_memory_get(e);
	struct emu_cpu *cpu = emu_cpu_get(e);

	emu_log_level_set(emu_logging_get(e),EMU_LOG_DEBUG);
	cargos_lib_mb_t *mb;
	int i=0;

	cargos_lib_csi_mbfirst(calib);
	while((mb = cargos_lib_csi_mbnext(calib)) != NULL)
//	for (mb = calib->csi->mblist.lh_first; mb != NULL; mb = mb->blocks.le_next)
	{
		if (mb == NULL)
			break;
/*
		printf("%-3i %08x %08x %-4i \n",
			   i,
			   cargos_lib_mb_addr(mb, CARGOS_LIB_PHYS).val32, 
			   cargos_lib_mb_addr(mb, CARGOS_LIB_VIRT).val32, 
			   cargos_lib_mb_dsize(mb));
*/
		int datasize = cargos_lib_mb_dsize(mb);
		void *data = malloc(datasize);
		cargos_lib_mb_data(mb, data, datasize);
		emu_memory_write_block(mem, 
							   cargos_lib_mb_addr(mb, CARGOS_LIB_VIRT).val32,
							   data,
							   datasize);
		free(data);
		i++;
	}

	
	emu_cpu_reg32_set(cpu, eax, cargos_lib_csi_regv(calib, CARGOS_LIB_EAX).val32);
	emu_cpu_reg32_set(cpu, ecx, cargos_lib_csi_regv(calib, CARGOS_LIB_ECX).val32);
	emu_cpu_reg32_set(cpu, edx, cargos_lib_csi_regv(calib, CARGOS_LIB_EDX).val32);
	emu_cpu_reg32_set(cpu, ebx, cargos_lib_csi_regv(calib, CARGOS_LIB_EBX).val32);
	emu_cpu_reg32_set(cpu, esp, cargos_lib_csi_regv(calib, CARGOS_LIB_ESP).val32);
	emu_cpu_reg32_set(cpu, ebp, cargos_lib_csi_regv(calib, CARGOS_LIB_EBP).val32);
	emu_cpu_reg32_set(cpu, esi, cargos_lib_csi_regv(calib, CARGOS_LIB_ESI).val32);
	emu_cpu_reg32_set(cpu, edi, cargos_lib_csi_regv(calib, CARGOS_LIB_EDI).val32);

	emu_cpu_eip_set(cpu, cargos_lib_csi_regv(calib, CARGOS_LIB_EIP).val32);
	return 0;
#else
	printf("compiled without support for argos csi (libcargos)\n");
	return -1;
#endif
	
}

int prepare(struct emu *emu)
{
	if (opts.from_stdin)
		return prepare_from_stdin(emu);

	if (opts.from_argos_csi)
		return prepare_argos(emu);

	if (opts.testnumber >= 0)
		return prepare_testnumber(emu);

	return -1;
}


void print_help(void)
{
	struct help_info 
	{
		const char *short_param;
		const char *long_param;
		const char *args;
		const char *description;
	};

	struct help_info help_infos[] =
	{
		{"a", "argos-csi"   , "PATH"    , "use this argos csi files as input"},
		{"b", "bind"        , "IP:PORT" , "bind this ip:port"},
		{"c", "connect"     , "IP:PORT" , "redirect connects to this ip:port"},
		{"C", "cmd"         , "CMD"     , "command to execute for \"cmd\" in shellcode (default: cmd=\"/bin/sh -c \\\"cd ~/.wine/drive_c/; wine 'c:\\windows\\system32\\cmd_orig.exe' \\\"\")"},
		{"d", "dump"        , "INTEGER" , "dump the shellcode (binary) to stdout"},
		{"g", "getpc"       , NULL      , "run getpc mode, try to detect a shellcode"},
		{"G", "graph"       , "FILEPATH", "save a dot formatted callgraph in filepath"},
		{"h", "help"        , NULL      , "show this help"},
		{"i", "interactive" , NULL      , "proxy api calls to the host operating system"},
		{"l", "listtests"   , NULL      , "list all tests"},
		{"o", "offset"      , "[INT|HEX]", "manual offset for shellcode, accepts int and hexvalues"},
		{"p", "profile"     , "PATH"    , "write shellcode profile to this file"},
		{"S", "stdin"       , NULL      , "read shellcode/buffer from stdin, works with -g"},
		{"s", "steps"       , "INTEGER" , "max number of steps to run"},
		{"t", "testnumber"  , "INTEGER" , "the test to run"},
		{"v", "verbose"     , NULL              , "be verbose, can be used multiple times, f.e. -vv"},
	};

	int i;
	for (i=0;i<sizeof(help_infos)/sizeof(struct help_info); i++)
	{
		printf("\t-%1s ", help_infos[i].short_param);
		if (help_infos[i].args != NULL)
			printf("%-7s ", help_infos[i].args);
		else
			printf("%-7s ", "");
		printf("\t\t%s\n", help_infos[i].description);

		printf("\t--%s", help_infos[i].long_param);
		if (help_infos[i].args != NULL)
			printf("=%7s ", help_infos[i].args);
		printf("\n\n");
	}
}


int main(int argc, char *argv[])
{
	memset(&opts,0,sizeof(struct run_time_options));

	opts.steps = 1;
	opts.testnumber = -1;
	opts.offset = 0;

	opts.override.commands.commands = emu_hashtable_new(16, emu_hashtable_string_hash, emu_hashtable_string_cmp);

	while ( 1 )
	{
		int c;
		int option_index = 0;
		static struct option long_options[] = {
			{"argos-csi"        , 1, 0, 'a'},
			{"bind"             , 1, 0, 'b'},
			{"connect"          , 1, 0, 'c'},
			{"cmd"              , 1, 0, 'C'},
			{"dump"             , 1, 0, 'd'},
			{"getpc"            , 0, 0, 'g'},
			{"graph"            , 1, 0, 'G'},
			{"help"             , 0, 0, 'h'},
			{"interactive"      , 0, 0, 'i'},
			{"listtests"        , 0, 0, 'l'},
			{"offset"           , 1, 0, 'o'},
			{"profile"          , 1, 0, 'p'},
			{"steps"            , 1, 0, 's'},
			{"stdin"            , 0, 0, 'S'},
			{"testnumber"       , 1, 0, 't'},
			{"verbose"          , 0, 0, 'v'},
			{0, 0, 0, 0}
		};

		c = getopt_long (argc, argv, "a:b:c:C:d:gG:hilo:p:s:St:v", long_options, &option_index);
		if ( c == -1 )
			break;

		switch ( c )
		{

		case 'a':
			opts.from_argos_csi = strdup(optarg);
			printf("argos-csi %s\n", opts.from_argos_csi);
			break;

		case 'b':
			{
				opts.override.bind.host = strdup(optarg);
				char *port;
				if (( port = strstr(opts.override.bind.host, ":")) != NULL)
				{
					*port = '\0';
					port++;
					opts.override.bind.port = atoi(port);

					if (*opts.override.bind.host == '\0')
					{
						free(opts.override.bind.host);
						opts.override.bind.host = NULL;
					}
				}

				printf("override bind %s:%i\n", opts.override.bind.host, opts.override.bind.port);
			}
			break;

		case 'c':
			{
				opts.override.connect.host = strdup(optarg);
				char *port;
				if (( port = strstr(opts.override.connect.host, ":")) != NULL)
				{
					*port = '\0';
					port++;
					opts.override.connect.port = atoi(port);

					if (*opts.override.connect.host == '\0')
					{
						free(opts.override.connect.host);
						opts.override.connect.host = NULL;
					}

				}

				printf("override connect %s:%i\n", opts.override.connect.host, opts.override.connect.port);
			}
			break;

		case 'C':
			{
				char *cmd = strdup(optarg);

				char *value = strstr(cmd, "=");
				*value = '\0';
				value++;
				printf("command for %s is %s\n", cmd, value );
				emu_hashtable_insert(opts.override.commands.commands, cmd, value);
			}
			break;

		case 'd':
			dump(atoi(optarg));
			return 0;
			break;

		case 'g':
			opts.getpc = 1;
			break;

		case 'G':
			opts.graphfile = strdup(optarg);
			printf("graph file %s\n", opts.graphfile);
			break;

		case 'h':
			print_help();
			exit(0);
			break;

		case 'i':
			opts.interactive = true;
			break;

		case 'l':
			list_tests();
			return 0;
			break;

		case 'o':
			if (strncasecmp(optarg, "0x", 2) == 0)
				opts.offset = strtol(optarg+2, NULL, 16); // hex vvalue
			else
				opts.offset = strtol(optarg, NULL, 10);	  // decimal vvalue
			printf("offset %i (0x%x)\n", opts.offset, (unsigned int)opts.offset);
			break;


		case 'p':
			opts.profile_file = strdup(optarg);
			printf("profile %s\n", opts.profile_file);
			break;

		case 's':
			opts.steps = atoi(optarg);
			break;


		case 'S':
			opts.from_stdin = true;
			break;

		case 't':
			opts.testnumber = atoi(optarg);
			break;

		case 'v':
			opts.verbose++;
			break;






		default:
			printf ("?? getopt returned character code 0%o ??\n", c);
			break;

		}
	}
	printf("verbose = %i\n", opts.verbose);

	struct emu *e = emu_new();
	if ( prepare(e) == 0 )
	{
		if (opts.getpc == 1)
		{
			getpctest();

			emu_free(e);
			e = emu_new();
			prepare(e);
		}

		test(e);
	}

	emu_free(e);

//	dump_export_table();
	if (opts.from_argos_csi)
		free(opts.from_argos_csi);

	if (opts.graphfile)
		free(opts.graphfile);


	if (opts.profile_file)
		free(opts.profile_file);

	if (opts.scode)
		free(opts.scode);

	return 0;
}


