/********************************************************************************
 *                               libemu
 *
 *                    - x86 shellcode emulation -
 *
 *
 * Copyright (C) 2008  Paul Baecher & Markus Koetter
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

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "emu/emu_list.h"
#include "emu/emu_stack.h"

#ifndef HAVE_EMU_PROFILE_H
#define HAVE_EMU_PROFILE_H


enum emu_profile_argument_render
{
	render_none,
	render_ptr,
	render_int,
	render_short,
	render_struct,
	render_string,
	render_bytea,
	render_ip,
	render_port,
	render_array
};



header_list_typedefs(emu_profile_argument_root,emu_profile_argument,emu_profile_argument_link);

struct emu_profile_argument
{
	enum emu_profile_argument_render render;

	char *argname;
	char *argtype;

	union
	{
		int32_t tint;
		int16_t tshort;

		char *tchar;

		struct 
		{
			unsigned char *data;
			uint32_t size;
		} bytea;

		struct 
		{
			emu_profile_argument_root *arguments;
		} tstruct;

		struct
		{
			struct emu_profile_argument *ptr;
			uint32_t addr;
		}tptr;
	} value;

	emu_profile_argument_link link;
};

header_list_functions(emu_profile_arguments,emu_profile_argument_root, emu_profile_argument, link);

header_list_typedefs(emu_profile_function_root,emu_profile_function,emu_profile_function_link);
struct emu_profile_function
{
	enum emu_profile_argument_render retval;
	char *fnname;
	emu_profile_argument_root *arguments;

	emu_profile_function_link link;
	struct emu_profile_argument *return_value;
};
header_list_functions(emu_profile_functions,emu_profile_function_root, emu_profile_function, link);


struct emu_profile
{

	emu_profile_function_root *functions;
	struct emu_stack *argument_stack;
	struct emu_profile_argument *last_ref;
};


struct emu_profile *emu_profile_new(void);
void emu_profile_free(struct emu_profile *profile);


struct emu_profile_function *emu_profile_function_new(void);
void emu_profile_function_free(struct emu_profile_function *function);


struct emu_profile_argument *emu_profile_argument_new(enum emu_profile_argument_render render, const char *type, const char *name);
void emu_profile_argument_free(struct emu_profile_argument *argument);



void emu_profile_debug(struct emu_profile *profile);

int emu_profile_dump(struct emu_profile *profile, const char *path);
int emu_profile_parse(struct emu_profile *profile, const char *path);

void emu_profile_function_debug(struct emu_profile_function *function);

void emu_profile_argument_debug(struct emu_profile_argument *argument, int indent);

void emu_profile_argument_add_none(struct emu_profile *profile);
void emu_profile_argument_add_int(struct emu_profile *profile, char *argtype, char *argname, int32_t value);
void emu_profile_argument_add_short(struct emu_profile *profile, char *argtype, char *argname, int16_t value);
void emu_profile_argument_add_string(struct emu_profile *profile, char *argtype,  char *argname, char *value);
void emu_profile_argument_add_ptr(struct emu_profile *profile,	char *argtype,  char *argname, uint32_t value);
void emu_profile_argument_add_ip(struct emu_profile *profile, char *argtype,  char *argname, uint32_t value);
void emu_profile_argument_add_port(struct emu_profile *profile,	char *argtype,  char *argname, uint32_t value);
void emu_profile_argument_add_bytea(struct emu_profile *profile, char *argtype, char *argname, unsigned char *data, uint32_t size);
void emu_profile_argument_array_start(struct emu_profile* profile, const char* arraytype, const char* arrayname);
void emu_profile_argument_array_end(struct emu_profile *profile);


void emu_profile_function_add(struct emu_profile *profile, char *fnname);
void emu_profile_argument_struct_start(struct emu_profile* profile, const char* structtype, const char* structname);
void emu_profile_argument_struct_end(struct emu_profile *profile);

void emu_profile_function_returnvalue_int_set(struct emu_profile *profile, const char *type, int value);
void emu_profile_function_returnvalue_ptr_set(struct emu_profile *profile, const char *type, int value);


void *emu_profile_function_argument_get(struct emu_profile_function *function, int argc);

void emu_profile_argument_add_sockaddr_ptr(struct emu_profile *profile, const char *name, uint32_t ptr, struct sockaddr sa);

#endif                                                                                              

