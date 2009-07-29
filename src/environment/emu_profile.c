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



#include "emu/environment/emu_profile.h"




/*
static char *renderings[] =
{

	"render_none",
	"render_ptr",
	"render_int",
	"render_struct",
	"render_string",
	"render_ip",
	"render_port",
	"render_array"
};
*/
typedef unsigned char byte;

source_list_functions(emu_profile_functions,emu_profile_function_root, emu_profile_function, link);

source_list_functions(emu_profile_arguments,emu_profile_argument_root, emu_profile_argument, link);

struct emu_profile *emu_profile_new(void)
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

void emu_profile_argument_add_int(struct emu_profile *profile, char *argtype, char *argname, int32_t value)
{

    struct emu_profile_argument *argument = emu_profile_argument_new(render_int, argtype, argname);
	argument->value.tint = value;
	emu_profile_argument_add(profile, argument);
}

void emu_profile_argument_add_short(struct emu_profile *profile, char *argtype, char *argname, int16_t value)
{
    struct emu_profile_argument *argument = emu_profile_argument_new(render_short, argtype, argname);
	argument->value.tshort = value;
	emu_profile_argument_add(profile, argument);
}


void emu_profile_argument_add_string(struct emu_profile *profile, char *argtype, char *argname, char *value)
{

    struct emu_profile_argument *argument = emu_profile_argument_new(render_string, argtype, argname);
	if (value == NULL)
		value = "";
	argument->value.tchar = strdup(value);
	emu_profile_argument_add(profile, argument);
}

void emu_profile_argument_add_bytea(struct emu_profile *profile, char *argtype, char *argname, unsigned char *data, uint32_t size)
{

    struct emu_profile_argument *argument = emu_profile_argument_new(render_bytea, argtype, argname);
	argument->value.bytea.data = malloc(size);
	memcpy(argument->value.bytea.data, data, size);
	argument->value.bytea.size = size;
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

void emu_profile_argument_add_sockaddr_ptr(struct emu_profile *profile, const char *name, uint32_t ptr, struct sockaddr sa)
{

	if ( sa.sa_family == AF_INET )
	{                                                                                          
		struct sockaddr_in *si = (struct sockaddr_in *)&sa;                                    
		emu_profile_argument_add_ptr(profile, "sockaddr_in *", (char*)name, ptr);               
		emu_profile_argument_struct_start(profile, "", "");                                    
		emu_profile_argument_add_short(profile, "short", "sin_family", si->sin_family);        
		emu_profile_argument_add_port(profile, "unsigned short", "sin_port", si->sin_port);    
		emu_profile_argument_struct_start(profile, "in_addr", "sin_addr");                     
		emu_profile_argument_add_ip(profile, "unsigned long", "s_addr", si->sin_addr.s_addr);  
		emu_profile_argument_struct_end(profile);                                              
		emu_profile_argument_add_string(profile, "char", "sin_zero", "       ");               
		emu_profile_argument_struct_end(profile);                                              

	}
	else
	{                                                                                          
		emu_profile_argument_struct_start(profile, "sockaddr *", "name");                      
		emu_profile_argument_struct_end(profile);                                              
	}                                                                                          

}

struct emu_profile_function *emu_profile_function_new(void)
{
	struct emu_profile_function *function = malloc(sizeof(struct emu_profile_function));
	memset(function, 0, sizeof(struct emu_profile_function));

	function->arguments = emu_profile_arguments_create();
	emu_profile_functions_init_link(function);
	function->return_value = emu_profile_argument_new(render_int, "ERROR ", "");
	function->return_value->value.tint = -1;
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
	emu_profile_argument_free(function->return_value);

	free(function);
}

void emu_profile_function_returnvalue_int_set(struct emu_profile *profile, const char *type, int value)
{
	struct emu_profile_function *function = emu_profile_functions_last(profile->functions);
	if (function->return_value->argtype != NULL)
		free(function->return_value->argtype);
	function->return_value->argtype = strdup(type);
	function->return_value->render = render_int;
	function->return_value->value.tint = value;
}

void emu_profile_function_returnvalue_ptr_set(struct emu_profile *profile, const char *type, int value)
{
	struct emu_profile_function *function = emu_profile_functions_last(profile->functions);
	if (function->return_value->argtype != NULL)
		free(function->return_value->argtype);
	function->return_value->argtype = strdup(type);
	function->return_value->render = render_ptr;
	function->return_value->value.tptr.addr = value;
	profile->last_ref = function->return_value;
}

uint32_t measure_size(struct emu_profile_argument *argument, bool followptr)
{
	uint32_t size = 0;
	struct emu_profile_argument *argumentit;

	switch(argument->render)
	{
	case render_struct:
		for (argumentit = emu_profile_arguments_first(argument->value.tstruct.arguments); 
			  !emu_profile_arguments_istail(argumentit); 
			  argumentit = emu_profile_arguments_next(argumentit))
		{
			size += measure_size(argumentit,  followptr);
		}
		break;

	case render_array:

		for (argumentit = emu_profile_arguments_first(argument->value.tstruct.arguments); 
			  !emu_profile_arguments_istail(argumentit); 
			  argumentit = emu_profile_arguments_next(argumentit))
		{
			size += measure_size(argumentit, followptr);

		}
		break;

	case render_int:
		size += 4;
		break;

	case render_short:
		size += 2;
		break;

	case render_string:
		size += strlen(argument->value.tchar) +1;
		break;

	case render_bytea:
		size += argument->value.bytea.size;
		break;

	case render_ptr:
		{
			size += sizeof(uintptr_t); // += 4;
			if (followptr)
				size += measure_size(argument->value.tptr.ptr, followptr);
		}
		break;

	case render_ip:
		size += 4;
		break;

	case render_port:
		size += 2;
		break;

	case render_none:
		size += 4;
		break;
	}

	return size;
}

int copy_data(struct emu_profile_argument *argument, uint8_t *addr, uint8_t **next)
{
//	printf("%s : %i \n", __PRETTY_FUNCTION__, __LINE__);

	uintptr_t *addrp = (uintptr_t *)addr;
	uint32_t *addr32 = (uint32_t *)addr;
	uint16_t *addr16 = (uint16_t *)addr;

	int size = measure_size(argument, false);
	int offset = 0;

	struct emu_profile_argument *argumentit;

	if (addr == *next)
	{
		*next = addr + size;
	}

	switch(argument->render)
	{
	case render_struct:
		for (argumentit = emu_profile_arguments_first(argument->value.tstruct.arguments); 
			  !emu_profile_arguments_istail(argumentit); 
			  argumentit = emu_profile_arguments_next(argumentit))
		{
//			size += measure_size(argumentit,  false);
			offset += copy_data(argumentit, addr+offset, next);
		}
		break;

	case render_array:

		for (argumentit = emu_profile_arguments_first(argument->value.tstruct.arguments); 
			  !emu_profile_arguments_istail(argumentit); 
			  argumentit = emu_profile_arguments_next(argumentit))
		{
//			size += measure_size(argumentit, level - 1);
			offset += copy_data(argumentit, addr+offset, next);
		}
		break;

	case render_int:
//		size += 4;
		*addr32 = argument->value.tint;
		break;

	case render_short:
		*addr16 = argument->value.tshort;
		break;

	case render_string:
//		size += strlen(argument->value.tchar) +1;
		strcpy((char *)addr, argument->value.tchar); 
		break;

	case render_bytea:
		memcpy((char *)addr, argument->value.bytea.data, argument->value.bytea.size);
		break;

	case render_ptr:
		{
//			size += 4;
//			size += measure_size(argument->value.tptr.ptr, false);
			*addrp = (uintptr_t)*next;

			copy_data(argument->value.tptr.ptr, *next, next);
		}
		break;

	case render_ip:
//		size += 4;
		*addr32 = argument->value.tint;
		break;

	case render_port:
//		size += 2;		
		*addr16 = (uint16_t)argument->value.tint;

		break;

	case render_none:
//		size += 4;
		break;
	}

	return size;
}

void *emu_profile_function_argument_get(struct emu_profile_function *function, int argc)
{
	struct emu_profile_argument *argument;

	if ( argc == 0 )
	{
		argument = function->return_value;
	}else
	{
		int i = 1;
		argument = emu_profile_arguments_first(function->arguments);
		while ( i < argc )
		{
			argument = emu_profile_arguments_next(argument);
			i++;

			if ( emu_profile_arguments_istail(argument) )
				return NULL;
		}
	}
	
	uint32_t size = 0;
	size = measure_size(argument, true);

//	printf("%s size is %i\n", argument->argname, size);

	uint8_t *data = malloc(size);
	uint8_t *next = data;
	memset(data, 0x90, size);
	copy_data(argument, data, &next);

	return data;
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
	case render_short:
		break;

	case render_string:
		if (argument->value.tchar != NULL)
			free(argument->value.tchar);
		break;

	case render_bytea:
		if (argument->value.bytea.data != NULL)
			free(argument->value.bytea.data);
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

	case render_short:
		printf("%s %s %s = %i;\n", indents(indent), argument->argtype, argument->argname, argument->value.tshort);
		break;


	case render_string:
		printf("%s %s %s = \"%s\";\n", indents(indent), argument->argtype, argument->argname, argument->value.tchar);
		break;

	case render_bytea:
		printf("%s %s %s = \"%s\" (%i bytes);\n", indents(indent), argument->argtype, argument->argname, ".binary.", argument->value.bytea.size);
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
	printf("%s %s ", function->return_value->argtype, function->fnname);
	printf("(\n");
	struct emu_profile_argument *argument;
	for (argument = emu_profile_arguments_first(function->arguments); 
		  !emu_profile_arguments_istail(argument); 
		  argument = emu_profile_arguments_next(argument))
	{
		emu_profile_argument_debug(argument,1);
	}
	printf(")");
	switch (function->return_value->render)
	{
	case render_none:
		printf(";\n");
		break;
	case render_int:
		printf(" =  %i;\n", function->return_value->value.tint);
		break;

	case render_ptr:
		printf(" = 0x%08x;\n", function->return_value->value.tptr.addr);
		break;
	default:
		printf(";\n");
		break;

	}

}


int emu_profile_dump_byte_write(FILE *f, byte value)
{
	if (fwrite(&value, 1, 1, f) == 1)
		return 0;
	return -1;
}

int emu_profile_dump_int_write(FILE *f, int value)
{
	uint32_t nval = htonl(value);

	if (fwrite(&nval, 4, 1, f) == 4)
		return 0;
	return -1;
}

int emu_profile_dump_short_write(FILE *f, int16_t value)
{
	int16_t nval = htons(value);

	if (fwrite(&nval, 2, 1, f) == 2)
		return 0;
	return -1;
}


int emu_profile_dump_string_write(FILE *f, const char *string)
{
	uint32_t strsize = 0;
	if (string)
		strsize = strlen(string);
	emu_profile_dump_int_write(f, strsize);
	if (fwrite(string, strsize, 1, f) == strsize)
		return 0;
	return -1;
}

int emu_profile_dump_bytea_write(FILE *f, const unsigned char *data, uint32_t size)
{
	emu_profile_dump_int_write(f, size);
	if (fwrite(data, size, 1, f) == size)
		return 0;
	return -1;
}


int emu_profile_argument_dump(FILE *f, struct emu_profile_argument *argument)
{
	emu_profile_dump_byte_write(f, (byte)argument->render);
	emu_profile_dump_string_write(f, argument->argtype);
	emu_profile_dump_string_write(f, argument->argname);

	switch ( argument->render )
	{
	case render_struct:
	case render_array:
		{
			emu_profile_dump_int_write(f, emu_profile_arguments_length(argument->value.tstruct.arguments));

			struct emu_profile_argument *argumentit=NULL;
			for ( argumentit = emu_profile_arguments_first(argument->value.tstruct.arguments); 
				!emu_profile_arguments_istail(argumentit); 
				argumentit = emu_profile_arguments_next(argumentit) )
			{
				emu_profile_argument_dump(f, argumentit);
			}

		}
		break;

	case render_int:
		emu_profile_dump_int_write(f, argument->value.tint);
		break;

	case render_short:
		emu_profile_dump_short_write(f, argument->value.tshort);
		break;


	case render_string:
		emu_profile_dump_string_write(f, argument->value.tchar);
		break;

	case render_bytea:
		emu_profile_dump_bytea_write(f, argument->value.bytea.data, argument->value.bytea.size);
		break;

	case render_ip:
	case render_port:

		if (fwrite(&argument->value.tint, 4, 1, f) == 4)
			return 0;

		break;

	case render_none:
		break;

	case render_ptr:
		emu_profile_dump_int_write(f, argument->value.tptr.addr);
		emu_profile_argument_dump(f, argument->value.tptr.ptr);
		break;
	}

	return 0;
}

int emu_profile_function_dump(FILE *f, struct emu_profile_function *function)
{
	
	emu_profile_dump_string_write(f, function->fnname);
	
	emu_profile_dump_int_write(f, emu_profile_arguments_length(function->arguments));

	struct emu_profile_argument *argumentit;
	for (argumentit = emu_profile_arguments_first(function->arguments); 
		  !emu_profile_arguments_istail(argumentit); 
		  argumentit = emu_profile_arguments_next(argumentit))
	{
		emu_profile_argument_dump(f, argumentit);
	}

	emu_profile_argument_dump(f, function->return_value);

	return 0;
}

int emu_profile_dump(struct emu_profile *profile, const char *path)
{
	

/*
	FUNCTION ARGUMENTS RETVAL


	RETVAL : ARGUMENTS
	FUNCTION: name STRING
			  argcount INT

	ARGUMENTS: ARGUMENT(s)

	ARGUMENT:
	render: 1 byte
	type: STRING
	name: STRING
	optcount: INT
	ARGUMENT(s)

	STRING: INT len
			char string[len]
*/

	FILE *f;

	if ((f = fopen(path, "w+")) == NULL)
		return -1;


	emu_profile_dump_int_write(f, emu_profile_functions_length(profile->functions));

	struct emu_profile_function *function;
	for (function = emu_profile_functions_first(profile->functions); !emu_profile_functions_istail(function); function = emu_profile_functions_next(function))
	{
		emu_profile_function_dump(f, function);
	}

	fclose(f);
	return 0;
}

int emu_profile_dump_byte_read(FILE *f, byte *b)
{
	if (fread(b, 1, 1, f) == 1)
	{
		return 0;
	}
	return -1;
}

int emu_profile_dump_int_read(FILE *f, int *i)
{
	if (fread(i, 1, 4, f) == 4)
	{
		*i = ntohl(*i);
    	return 0;
	}
	return -1;
}

int emu_profile_dump_short_read(FILE *f, int16_t *i)
{
	if (fread(i, 1, 2, f) == 2)
	{
		*i = ntohs(*i);
    	return 0;
	}
	return -1;
}



int emu_profile_dump_string_read(FILE *f, char **string)
{
	int strsize = 0;
	emu_profile_dump_int_read(f, &strsize);
	*string = malloc(strsize+1);
	memset(*string, 0, strsize+1);
	if (fread(*string, 1, strsize, f) != strsize)
		return -1;
	return 0;
}

int emu_profile_dump_bytea_read(FILE *f, unsigned char **data, uint32_t *size)
{
	emu_profile_dump_int_read(f, (int *)size);
	*data = malloc(*size);
	memset(*data, 0, *size);
	if (fread(*data, 1, *size, f) != *size)
		return -1;
	return 0;
}


int emu_profile_argument_parse(FILE *f, struct emu_profile *profile)
{
	byte render;
	char *argtype;
	char *argname;

	emu_profile_dump_byte_read(f, &render);
	emu_profile_dump_string_read(f, &argtype);
	emu_profile_dump_string_read(f, &argname);

	if (render > render_array)
		return 0;

//	printf("%i %s %s %s\n",render , renderings[render], argtype, argname);

	switch ( render )
	{
	
	case render_struct:
		{

			emu_profile_argument_struct_start(profile, argtype, argname);
			int argcount=0;
			emu_profile_dump_int_read(f, &argcount);
//			printf("parsing %i struct arguments\n", argcount);
			while ( argcount > 0 )
			{
				emu_profile_argument_parse(f, profile);
				argcount--;
			}
			emu_profile_argument_struct_end(profile);
		}
		break;

	case render_array:
		{

			emu_profile_argument_array_start(profile, argtype, argname);
			int argcount=0;
			emu_profile_dump_int_read(f, &argcount);
//			printf("parsing %i array arguments\n", argcount);
			while ( argcount > 0 )
			{
				emu_profile_argument_parse(f, profile);
				argcount--;
			}
			emu_profile_argument_array_end(profile);
		}
		break;

	case render_int:
		{
			int value = 0;
			emu_profile_dump_int_read(f, &value);
			emu_profile_argument_add_int(profile, argtype, argname, value);
		}
		break;

	case render_short:
		{
			int16_t value = 0;
			emu_profile_dump_short_read(f,&value);
			emu_profile_argument_add_short(profile, argtype, argname, value);
		}
		break;

	case render_string:
		{
			char *string;
			emu_profile_dump_string_read(f, &string);
			emu_profile_argument_add_string(profile, argtype, argname, string);
		}
		break;

	case render_bytea:
		{
			unsigned char *data;
			uint32_t size;
			emu_profile_dump_bytea_read(f, &data, &size);
			emu_profile_argument_add_bytea(profile, argtype, argname, data, size);
		}
		break;

	case render_port:
		{

			uint32_t x = 0;
			if ( fread(&x, 4, 1, f) == 4)
				emu_profile_argument_add_port(profile, argtype, argname, x);
		}
		break;

	case render_ip:
		{
			uint32_t x = -1;
        	if ( fread(&x, 4, 1, f) == 4)
				emu_profile_argument_add_ip(profile, argtype, argname, x);
		}
		break;

	case render_none:
		emu_profile_argument_add_none(profile);
		break;
	
	case render_ptr:
		{
			int addr = -1;
			emu_profile_dump_int_read(f, &addr);
        	emu_profile_argument_add_ptr(profile, argtype, argname, addr);
			emu_profile_argument_parse(f, profile);
		}
		break;

	}

	return 0;
}

int emu_profile_function_parse(FILE *f, struct emu_profile *profile)
{
	char *fnname;

	emu_profile_dump_string_read(f, &fnname);
	emu_profile_function_add(profile, fnname);

	int argcount = 0;
	emu_profile_dump_int_read(f, &argcount);
//	printf("parsing %i function arguments\n", argcount);
	while (argcount > 0)
	{
		emu_profile_argument_parse(f, profile);
		argcount--;
	}
	emu_profile_argument_parse(f, profile);

	struct emu_profile_function *function = emu_profile_functions_last(profile->functions);
	struct emu_profile_argument *argument = emu_profile_arguments_remove_last(function->arguments);
	function->return_value = argument;

	return 0;
}

int emu_profile_parse(struct emu_profile *profile, const char *path)
{
	FILE *f;

	if ((f = fopen(path, "r")) == NULL)
		return -1;

	int functions = 0;
	emu_profile_dump_int_read(f, &functions);
//	printf("parsing %i functions\n", functions);
	while (functions > 0)
	{
		emu_profile_function_parse(f, profile);
		functions--;
	}

	fclose(f);
	return 0;

}
