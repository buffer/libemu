#include <stdio.h>
#include <string.h>

#include <emu/emu_hashtable.h>

struct addr_instr
{
	uint32_t addr;
	const char *instr;
};


struct addr_instr insert_data[] = 
{
	{ 0x0012fc90, "sub esp,0x20"},
	{ 0x0012fc93, "mov ebp,esp"},
	{ 0x0012fc95, "mov [ebp+0x4],ebx"},
	{ 0x0012fc98, "mov [ebp+0x0],edi"},
	{ 0x0012fc9b, "sub esp,0x200"},
	{ 0x0012fca1, "mov [ebp+0x14],esp"},
	{ 0x0012fca4, "xor ebx,ebx"},
	{ 0x0012fca6, "mov eax,fs:[ebx+0x30]"},
	{ 0x0012fcaa, "mov eax,[eax+0xc]"},
	{ 0x0012fcad, "mov esi,[eax+0x1c]"},
	{ 0x0012fcb0, "lodsd "},
	{ 0x0012fcb1, "mov edi,[eax+0x8]"},
	{ 0x0012fcb4, "mov [ebp+0x8],edi"},
	{ 0x0012fcb7, "call 0x4a"},
	{ 0x0012fcbc, "push ebx"},
	{ 0x0012fcbd, "push esi"},
	{ 0x0012fcbe, "mov ebx,[edi+0x3c]"},
	{ 0x0012fcc1, "mov ebx,[ebx+edi+0x78]"},
	{ 0x0012fcc5, "add ebx,edi"},
	{ 0x0012fcc7, "push ebx"},
	{ 0x0012fcc8, "mov ebx,[ebx+0x20]"},
	{ 0x0012fccb, "add ebx,edi"},
	{ 0x0012fccd, "push ebx"},
	{ 0x0012fcce, "add ebx,0x4"},
	{ 0x0012fcd1, "mov esi,[ebx]"},
	{ 0x0012fcd3, "add esi,edi"},
	{ 0x0012fcd5, "xor ecx,ecx"},
	{ 0x0012fcd7, "lodsb "},
	{ 0x0012fcd8, "xor cl,al"},
	{ 0x0012fcda, "rol ecx,0x5"},
	{ 0x0012fcdd, "test al,al"},
	{ 0x0012fcdf, "jnz 0xfffffff8"},
	{ 0x0012fce1, "sub ecx,edx"},
	{ 0x0012fce3, "jnz 0xffffffeb"},
	{ 0x0012fd01, "pop esi"},
	{ 0x0012fd02, "push dword"},
	{ 0x0012fd07, "push dword"},
	{ 0x0012fd0c, "push esp"},
	{ 0x0012fd0d, "mov edx,0x84046e92"},
	{ 0x0012fd12, "call esi"},
	{ 0x00402002, "nop "},
	{ 0x00402003, "nop "},
	{ 0x00402004, "nop "},
	{ 0x00402005, "jmp 0x6"},
	{ 0x0040200b, "nop "},
	{ 0x0040200c, "nop "},
	{ 0x0040200d, "nop "},
	{ 0x0040200e, "nop "},
	{ 0x0040200f, "nop "},
	{ 0x00402010, "nop "},
	{ 0x00402011, "nop "},
	{ 0x00402012, "nop "},
	{ 0x00402013, "jmp 0x6"},
	{ 0x00402019, "nop "},
	{ 0x0040201a, "nop "},
	{ 0x0040201b, "jmp 0x6"},
	{ 0x00402021, "nop "},
	{ 0x00402022, "nop "},
	{ 0x00402023, "nop "},
	{ 0x00402024, "nop "},
	{ 0x00402025, "nop "},
	{ 0x00402026, "nop "},
	{ 0x00402027, "nop "},
	{ 0x00402028, "nop "},
	{ 0x00402029, "nop "},
	{ 0x0040202a, "nop "},
	{ 0x0040202b, "nop "},
	{ 0x0040202c, "nop "},
	{ 0x0040202d, "nop "},
	{ 0x0040202e, "nop "},
	{ 0x0040202f, "nop "},
	{ 0x00402030, "nop "},
	{ 0x00402031, "nop "},
	{ 0x00402032, "nop "},
	{ 0x00402033, "nop "},
	{ 0x00402034, "nop "},
	{ 0x00402035, "jmp 0x17"},
	{ 0x00402037, "mov ecx,0x4113e68b"},
	{ 0x0040203c, "xor ecx,0x4113e64d"},
	{ 0x00402042, "pop esi"},
	{ 0x00402043, "xor byte"},
	{ 0x00402048, "loop 0xfffffffb"},
	{ 0x0040204a, "jmp 0x7"},
	{ 0x0040204c, "call 0xffffffeb"},
	{ 0x00402051, "xor ebx,ebx"},
	{ 0x00402053, "mov eax,fs:[ebx+0x30]"},
	{ 0x00402057, "mov eax,[eax+0xc]"},
	{ 0x0040205a, "mov esi,[eax+0x1c]"},
	{ 0x0040205d, "lodsd "},
	{ 0x0040205e, "mov edi,[eax+0x8]"},
	{ 0x00402061, "call 0x4a"},
	{ 0x00402066, "push ebx"},
	{ 0x00402067, "push esi"},
	{ 0x00402068, "mov ebx,[edi+0x3c]"},
	{ 0x0040206b, "mov ebx,[ebx+edi+0x78]"},
	{ 0x0040206f, "add ebx,edi"},
	{ 0x00402071, "push ebx"},
	{ 0x00402072, "mov ebx,[ebx+0x20]"},
	{ 0x00402075, "add ebx,edi"},
	{ 0x00402077, "push ebx"},
	{ 0x00402078, "add ebx,0x4"},
	{ 0x0040207b, "mov esi,[ebx]"},
	{ 0x0040207d, "add esi,edi"},
	{ 0x0040207f, "xor ecx,ecx"},
	{ 0x00402081, "lodsb "},
	{ 0x00402082, "xor cl,al"},
	{ 0x00402084, "rol ecx,0x5"},
	{ 0x00402087, "test al,al"},
	{ 0x00402089, "jnz 0xfffffff8"},
	{ 0x0040208b, "sub ecx,edx"},
	{ 0x0040208d, "jnz 0xffffffeb"},
	{ 0x0040208f, "pop eax"},
	{ 0x00402090, "sub ebx,eax"},
	{ 0x00402092, "shr ebx,0x1"},
	{ 0x00402094, "pop esi"},
	{ 0x00402095, "add ebx,[esi+0x24]"},
	{ 0x00402098, "add ebx,edi"},
	{ 0x0040209a, "mov cx,[ebx]"},
	{ 0x0040209d, "mov ebx,[esi+0x1c]"},
	{ 0x004020a0, "add ebx,edi"},
	{ 0x004020a2, "mov eax,[ebx+ecx*4]"},
	{ 0x004020a5, "add eax,edi"},
	{ 0x004020a7, "pop esi"},
	{ 0x004020a8, "pop ebx"},
	{ 0x004020a9, "jmp eax"},
	{ 0x004020ab, "pop esi"},
	{ 0x004020ac, "push dword"},
	{ 0x004020b1, "push dword"},
	{ 0x004020b6, "push esp"},
	{ 0x004020b7, "mov edx,0x84046e92"},
	{ 0x004020bc, "call esi"},
	{ 0x004020be, "mov edi,eax"},
	{ 0x004020c0, "sub esp,0x200"},
	{ 0x004020c6, "mov ebp,esp"},
	{ 0x004020c8, "push ebx"},
	{ 0x004020c9, "push byte"},
	{ 0x004020cb, "push byte"},
	{ 0x004020cd, "mov edx,0x835383"},
	{ 0x004020d2, "call esi"},
	{ 0x004020d4, "push ebx"},
	{ 0x004020d5, "push ebx"},
	{ 0x004020d6, "push ebx"},
	{ 0x004020d7, "push dword"},
	{ 0x004020dc, "mov edx,esp"},
	{ 0x004020de, "mov ebx,eax"},
	{ 0x004020e0, "push byte"},
	{ 0x004020e2, "push edx"},
	{ 0x004020e3, "push ebx"},
	{ 0x004020e4, "mov edx,0xc2a69000"},
	{ 0x004020e9, "call esi"},
	{ 0x004020eb, "inc eax"},
	{ 0x004020ec, "push eax"},
	{ 0x004020ed, "push ebx"},
	{ 0x004020ee, "mov edx,0xa1733b7a"},
	{ 0x004020f3, "call esi"},
	{ 0x004020f5, "push eax"},
	{ 0x004020f6, "push eax"},
	{ 0x004020f7, "push ebx"},
	{ 0x004020f8, "mov edx,0x69d310"},
	{ 0x004020fd, "call esi"},
	{ 0x004020ff, "mov ebx,eax"},
	{ 0x00402101, "xor eax,eax"},
	{ 0x00402103, "push eax"},
	{ 0x00402104, "mov ah,0x2"},
	{ 0x00402106, "push eax"},
	{ 0x00402107, "push ebp"},
	{ 0x00402108, "push ebx"},
	{ 0x00402109, "mov edx,0xe2605800"},
	{ 0x0040210e, "call esi"},
	{ 0x00402110, "mov edi,0x72c6e0d5"},
	{ 0x00402115, "jmp ebp"}
};

struct addr_instr dup_data[] = 
{
	{ 0x0012fc90, "fox"},
	{ 0x0012fc90, "foo"},
	{ 0x0012fc90, "bar"},
	{ 0x0012fc90, "baz"},
	{ 0x0012fc90, "success"}
};


int main(void)
{
	struct emu_hashtable *eh = emu_hashtable_new(4095, emu_hashtable_ptr_hash, emu_hashtable_ptr_cmp);
	int i;
	for (i=0;i<sizeof(insert_data)/sizeof(struct addr_instr);i++)
	{
		emu_hashtable_insert(eh, (void *)(uintptr_t)insert_data[i].addr, (void *)(uintptr_t)insert_data[i].instr);
	}


	struct emu_hashtable_item *ehi;
	for (i=0;i<sizeof(insert_data)/sizeof(struct addr_instr);i++)
	{
		ehi = emu_hashtable_search(eh, (void *)(uintptr_t)insert_data[i].addr);

		if (strcmp(insert_data[i].instr, (char *)ehi->value) != 0 )
		{
			printf("mismatch %x %s %s\n", insert_data[i].addr, insert_data[i].instr, 
				   (char *)(ehi->value));
			return -1;
		}

	}

	for (i=0;i<sizeof(dup_data)/sizeof(struct addr_instr);i++)
	{
		emu_hashtable_insert(eh, (void *)(uintptr_t)dup_data[i].addr, (void *)dup_data[i].instr);
	}

	ehi = emu_hashtable_search(eh, (void *)0x0012fc90);
	if (ehi != NULL)
	{
		printf("%s\n", (char *)ehi->value);
	}

//	printf("success\n");
//	emu_hashtable_free(eh);

//	eh = emu_hashtable_double_new(4095, double_hash1

	return 0;
}
