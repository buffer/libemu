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


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/select.h>
#include <unistd.h>
#include <stdint.h>

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <getopt.h>


#include "emu/emu.h"
#include "emu/emu_memory.h"
#include "emu/emu_cpu.h"
#include "emu/emu_log.h"

static const char *regm[] = {
	"eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi"
};

struct run_options
{
	uint8_t single_step;
	uint8_t append_no_break;
	uint8_t from_stdin;

	uint32_t regs[8];
};

int main(int argc, char *argv[])
{

	struct run_options opt;
	memset(&opt, 0, sizeof(struct run_options));

	int c;
	int i;
//	int digit_optind = 0;

	while ( 1 )
	{
//		int this_option_optind = optind ? optind : 1;
		int option_index = 0;
		static struct option long_options[] = {
			{"append-no-break"	, 0, 0, 'a'},
			{"single-step"		, 0, 0, 's'},
			{"eax"				, 1, 0, 'A'},
			{"ebx"				, 1, 0, 'B'},
			{"ecx"				, 1, 0, 'C'},
			{"edx"				, 1, 0, 'D'}, 
			{"edi"				, 1, 0, 'I'},
			{"ebp"				, 1, 0, 'P'},
			{"esi"				, 1, 0, 'X'},
			{"esp"				, 1, 0, 'Y'},
			{"stdin"			, 0, 0, 'S'},
			{0, 0, 0, 0}
		};

		c = getopt_long (argc, argv, "asSA:B:C:D:I:P:X:Y:", long_options, &option_index);
		if ( c == -1 )
			break;

		switch ( c )
		{
		case 'a':
			opt.append_no_break = 1;
			printf("won't append break\n");
			break;

		case 's':
			opt.single_step = 1;
			printf("single step\n");
			break;

		case 'S':
			opt.from_stdin = 1;
			printf("reading from stdin\n");
			break;

		case 'A': // eax
			if ( strncmp(optarg,"0x", 2) == 0 )
				opt.regs[eax] = strtoul(optarg+2, NULL, 16);
			else
				opt.regs[eax] = strtoul(optarg, NULL, 10);

			break;

		case 'B': // ebx
			if ( strncmp(optarg,"0x", 2) == 0 )
				opt.regs[ebx] = strtoul(optarg+2, NULL, 16);
			else
				opt.regs[ebx]	= strtoul(optarg, NULL, 10);
			break;

		case 'C': // ecx
			if ( strncmp(optarg,"0x", 2) == 0 )
				opt.regs[ecx] = strtoul(optarg+2, NULL, 16);
			else
				opt.regs[ecx]	= strtoul(optarg, NULL, 10);
			break;

		case 'D': // edx
			if ( strncmp(optarg,"0x", 2) == 0 )
				opt.regs[edx] = strtoul(optarg+2, NULL, 16);
			else
				opt.regs[edx]	= strtoul(optarg, NULL, 10);
			break;

		case 'I': // edi
			if ( strncmp(optarg,"0x", 2) == 0 )
				opt.regs[edi] = strtoul(optarg+2, NULL, 16);
			else
				opt.regs[edi]	= strtoul(optarg, NULL, 10);
			break;

		case 'P': // ebp
			if ( strncmp(optarg,"0x", 2) == 0 )
				opt.regs[ebp] = strtoul(optarg+2, NULL, 16);
			else
				opt.regs[ebp]	= strtoul(optarg, NULL, 10);
			break;

		case 'X': // esi
			if ( strncmp(optarg,"0x", 2) == 0 )
				opt.regs[esi] = strtoul(optarg+2, NULL, 16);
			else
				opt.regs[esi]	= strtoul(optarg, NULL, 10);
			break;

		case 'Y': // esp
			if (strncmp(optarg,"0x", 2) == 0)
				opt.regs[esp] = strtoul(optarg+2, NULL, 16);
			else
				opt.regs[esp] = strtoul(optarg, NULL, 10);
			break;

		default:
			printf ("?? getopt returned character code 0%o ??\n", c);
			break;
		}
	}

/*
	if ( optind < argc )
	{
		printf ("non-option ARGV-elements: ");
		while ( optind < argc )
			printf ("%s ", argv[optind++]);
		printf ("\n");
	}
*/
	for ( i=0;i<8;i++ )
	{
		printf("%s is %08x\n", regm[i],(unsigned int)opt.regs[i]);
	}




	unsigned char *scode=NULL;
	uint32_t size;

	if ( opt.from_stdin )
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
						if ( (scode = (unsigned char *) realloc(scode, len+bytes_read)) == NULL )
						{
							fprintf(stderr, "Error while allocating memory: %s.\n", strerror(errno));
							exit(1);
						}
						memcpy(scode+len, buffer, bytes_read);
						len += bytes_read;
					}
				}
			}
		}
		size = len;
	}else
	{
		printf("reading from file %s\n", argv[optind]);
		FILE *f;
		if (( f = fopen(argv[optind],"r")) == NULL)
			perror("could not open file");

		int16_t bytes_read=0;
		uint32_t len=0;

		while (feof(f) == 0)
		{
			unsigned buffer[BUFSIZ];
			bytes_read = fread(buffer, 1, 1, f);
//			printf("read %i bytes %s %i\n", bytes_read, strerror(errno), ferror(f));
			if ( (scode = (unsigned char *) realloc(scode, len+bytes_read)) == NULL )
			{
				fprintf(stderr, "Error while allocating memory: %s.\n", strerror(errno));
				exit(1);
			}
			memcpy(scode+len, buffer, bytes_read);
			len += bytes_read;
		}
		printf("read %i bytes\n", len);
		int i;
		for (i=0;i<len;i++)
		{
			printf("%02x ", scode[i]);
		}
		size = len;
	}


	struct emu *e = emu_new();
	for ( i=0;i<8;i++ )
	{
		emu_cpu_reg32_set(emu_cpu_get(e), i , opt.regs[i]);
	}

	emu_log_level_set(emu_logging_get(e), EMU_LOG_DEBUG);


	struct emu_memory *mem = emu_memory_get(e);
	int static_offset = 4711;
	for( i = 0; i < size; i++ )
	{
		emu_memory_write_byte(mem, static_offset+i, scode[i]);
	}
	emu_memory_write_byte(mem, static_offset+i, '\xcc');

	emu_cpu_eip_set(emu_cpu_get(e), static_offset);
	emu_cpu_run(emu_cpu_get(e));

	emu_free(e);


	return 0;
}
