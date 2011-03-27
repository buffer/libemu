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

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "../../../config.h"
#include "emu/emu.h"
#include "emu/emu_log.h"
#include "emu/emu_memory.h"
#include "emu/emu_cpu.h"
#include "emu/emu_cpu_data.h"
#include "emu/emu_cpu_stack.h"
#include "emu/emu_hashtable.h"
#include "emu/emu_string.h"
#include "emu/environment/emu_env.h"
#include "emu/environment/emu_profile.h"
#include "emu/environment/win32/emu_env_w32.h"
#include "emu/environment/win32/emu_env_w32_dll.h"
#include "emu/environment/win32/emu_env_w32_dll_export.h"
#include "emu/environment/win32/env_w32_dll_export_msvcrt_hooks.h"

int32_t	env_w32_hook__execv(struct emu_env *env, struct emu_env_hook *hook)
{
	logDebug(env->emu, "Hook me Captain Cook!\n");
	logDebug(env->emu, "%s:%i %s\n",__FILE__,__LINE__,__FUNCTION__);

	struct emu_cpu *c = emu_cpu_get(env->emu);

	uint32_t eip_save;

	POP_DWORD(c, &eip_save);

/*
intptr_t _execv( 
   const char *cmdname,
   const char *const *argv 
);
intptr_t _wexecv( 
   const wchar_t *cmdname,
   const wchar_t *const *argv 
);
*/

	uint32_t p_cmdname;
	POP_DWORD(c, &p_cmdname);

	struct emu_string *cmdname = emu_string_new();
	emu_memory_read_string(c->mem, p_cmdname, cmdname, 512);



	uint32_t p_argv;
	POP_DWORD(c, &p_argv);


	if ( env->profile != NULL )
	{
		emu_profile_function_add(env->profile, "_execv");
		emu_profile_argument_add_ptr(env->profile, "const char *", "cmdname", p_cmdname);
		emu_profile_argument_add_string(env->profile, "", "", emu_string_char(cmdname)); 
		emu_profile_argument_add_ptr(env->profile, "const char *const *", "argv", p_argv);
		emu_profile_argument_add_none(env->profile);
		emu_profile_function_returnvalue_int_set(env->profile, "int ", 0);
	}

	emu_string_free(cmdname);
    emu_cpu_eip_set(c, eip_save);
	return 0;
}


int32_t	env_w32_hook_fclose(struct emu_env *env, struct emu_env_hook *hook)
{
	logDebug(env->emu, "Hook me Captain Cook!\n");
	logDebug(env->emu, "%s:%i %s\n",__FILE__,__LINE__,__FUNCTION__);

	struct emu_cpu *c = emu_cpu_get(env->emu);

	uint32_t eip_save;

	POP_DWORD(c, &eip_save);

/*
int _fcloseall( void );
int fclose( FILE *stream );
*/

	uint32_t p_stream;
	MEM_DWORD_READ(c, c->reg[esp], &p_stream);



	logDebug(env->emu, "fclose(0x%08x)\n", p_stream);
	
	emu_cpu_reg32_set(c, eax, 0);

	if (env->profile != NULL)
	{
		emu_profile_function_add(env->profile, "fclose");
		emu_profile_argument_add_ptr(env->profile, "FILE *", "stream", p_stream);
		emu_profile_argument_add_none(env->profile);
		emu_profile_function_returnvalue_int_set(env->profile, "int", 0);
	}

    emu_cpu_eip_set(c, eip_save);
	return 0;
}


int32_t	env_w32_hook_fopen(struct emu_env *env, struct emu_env_hook *hook)
{
	logDebug(env->emu, "Hook me Captain Cook!\n");
	logDebug(env->emu, "%s:%i %s\n",__FILE__,__LINE__,__FUNCTION__);

	struct emu_cpu *c = emu_cpu_get(env->emu);

	uint32_t eip_save;

	POP_DWORD(c, &eip_save);

/*
FILE *fopen( const char *filename, const char *mode );
FILE *_wfopen( const wchar_t *filename, const wchar_t *mode );
*/

	uint32_t p_filename;
	MEM_DWORD_READ(c, c->reg[esp], &p_filename);

	struct emu_string *filename = emu_string_new();
	emu_memory_read_string(c->mem, p_filename, filename, 512);
	
	uint32_t p_mode;
	MEM_DWORD_READ(c, c->reg[esp]+4, &p_mode);
	struct emu_string *mode = emu_string_new();
	emu_memory_read_string(c->mem, p_mode, mode, 512);



//	printf("fopen(%s, %s)\n", emu_string_char(filename), (char *)mode->data);
	

	uint32_t returnvalue;
	if ( hook->hook.win->userhook != NULL )
	{
		returnvalue = hook->hook.win->userhook(env, hook, 
											   emu_string_char(filename),
											   emu_string_char(mode));
	}else
	{
		returnvalue	= 0x89898989;
	}

	emu_cpu_reg32_set(c, eax, returnvalue);

	if (env->profile != NULL)
	{
		emu_profile_function_add(env->profile, "fopen");
		emu_profile_argument_add_ptr(env->profile, "const char *", "filename", p_filename);
		emu_profile_argument_add_string(env->profile, "", "", emu_string_char(filename));
		emu_profile_argument_add_ptr(env->profile, "const char *", "mode", p_mode);
		emu_profile_argument_add_string(env->profile,  "", "", emu_string_char(mode));
		emu_profile_function_returnvalue_ptr_set(env->profile, "FILE *", returnvalue);
		emu_profile_argument_add_none(env->profile);
	}



	emu_string_free(filename);
	emu_string_free(mode);

    emu_cpu_eip_set(c, eip_save);
	return 0;
}

int32_t	env_w32_hook_fwrite(struct emu_env *env, struct emu_env_hook *hook)
{
	logDebug(env->emu, "Hook me Captain Cook!\n");
	logDebug(env->emu, "%s:%i %s\n",__FILE__,__LINE__,__FUNCTION__);

	struct emu_cpu *c = emu_cpu_get(env->emu);

	uint32_t eip_save;

	POP_DWORD(c, &eip_save);

/*
size_t fwrite( const void *buffer, size_t size, size_t count, FILE *stream );
*/

	uint32_t p_buffer;
	MEM_DWORD_READ(c, c->reg[esp], &p_buffer);
	

    uint32_t size;
	MEM_DWORD_READ(c, (c->reg[esp]+4), &size);



	uint32_t count;
	MEM_DWORD_READ(c, (c->reg[esp]+8), &count);

	unsigned char *buffer = malloc(size*count);
	emu_memory_read_block(emu_memory_get(env->emu), p_buffer, buffer, size*count);



	uint32_t p_stream;
	MEM_DWORD_READ(c, c->reg[esp]+12, &p_stream);
	

	uint32_t returnvalue;
	if ( hook->hook.win->userhook != NULL )
	{
		returnvalue = hook->hook.win->userhook(env, hook, 
											   buffer,
											   size,
											   count,
											   p_stream);
	}else
	{
		returnvalue	= size*count;
	}


	logDebug(env->emu, "fwrite(0x%08x, %d, %d, 0x%08x)\n", p_buffer, size, count, p_stream);
	
	emu_cpu_reg32_set(c, eax, returnvalue);

	if ( env->profile != NULL )
	{
		emu_profile_function_add(env->profile, "fwrite");
		emu_profile_function_returnvalue_int_set(env->profile, "size_t", returnvalue);
		emu_profile_argument_add_ptr(env->profile, "const void *", "buffer", p_buffer);
		emu_profile_argument_add_bytea(env->profile, "", "", buffer, size*count);
		emu_profile_argument_add_int(env->profile, "size_t", "size", size);
		emu_profile_argument_add_int(env->profile, "count_t", "count", count);
		emu_profile_argument_add_ptr(env->profile, "FILE *", "stream", p_stream);
		emu_profile_argument_add_none(env->profile);
	}

	free(buffer);

    emu_cpu_eip_set(c, eip_save);
	return 0;
}

