#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/select.h>
#include <unistd.h>
#include <stdio.h>

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <getopt.h>


#include <emu/emu.h>
#include <emu/emu_memory.h>
#include <emu/emu_cpu.h>
#include <emu/emu_log.h>

#define FAILED "\033[31;1mfailed\033[0m"
#define SUCCESS "\033[32;1msuccess\033[0m"

static const char *regm[] = {
	"eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi"
};


struct instr_test
{
	const char *instr;

	uint8_t  *code;
	uint16_t codesize;

	struct 
	{
		uint32_t reg[8];
	} start;

	struct 
	{
		uint32_t reg[8];
		uint32_t		mem_state_0[2];
	}stopp;
};


struct instr_test tests[] = 
{
	{
		.instr = "add [ebx],bh",
		.start.reg  = {0,0,0,0,0,0,0,0 },
	},
	{
		.instr = "add cx,ax",
		.start.reg  = {0,0,0,0,0,0,0,0 },
	},
	{
		.instr = "add ebx,ecx",
		.start.reg  = {0,0,0,0,0,0,0,0 },
	}
};

int prepare()
{
	int i;
	for (i=0;i<sizeof(tests)/sizeof(struct instr_test);i++)
	{
		const char *use = "USE32\n";
		FILE *f=fopen("/tmp/foo.S","w+");

		fwrite(use,strlen(use),1,f);
		fwrite(tests[i].instr,1,strlen(tests[i].instr),f);
		fclose(f);
		system("cd /tmp/; nasm foo.S");
		f=fopen("/tmp/foo","r");
		fseek(f,0,SEEK_END);

		tests[i].codesize = ftell(f);
		tests[i].code = malloc(tests[i].codesize);
		fseek(f,0,SEEK_SET);
		fread(tests[i].code,1,tests[i].codesize,f);
		fclose(f);

		unlink("/tmp/foo.S");
		unlink("/tmp/foo");
	}
	return 0;
}


int test()
{
	int i=0;

	for (i=0;i<sizeof(tests)/sizeof(struct instr_test);i++)
	{
		printf("testing '%s'\n",tests[i].instr);
		printf("\t code '");
		int j=0;
		for (j=0;j<tests[i].codesize;j++)
		{
			printf("%02x ",tests[i].code[j]);
		}
		printf("'\n");

		struct emu *e = emu_new();
		struct emu_cpu *cpu = emu_cpu_get(e);

		// for i in eax  ecx edx ebx esp ebp esi edi; do echo "emu_cpu_reg32_set(cpu, $i, tests[i].start.$i );" ; done
		for ( j=0;j<8;j++ )
		{
			emu_cpu_reg32_set(cpu,j ,tests[i].start.reg[j]);
		}
   	


		struct emu_memory *mem = emu_memory_get(e);
		int static_offset = 4711;
		for( j = 0; j < tests[i].codesize; j++ )
		{
			emu_memory_write_byte(mem, static_offset+j, tests[i].code[j]);
		}
		emu_memory_write_byte(mem, static_offset+i, '\xcc');
		emu_cpu_eip_set(emu_cpu_get(e), static_offset);
		emu_cpu_run(emu_cpu_get(e));

		// for i in eax  ecx edx ebx esp ebp esi edi; do echo "if (emu_cpu_reg32_get(cpu, $i) ==  tests[i].stopp.$i ) { printf(\"\t $i \"SUCCESS); } else { printf(\"\t $i "FAILED" %i expected %i\n\",emu_cpu_reg32_get(cpu, $i),tests[i].stopp.$i); }" ; done

		for ( j=0;j<8;j++ )
		{
			if ( emu_cpu_reg32_get(cpu, j) ==  tests[i].stopp.reg[j] )
			{
				printf("\t %s "SUCCESS"\n",regm[j]);
			} else
			{
				printf("\t %s "FAILED" got %i expected %i\n",regm[j],emu_cpu_reg32_get(cpu, j),tests[i].stopp.reg[j]);
			}
		}
	}
	return 0;
}

int main()
{
	prepare();
	test();

}
