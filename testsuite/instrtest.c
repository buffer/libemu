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

#define FAILED "\033[31;1mfailed\033[0m\n"
#define SUCCESS "\033[32;1msuccess\033[0m\n"


struct instr_test
{
	const char *instr;

	uint8_t  *code;
	uint16_t codesize;

	struct 
	{
		uint32_t eax , ecx, edx, ebx, esp, ebp, esi, edi;
	} start;

	struct 
	{
		uint32_t eax , ecx, edx, ebx, esp, ebp, esi, edi;
		uint32_t		mem_state_0[2];
	}stopp;
};


struct instr_test tests[] = 
{
	{
		.instr = "add [ebx],bh",
		.start.eax = 0,
	},
	{
		.instr = "add cx,ax",
		.start.eax = 0,
	},
	{
		.instr = "add ebx,ecx",
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
		emu_cpu_reg32_set(cpu, eax, tests[i].start.eax );
		emu_cpu_reg32_set(cpu, ecx, tests[i].start.ecx );
		emu_cpu_reg32_set(cpu, edx, tests[i].start.edx );
		emu_cpu_reg32_set(cpu, ebx, tests[i].start.ebx );
		emu_cpu_reg32_set(cpu, esp, tests[i].start.esp );
		emu_cpu_reg32_set(cpu, ebp, tests[i].start.ebp );
		emu_cpu_reg32_set(cpu, esi, tests[i].start.esi );
		emu_cpu_reg32_set(cpu, edi, tests[i].start.edi );
   	


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

		if ( emu_cpu_reg32_get(cpu, eax) ==  tests[i].stopp.eax )
		{
			printf("\t eax "SUCCESS);
		} else
		{
			printf("\t eax FAILED %i expected %i\n",emu_cpu_reg32_get(cpu, eax),tests[i].stopp.eax);
		}
		if ( emu_cpu_reg32_get(cpu, ecx) ==  tests[i].stopp.ecx )
		{
			printf("\t ecx "SUCCESS);
		} else
		{
			printf("\t ecx FAILED %i expected %i\n",emu_cpu_reg32_get(cpu, ecx),tests[i].stopp.ecx);
		}
		if ( emu_cpu_reg32_get(cpu, edx) ==  tests[i].stopp.edx )
		{
			printf("\t edx "SUCCESS);
		} else
		{
			printf("\t edx FAILED %i expected %i\n",emu_cpu_reg32_get(cpu, edx),tests[i].stopp.edx);
		}
		if ( emu_cpu_reg32_get(cpu, ebx) ==  tests[i].stopp.ebx )
		{
			printf("\t ebx "SUCCESS);
		} else
		{
			printf("\t ebx FAILED %i expected %i\n",emu_cpu_reg32_get(cpu, ebx),tests[i].stopp.ebx);
		}
		if ( emu_cpu_reg32_get(cpu, esp) ==  tests[i].stopp.esp )
		{
			printf("\t esp "SUCCESS);
		} else
		{
			printf("\t esp FAILED %i expected %i\n",emu_cpu_reg32_get(cpu, esp),tests[i].stopp.esp);
		}
		if ( emu_cpu_reg32_get(cpu, ebp) ==  tests[i].stopp.ebp )
		{
			printf("\t ebp "SUCCESS);
		} else
		{
			printf("\t ebp FAILED %i expected %i\n",emu_cpu_reg32_get(cpu, ebp),tests[i].stopp.ebp);
		}
		if ( emu_cpu_reg32_get(cpu, esi) ==  tests[i].stopp.esi )
		{
			printf("\t esi "SUCCESS);
		} else
		{
			printf("\t esi FAILED %i expected %i\n",emu_cpu_reg32_get(cpu, esi),tests[i].stopp.esi);
		}
		if ( emu_cpu_reg32_get(cpu, edi) ==  tests[i].stopp.edi )
		{
			printf("\t edi "SUCCESS);
		} else
		{
			printf("\t edi FAILED %i expected %i\n",emu_cpu_reg32_get(cpu, edi),tests[i].stopp.edi);
		}

	}
	return 0;
}

int main()
{
	prepare();
	test();

}
