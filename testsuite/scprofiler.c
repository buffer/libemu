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


#include "../config.h"

#define HAVE_GETOPT_H
#ifdef HAVE_GETOPT_H
	#include <getopt.h>
#endif

#ifdef HAVE_STDLIB_H
	#include <stdlib.h>
#endif


#include <stdint.h>

#define HAVE_UNISTD
#ifdef HAVE_UNISTD
	#include <unistd.h>
#endif
#include <stdio.h>
#include <string.h>

#include <arpa/inet.h>

#include "emu/environment/emu_profile.h"
#include "emu/environment/win32/emu_env_w32.h"


struct emu_profile_function *find_function(struct emu_profile *profile, const char *fnname)
{
	struct emu_profile_function *function;
	for ( function = emu_profile_functions_first(profile->functions); !emu_profile_functions_istail(function); function = emu_profile_functions_next(function) )
	{
		if ( strcmp(function->fnname,fnname) == 0 )
			return function;
	}
	return NULL;
}

struct emu_profile_argument *find_argument(struct emu_profile_function *function, const char *name)
{
	struct emu_profile_argument *argument;
	for ( argument = emu_profile_arguments_first(function->arguments); !emu_profile_arguments_istail(argument); argument = emu_profile_arguments_next(argument) )
	{
		if ( strcmp(argument->argname,name) == 0 )
			return argument;
	}

	return NULL;
}

int main(int argc, char *argv[])
{
	int option_index = 0;

	char *profile_name = NULL;
	char *function_name = NULL;
	char *argument_name = NULL;

	while ( 1 )
	{
		int c;

		static struct option long_options[] = {
			{"function"         , 1, 0, 'f'},
			{"argument"         , 1, 0, 'a'},
			{0, 0, 0, 0}
		};

		c = getopt_long (argc, argv, "f:a:", long_options, &option_index);
		if ( c == -1 )
			break;

		switch ( c )
		{
		
		case 'f':
			function_name = strdup(optarg);
			printf("function %s\n", optarg);
			break;

		case 'a':
			argument_name = strdup(optarg);
			printf("argument %s\n", optarg);
			break;

		default:
			printf ("?? getopt returned character code 0%o ??\n", c);
			break;

		}
	}

	if ( optind < argc )
	{
		profile_name = strdup(argv[optind]);
		printf("Profile %s\n", argv[optind]);
	}

	struct emu_profile *profile = emu_profile_new();
	if ( emu_profile_parse(profile, profile_name) != 0 )
	{
		printf("error parsing file %s!\n",profile_name);
		return -1;
	}


	struct emu_profile_function *function;
	for ( function = emu_profile_functions_first(profile->functions); !emu_profile_functions_istail(function); function = emu_profile_functions_next(function) )
	{
		printf("%s\n", function->fnname);
		int i=0;
		uint8_t *data;
		while ((data = emu_profile_function_argument_get(function, i)) != NULL)
		{

			if ( (strcmp(function->fnname, "bind") == 0  || strcmp(function->fnname, "connect") == 0 )
				&& i == 1)
			{

				int j;
				for (j=0; j < 22; j++)
				{
					printf("%02x ", data[j]);
				}
				printf("\n");
				struct sockaddr_in *addrin = *(struct sockaddr_in **)(void **)data;
				printf("host %s:%i\n", inet_ntoa(addrin->sin_addr), ntohs(addrin->sin_port));
				printf("sizeof(struct sockaddr_in)=%i\n",sizeof(struct sockaddr_in)); 
			}else
			if (strcmp(function->fnname, "LoadLibraryA") == 0
					&& i == 0 )
			{
				const char *dllname = *(const char **)(void **)data;
				printf("filename %s\n", dllname);
				
			}
			free(data);
			i++;

		}

	}
	

//	return 0;
	emu_profile_debug(profile);

	return 0;
/*	struct emu_profile_function *function;
	if ((function = find_function(profile, function_name)) == NULL)
	{
		printf("Could not find function %s\n", function_name);
		return -1;
	}

	struct emu_profile_argument *argument;

	if ((argument = find_argument(function, argument_name)) == NULL)
	{
		printf("Could not find argument %s\n", argument_name);
		return -1;
	}
	emu_profile_argument_debug(argument, 0);

	return 0;
*/
}



