#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/select.h>
#include <unistd.h>

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <getopt.h>


#include <emu/emu.h>
#include <emu/emu_memory.h>
#include <emu/emu_cpu.h>
#include <emu/log.h>


struct run_options
{
	uint8_t single_step;
	uint8_t append_no_break;
	uint8_t stdin;
};

int main(int argc, char *argv[])
{

	struct run_options opt;
	memset(&opt,0,sizeof(struct run_options));

	int c;
//	int digit_optind = 0;

	while ( 1 )
	{
//		int this_option_optind = optind ? optind : 1;
		int option_index = 0;
		static struct option long_options[] = {
			{"append-no-break"   , 0, 0, 'a'},
			{"single-step"       , 0, 0, 's'},
			{"stdin"             , 0, 0, 'S'},
			{0, 0, 0, 0}
		};

		c = getopt_long (argc, argv, "asS", long_options, &option_index);
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
			opt.stdin = 1;
			printf("reading from stdin\n");
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

	
	unsigned char *scode=NULL;
	uint32_t size;

	if ( opt.stdin )
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
		printf("stdin is the way to go!\n");
	}


	struct emu *e = emu_new();
	emu_log_level_set(emu_logging_get(e),EMU_LOG_DEBUG);


	struct emu_memory *mem = emu_memory_get(e);
	int i;
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
