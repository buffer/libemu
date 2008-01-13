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


#include "emu/emu_list.h"
#include "emu/emu_stack.h"

#ifndef HAVE_EMU_PROFILE_H
#define HAVE_EMU_PROFILE_H


enum emu_profile_argument_render
{
	render_none,
	render_ptr,
	render_int,
	render_struct,
	render_string,
	render_ip,
	render_port
};



header_list_typedefs(emu_profile_argument_root,emu_profile_argument,emu_profile_argument_link);

struct emu_profile_argument
{
	enum emu_profile_argument_render render;

	char *argname;
	char *argtype;

	union
	{
		int tint;
		char *tchar;
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
};
header_list_functions(emu_profile_functions,emu_profile_function_root, emu_profile_function, link);


struct emu_profile
{

	emu_profile_function_root *functions;
	struct emu_stack *argument_stack;
	struct emu_profile_argument *last_ref;
};


struct emu_profile *emu_profile_new();
void emu_profile_free(struct emu_profile *profile);


struct emu_profile_function *emu_profile_function_new();
void emu_profile_function_free(struct emu_profile_function *function);


struct emu_profile_argument *emu_profile_argument_new(enum emu_profile_argument_render render, const char *type, const char *name);
void emu_profile_argument_free(struct emu_profile_argument *argument);



void emu_profile_debug(struct emu_profile *profile);
void emu_profile_function_debug(struct emu_profile_function *function);

void emu_profile_argument_add_none(struct emu_profile *profile);
void emu_profile_argument_add_int(struct emu_profile *profile, char *argtype, char *argname, int value);
void emu_profile_argument_add_string(struct emu_profile *profile, char *argtype,  char *argname, char *value);
void emu_profile_argument_add_ptr(struct emu_profile *profile,	char *argtype,  char *argname, uint32_t value);
void emu_profile_argument_add_ip(struct emu_profile *profile, char *argtype,  char *argname, uint32_t value);
void emu_profile_argument_add_port(struct emu_profile *profile,	char *argtype,  char *argname, uint32_t value);

void emu_profile_function_add(struct emu_profile *profile, char *fnname);
void emu_profile_argument_struct_start(struct emu_profile* profile, const char* structtype, const char* structname);
void emu_profile_argument_struct_end(struct emu_profile *profile);


#endif
