/*         
 *               emunids
 *
 *   emulation based network intrusion system
 *
 *   Markus Koetter 2007
 *
 *   gcc -Wall -I/opt/libemu/include/ -L/opt/libemu/lib/libemu -o printall printall.c -lnids -lemu
 *
 *
 */

#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/in_systm.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include "nids.h"
#include <pcap.h>
#include <emu/emu.h>
#include <emu/emu_shellcode.h>
#include <emu/emu_memory.h>


#define int_ntoa(x)	inet_ntoa(*((struct in_addr *)&x))


struct emu *emu;
struct ip;

char *adres (struct tuple4 addr)
{
	static char buf[256];
	strcpy (buf, int_ntoa (addr.saddr));
	sprintf (buf + strlen (buf), ":%i <-> ", addr.source);
	strcat (buf, int_ntoa (addr.daddr));
	sprintf (buf + strlen (buf), ":%i", addr.dest);
	return buf;
}


void tcp_callback (struct tcp_stream *a_tcp, void ** this_time_not_needed)
{
//	char buf[1024];
//	strcpy (buf, adres (a_tcp->addr)); // we put conn params into buf

	if ( a_tcp->nids_state == NIDS_JUST_EST )
	{
		a_tcp->client.collect++; 
		a_tcp->client.collect_urg++; 

		a_tcp->server.collect++; 						  
		a_tcp->server.collect_urg++; 

//		fprintf (stderr, "%s established\n", buf);
	}/* else
	if ( a_tcp->nids_state == NIDS_CLOSE )
	{
		fprintf (stderr, "%s closing\n", buf);
	}else
	if ( a_tcp->nids_state == NIDS_RESET )
	{
		fprintf (stderr, "%s reset\n", buf);
	} */ else
	if ( a_tcp->nids_state == NIDS_DATA )
	{

		struct half_stream *hlf;
		char *data;
		int size;

		if ( a_tcp->server.count_new_urg || a_tcp->server.count_new )
		{
			hlf = &a_tcp->server;
		}else
		if ( a_tcp->client.count_new_urg ||  a_tcp->client.count_new )
		{
			hlf = &a_tcp->client;
		}else
		{
			return;
		}

        size = hlf->count - hlf->offset;
		data = hlf->data;

//		printf("size is %i\n", size);
//		printf("count %i offset %i size %i\n",hlf->count, hlf->offset, size);

		if ( emu_shellcode_test(emu, (uint8_t *)data, size) >= 0 )
		{
			fprintf(stderr, "suspecting shellcode in connection %s\n", adres(a_tcp->addr));
		}

		if (size > 2048)
			nids_discard(a_tcp, abs(2048-size));
		
		emu_memory_clear(emu_memory_get(emu));
	}

	return ;
}

void xlog(int type, int err, struct ip *iph, void *data)
{
	char *nids_warnings[] = {
		"Murphy - you never should see this message !",
		"Oversized IP packet",
		"Invalid IP fragment list: fragment over size",
		"Overlapping IP fragments",
		"Invalid IP header",
		"Source routed IP frame",
		"Max number of TCP streams reached",
		"Invalid TCP header",
		"Too much data in TCP receive queue",
		"Invalid TCP flags"
	};

	if (type != NIDS_WARN_SCAN)
	{
    	printf("%s\n", nids_warnings[err]);
	}
}

int main (int argc, const char *argv[])
{
	if (argc == 1)
	{
		printf("useage %s <device>\n", argv[0]);
		return -1;
	}else
	{
		printf("listening on device %s\n", argv[1]);
	}

	emu = emu_new();


	nids_params.device = strdup(argv[1]);;
	nids_params.syslog = xlog;
	if ( !nids_init () )
	{
		fprintf(stderr,"%s\n",nids_errbuf);
		exit(1);
	}

	// disable checksumming as host may calculate the checksum on the nic in hardware
	struct nids_chksum_ctl disable_checksums;
	disable_checksums.netaddr = 0;
	disable_checksums.mask    = 0;
	disable_checksums.action  = NIDS_DONT_CHKSUM;

	nids_register_chksum_ctl(&disable_checksums,1);

	nids_register_tcp (tcp_callback);
	nids_run ();


	emu_free(emu);
	return 0;
}

