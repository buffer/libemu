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

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>


#include "emu/environment/emu_profile.h"


source_list_functions(emu_profile_functions,emu_profile_function_root, emu_profile_function, link);

source_list_functions(emu_profile_arguments,emu_profile_argument_root, emu_profile_argument, link);

struct emu_profile *emu_profile_new()
{
	struct emu_profile *profile = malloc(sizeof(struct emu_profile));
	memset(profile, 0, sizeof(struct emu_profile));

	profile->functions = emu_profile_functions_create();
	profile->argument_stack = emu_stack_new();
	return profile;
}

void emu_profile_free(struct emu_profile *profile)
{
	struct emu_profile_function *function;
	while ((function = emu_profile_functions_remove_first(profile->functions)) != NULL)
	{
		emu_profile_function_free(function);
	}	
	emu_profile_functions_destroy(profile->functions);
	emu_stack_free(profile->argument_stack);
	free(profile);
}

void emu_profile_function_add(struct emu_profile *profile, char *fnname)
{
	struct emu_profile_function *function = emu_profile_function_new();

	function->retval = render_struct;
	function->fnname = strdup(fnname);

	emu_profile_functions_insert_last(profile->functions, function);
}

void emu_profile_argument_add(struct emu_profile *profile, struct emu_profile_argument *argument)
{
	if (profile->last_ref == NULL)
	{
		emu_profile_argument_root *arg_root;
		if (emu_stack_empty(profile->argument_stack))
		{
			struct emu_profile_function *function = emu_profile_functions_last(profile->functions);
			arg_root = function->arguments;
		}else
		{
			struct emu_profile_argument *stack_front = emu_stack_front(profile->argument_stack);
			arg_root = stack_front->value.tstruct.arguments;
		}
		emu_profile_arguments_insert_last(arg_root, argument);
	}else
	{
		profile->last_ref->value.tptr.ptr = argument;
		profile->last_ref = NULL;
	}
}


void emu_profile_argument_struct_start(struct emu_profile* profile, const char* structtype, const char* structname)
{
//	printf("%s %s\n", __PRETTY_FUNCTION__,  structname);
	struct emu_profile_argument *argument = emu_profile_argument_new(render_struct, structtype, structname);
	emu_profile_argument_add(profile, argument);
	emu_stack_push(profile->argument_stack, argument);
}


void emu_profile_argument_struct_end(struct emu_profile *profile)
{
//	printf("%s %s\n", __PRETTY_FUNCTION__);
	emu_stack_pop(profile->argument_stack);
}

void emu_profile_argument_array_start(struct emu_profile* profile, const char* arraytype, const char* arrayname)
{
	struct emu_profile_argument *argument = emu_profile_argument_new(render_array, arraytype, arrayname);
	emu_profile_argument_add(profile, argument);
	emu_stack_push(profile->argument_stack, argument);
}

void emu_profile_argument_array_end(struct emu_profile *profile)
{
	emu_stack_pop(profile->argument_stack);
}


void emu_profile_argument_add_none(struct emu_profile *profile)
{
	struct emu_profile_argument *argument = emu_profile_argument_new(render_none, "", "");
	emu_profile_argument_add(profile, argument);
}

void emu_profile_argument_add_int(struct emu_profile *profile, char *argtype, char *argname, int value)
{

    struct emu_profile_argument *argument = emu_profile_argument_new(render_int, argtype, argname);
	argument->value.tint = value;
	emu_profile_argument_add(profile, argument);
}

void emu_profile_argument_add_string(struct emu_profile *profile, char *argtype, char *argname, char *value)
{

    struct emu_profile_argument *argument = emu_profile_argument_new(render_string, argtype, argname);
	argument->value.tchar = strdup(value);
	emu_profile_argument_add(profile, argument);
}

void emu_profile_argument_add_ptr(struct emu_profile *profile, char *argtype,  char *argname, uint32_t value)
{
	struct emu_profile_argument *argument = emu_profile_argument_new(render_ptr, argtype, argname);
	argument->value.tptr.addr = value;
	emu_profile_argument_add(profile, argument);
	profile->last_ref = argument;
}

void emu_profile_argument_add_ip(struct emu_profile *profile, char *argtype,  char *argname, uint32_t value)
{
	struct emu_profile_argument *argument = emu_profile_argument_new(render_ip, argtype, argname);
	argument->value.tint = value;
	emu_profile_argument_add(profile, argument);
}

void emu_profile_argument_add_port(struct emu_profile *profile,	char *argtype,  char *argname, uint32_t value)
{
	struct emu_profile_argument *argument = emu_profile_argument_new(render_port, argtype, argname);
	argument->value.tint = value;
	emu_profile_argument_add(profile, argument);
}


struct emu_profile_function *emu_profile_function_new()
{
	struct emu_profile_function *function = malloc(sizeof(struct emu_profile_function));
	memset(function, 0, sizeof(struct emu_profile_function));

	function->arguments = emu_profile_arguments_create();
	emu_profile_functions_init_link(function);
	return function;
}

void emu_profile_function_free(struct emu_profile_function *function)
{

	struct emu_profile_argument *argument;
	while ((argument = emu_profile_arguments_remove_first(function->arguments)) != NULL )
	{
		emu_profile_argument_free(argument);
	}

	if (function->fnname != NULL)
		free(function->fnname);

	emu_profile_arguments_destroy(function->arguments);
	free(function);
}



struct emu_profile_argument *emu_profile_argument_new(enum emu_profile_argument_render render, const char *type, const char *name)
{
	struct emu_profile_argument *argument = malloc(sizeof(struct emu_profile_argument));
	memset(argument, 0, sizeof(struct emu_profile_argument));

	emu_profile_arguments_init_link(argument);
	if ( render != render_none )
	{
		argument->argname = strdup(name);
		argument->argtype = strdup(type);
	}
	argument->render = render;

	if (render == render_struct || render == render_array)
	{
		argument->value.tstruct.arguments = emu_profile_arguments_create();
	}

	return argument;
}

void emu_profile_argument_free(struct emu_profile_argument *argument)
{
	if (argument->argname != NULL)
		free(argument->argname);

	if (argument->argtype != NULL)
		free(argument->argtype);

	switch(argument->render)
	{
	case render_port:
	case render_ip:
	case render_none:
	case render_int:
		break;

	case render_string:
		if (argument->value.tchar != NULL)
			free(argument->value.tchar);
		break;

	case render_ptr:
		emu_profile_argument_free(argument->value.tptr.ptr);
		break;

	case render_array:
	case render_struct:
		{
			struct emu_profile_argument *argumentit;
			while ((argumentit = emu_profile_arguments_remove_first(argument->value.tstruct.arguments)) != NULL )
			{
				emu_profile_argument_free(argumentit);
			}

			emu_profile_arguments_destroy(argument->value.tstruct.arguments);
		}
		break;
	}

	free(argument);
}

char *indents(int i)
{
	static char indents[255];
	memset(indents, ' ', 255);
	indents[i*4] = '\0';
	return indents;
}


void emu_profile_argument_debug(struct emu_profile_argument *argument, int indent)
{
//	printf("%s %s = ", indents(indent), argument->argname);
	switch(argument->render)
	{
	case render_struct:
		printf("%s struct %s %s = {\n", indents(indent), argument->argtype, argument->argname);


		struct emu_profile_argument *argumentit;
		for (argumentit = emu_profile_arguments_first(argument->value.tstruct.arguments); 
			  !emu_profile_arguments_istail(argumentit); 
			  argumentit = emu_profile_arguments_next(argumentit))
		{
			emu_profile_argument_debug(argumentit,indent+1);
		}

		printf("%s };\n", indents(indent));
		break;

	case render_array:
		printf("%s %s %s = [\n", indents(indent), argument->argtype, argument->argname);
		for (argumentit = emu_profile_arguments_first(argument->value.tstruct.arguments); 
			  !emu_profile_arguments_istail(argumentit); 
			  argumentit = emu_profile_arguments_next(argumentit))
		{
			emu_profile_argument_debug(argumentit,indent+1);
		}
		printf("%s ];\n", indents(indent));
		break;

	case render_int:
		printf("%s %s %s = %i;\n", indents(indent), argument->argtype, argument->argname, argument->value.tint);
		break;

	case render_string:
		printf("%s %s %s = \"%s\";\n", indents(indent), argument->argtype, argument->argname, argument->value.tchar);
		break;

	case render_ptr:
		{
			struct emu_profile_argument *argit = argument;
			while (argit->render == render_ptr)
			{
				argit = argit->value.tptr.ptr;
			}

			if (argit->render == render_struct)
				printf("%s struct %s %s = 0x%08x => \n", indents(indent), argument->argtype, argument->argname, argument->value.tptr.addr);
			else
				printf("%s %s %s = 0x%08x => \n", indents(indent), argument->argtype, argument->argname, argument->value.tptr.addr);

			emu_profile_argument_debug(argument->value.tptr.ptr, indent+1);
		}
		break;

	case render_ip:
		printf("%s %s %s = %i (host=%s);\n", indents(indent), argument->argtype, argument->argname, argument->value.tint, inet_ntoa(*(struct in_addr *)&argument->value.tint));
		break;

	case render_port:
		printf("%s %s %s = %i (port=%i);\n", indents(indent), argument->argtype, argument->argname, argument->value.tint, ntohs((uint16_t)argument->value.tint));
		break;

	case render_none:
		printf("%s none;\n", indents(indent));
		break;
	}
}


void emu_profile_debug(struct emu_profile *profile)
{
	struct emu_profile_function *function;
	for (function = emu_profile_functions_first(profile->functions); !emu_profile_functions_istail(function); function = emu_profile_functions_next(function))
	{
		emu_profile_function_debug(function);
	}

}

void emu_profile_function_debug(struct emu_profile_function *function)
{
	printf("%s ", function->fnname);
	printf("(\n");
	struct emu_profile_argument *argument;
	for (argument = emu_profile_arguments_first(function->arguments); 
		  !emu_profile_arguments_istail(argument); 
		  argument = emu_profile_arguments_next(argument))
	{
		emu_profile_argument_debug(argument,1);
	}
	printf(");\n");
}


