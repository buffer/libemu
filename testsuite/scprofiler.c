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


struct run_options
{
	char *profile_name;
	char *function_name;
	char *argument_name;
	bool profile;
	bool debug_dump;

};


struct run_options opts;

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

typedef void (*print_function)(struct emu_profile_function *function, void *args[]);

struct function_render
{
	const char *function;
	print_function printer;
};


void print_connect(struct emu_profile_function *function, void *args[])
{
//	int connect(int sockfd, const struct sockaddr *serv_addr, socklen_t addrlen);
	int retval = *(int *)args[0];
	int sockfd = *(int *)args[1];
	struct sockaddr_in *addrin = *(struct sockaddr_in **)(void **)args[2];
	socklen_t addrlen = *(socklen_t *)args[3];

	printf("%i = %s(int sockfd=%i, const struct sockaddr *serv_addr=%s:%i, socklen_t addrlen=%i);\n",
		   retval,
		   function->fnname,
		   sockfd,
		   inet_ntoa(addrin->sin_addr), 
		   ntohs(addrin->sin_port),
		   addrlen);

}

void print_CreateProcess(struct emu_profile_function *function, void *args[])
{
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

	bool retval = *(bool *)args[0];
	char *psCmdLine = *(char **)(void **)args[2];

	printf("%i = %s(\"%s\");\n", 
		   retval,
		   function->fnname,
		   psCmdLine);

}

struct function_render function_rendering[] =
{
	{ "connect", print_connect},
	{ "bind", print_connect},
	{ "CreateProcess", print_CreateProcess}
};


int main(int argc, char *argv[])
{
	int option_index = 0;

	memset(&opts, 0, sizeof(struct run_options));

	while ( 1 )
	{
		int c;

		static struct option long_options[] = {
			{"argument"         , 1, 0, 'a'},
			{"debugdump"        , 1, 0, 'd'},
			{"function"         , 1, 0, 'f'},
			{"profile"          , 0, 0, 'p'},
			{0, 0, 0, 0}
		};

		c = getopt_long (argc, argv, "a:df:p", long_options, &option_index);
		if ( c == -1 )
			break;

		switch ( c )
		{
		

		case 'a':
			opts.argument_name = strdup(optarg);
			printf("argument %s\n", optarg);
			break;

		case 'd':
			opts.debug_dump = true;
			break;

		case 'f':
			opts.function_name = strdup(optarg);
			printf("function %s\n", optarg);
			break;

		case 'p':
			opts.profile = true;
			break;

		default:
			printf ("?? getopt returned character code 0%o ??\n", c);
			break;

		}
	}

	if ( optind < argc )
	{
		opts.profile_name = strdup(argv[optind]);
		printf("Profile %s\n", argv[optind]);
	}

	struct emu_profile *profile = emu_profile_new();
	if ( emu_profile_parse(profile, opts.profile_name) != 0 )
	{
		printf("error parsing file %s!\n",opts.profile_name);
		return -1;
	}



	if ( opts.profile )
	{
//		printf("profiling ....\n");
		enum profile_state
		{
			PS_NONE,
			PS_WSASOCKET,
			PS_BIND,
			PS_LISTEN,
			PS_ACCEPT,
			PS_CONNECT,
			PS_CREATEPROCESS,
			PS_URLDOWNLOADTOFILE
		};

		enum profile_state state = PS_NONE;
		struct emu_profile_function *function;
		for ( function = emu_profile_functions_first(profile->functions); !emu_profile_functions_istail(function); function = emu_profile_functions_next(function) )
		{
//			printf("state %i\n", state);
			switch ( state )
			{
			case PS_NONE:
				if ( strcmp("WSAStartup", function->fnname) == 0 )
					state = PS_WSASOCKET;
				else
					if ( strcmp("URLDownloadToFile", function->fnname) == 0 )
					printf("url download\n");
				break;


			case PS_WSASOCKET:
				if ( strcmp("bind", function->fnname) == 0 )
					state = PS_BIND;
				else
					if ( strcmp("connect", function->fnname) == 0 )
					state = PS_CONNECT;
				break;

			case PS_BIND:
				if ( strcmp("listen", function->fnname) == 0 )
					state = PS_LISTEN;

				break;

			case PS_LISTEN:
				if ( strcmp("accept", function->fnname) == 0 )
					state = PS_ACCEPT;
				break;

			case PS_ACCEPT:
				if ( strcmp("CreateProcess", function->fnname) == 0 )
					printf("bindshell\n");
				else
					if ( strcmp("recv", function->fnname) == 0 )
					printf("bindfiletransfer");
				break;

			case PS_CONNECT:
				if ( strcmp("CreateProcess", function->fnname) == 0 )
					printf("connectbackshell\n");
				else
					if ( strcmp("recv", function->fnname) == 0 )
					printf("connectback transfer");

				break;

			default:
				break;

			}
		}
	}

	if ( opts.debug_dump )
	{
		emu_profile_debug(profile);
	}

	struct emu_profile_function *function;
	for ( function = emu_profile_functions_first(profile->functions); !emu_profile_functions_istail(function); function = emu_profile_functions_next(function) )
	{

		int argc = emu_profile_arguments_length(function->arguments);
		void **args = malloc( (argc+1) * sizeof(void *));

		int i;
		for ( i = 0; i <= argc; i++ )
			args[i] = emu_profile_function_argument_get(function, i);

		for ( i=0;i< sizeof(function_rendering)/sizeof(struct function_render);i++ )
		{
			if ( strcmp(function->fnname, function_rendering[i].function) == 0 )
			{
				function_rendering[i].printer(function, args);
				goto found_function;
			}
		}

		printf("could not find function %s\n",function->fnname);

		found_function:
		for ( i = 0; i < argc+1; i++ )
			free(args[i]);
	}

//	emu_profile_debug(profile);
	emu_profile_free(profile);
	return 0;
}

