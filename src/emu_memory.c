/* @header@ */
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include "emu/emu.h"
#include "emu/emu_log.h"
#include "emu/emu_memory.h"
#include "emu/emu_string.h"


#define PAGE_BITS 12 /* size of one page, 2^12 = 4096 */
#define PAGESET_BITS 10 /* number of pages in one pageset, 2^10 = 1024 */


#define PAGE_SIZE (1 << PAGE_BITS)
#define PAGESET_SIZE (1 << PAGESET_BITS)

#define PAGESET(x) ((x) >> (PAGESET_BITS + PAGE_BITS))
#define PAGE(x) (((x) >> PAGE_BITS) & ((1 << PAGESET_BITS) - 1))
#define OFFSET(x) (((1 << PAGE_BITS) - 1) & (x))

#define FS_SEGMENT_DEFAULT_OFFSET 0x7ffdf000



struct emu_memory
{
	struct emu *emu;
	void ***pagetable;
	
	uint32_t segment_offset;
	enum emu_segment segment_current;
	
	uint32_t segment_table[6];
};

#if 1
/*static void emu_memory_debug_pagetable(struct emu_memory *m)
{
	int pagesets = 1 << (32 - PAGESET_BITS - PAGE_BITS);
	int pagesets_used = 0;
	
	printf("*** memory debug\n");
	
	int i, j;
	
	for( i = 0; i < pagesets; i++ )
	{
		if( m->pagetable[i] != NULL )
		{
			printf("  pageset %d allocated at 0x%08x\n", i, (int)m->pagetable[i]);
			int pages = 1 << (PAGESET_BITS);
			int pages_used = 0;

			for( j = 0; j < pages; j++ )
			{
				if( m->pagetable[i][j] != NULL )
				{
					printf("    page %d allocated at 0x%08x\n", j, (int)m->pagetable[i][j]);
					pages_used++;
				}
			}
			
			printf("    %d/%d pages used\n", pages_used, pages);
			
			pagesets_used++;
		}
	}
	
	printf("  %d/%d pagesets used\n", pagesets_used, pagesets);
	printf("*** end of memory debug\n");
}

static void emu_memory_debug_addr(uint32_t addr)
{
	printf("addr 0x%08x, pageset 0x%08x, page 0x%08x, offset 0x%08x\n",
		addr, PAGESET(addr), PAGE(addr), OFFSET(addr));
}*/
#endif

uint32_t emu_memory_get_usage(struct emu_memory *m)
{
	uint32_t usage = (1 << (32 - PAGE_BITS - PAGESET_BITS)) * sizeof(void *); /* pageset table */
	int pagesets = 1 << (32 - PAGESET_BITS - PAGE_BITS);
	
	int i, j;
	
	for( i = 0; i < pagesets; i++ )
	{
		if( m->pagetable[i] != NULL )
		{
			usage += PAGESET_SIZE * sizeof(void *);
			int pages = 1 << (PAGESET_BITS);

			for( j = 0; j < pages; j++ )
				if( m->pagetable[i][j] != NULL )
					usage += PAGE_SIZE;
		}
	}
	
	return usage;
}

struct emu_memory *emu_memory_new(struct emu *e)
{
	struct emu_memory *em = (struct emu_memory *)malloc(sizeof(struct emu_memory));
	memset(em, 0, sizeof(struct emu_memory));
	
	em->emu = e;
	
	em->pagetable = malloc((1 << (32 - PAGE_BITS - PAGESET_BITS)) * sizeof(void *));
	memset(em->pagetable, 0, (1 << (32 - PAGE_BITS - PAGESET_BITS)) * sizeof(void *));
	
	em->segment_table[s_fs] = FS_SEGMENT_DEFAULT_OFFSET;
	
	return em;
}

void emu_memory_free(struct emu_memory *m)
{
	int i, j;
	
	for( i = 0; i < (1 << (32 - PAGESET_BITS - PAGE_BITS)); i++ )
	{
		if( m->pagetable[i] != NULL )
		{
			for( j = 0; j < PAGESET_SIZE; j++ )
				if( m->pagetable[i][j] != NULL )
					free(m->pagetable[i][j]);
			
			free(m->pagetable[i]);
		}
	}
	
	free(m->pagetable);
	free(m);
}

static inline int page_alloc(struct emu_memory *em, uint32_t addr)
{
	if( em->pagetable[PAGESET(addr)] == NULL )
	{
		em->pagetable[PAGESET(addr)] = malloc(PAGESET_SIZE * sizeof(void *));
		memset(em->pagetable[PAGESET(addr)], 0, PAGESET_SIZE * sizeof(void *));
	}

	em->pagetable[PAGESET(addr)][PAGE(addr)] = malloc(PAGE_SIZE);

	return 0;
}

static inline void *translate_addr(struct emu_memory *em, uint32_t addr)
{
	if( em->pagetable[PAGESET(addr)] != NULL )
	{
		if( em->pagetable[PAGESET(addr)][PAGE(addr)] != NULL )
		{
			return em->pagetable[PAGESET(addr)][PAGE(addr)] + OFFSET(addr);
		}
	}
	
	return NULL;
}

int32_t emu_memory_read_byte(struct emu_memory *m, uint32_t addr, uint8_t *byte)
{
	addr += m->segment_offset;
	void *address = translate_addr(m, addr);
	
	if( address == NULL )
	{
		emu_errno_set(m->emu, EFAULT);
		emu_strerror_set(m->emu, "error accessing 0x%08x not mapped\n", addr);
		return -1;
	}
	
	*byte = *((uint8_t *)address);
	
	return 0;
}

int32_t emu_memory_read_word(struct emu_memory *m, uint32_t addr, uint16_t *word)
{
	return emu_memory_read_block(m, addr, word, 2);
}

int32_t emu_memory_read_dword(struct emu_memory *m, uint32_t addr, uint32_t *dword)
{
	return emu_memory_read_block(m, addr, dword, 4);
}

int32_t emu_memory_read_block(struct emu_memory *m, uint32_t addr, void *dest, size_t len)
{
	uint32_t oaddr = addr; /* save original addr for recursive call */
	addr += m->segment_offset;
	
	void *address = translate_addr(m, addr);
	
	if( address == NULL )
	{
		emu_errno_set(m->emu, EFAULT);
		emu_strerror_set(m->emu, "error accessing 0x%08x not mapped\n", addr);
		return -1;
	}

	if (OFFSET(addr) + len <= PAGE_SIZE)
	{
		memcpy(dest, address, len);
		return 0;
	}
	else
	{
		uint32_t cb = PAGE_SIZE - OFFSET(addr);
		memcpy(dest, address, cb);
		return emu_memory_read_block(m, oaddr + cb, dest + cb, len - cb);
	}
}

int32_t emu_memory_read_string(struct emu_memory *m, uint32_t addr, struct emu_string *s, uint32_t maxsize)
{
	uint32_t i = 0;
	
	void *address;
	
	while( 1 )
	{
		if (i > maxsize - 1)
			return -1;
		
		address = translate_addr(m, addr + i);

		if( address == NULL )
			return -1;

		if( *(uint8_t *)address == '\0' )
			break;

		i++;
	}

	s->data = malloc(i + 1);
	memset(s->data, 0, i + 1);
	s->size = i;

	return emu_memory_read_block(m, addr, s->data, i);
}


int32_t emu_memory_write_byte(struct emu_memory *m, uint32_t addr, uint8_t byte)
{
	addr += m->segment_offset;

	void *address = translate_addr(m, addr);
	
	if( address == NULL )
	{
		if( page_alloc(m, addr) == -1 )
			return -1;

		address = translate_addr(m, addr);
	}
	
	*((uint8_t *)address) = byte;
	
	return 0;
}

int32_t emu_memory_write_word(struct emu_memory *m, uint32_t addr, uint16_t word)
{
	return emu_memory_write_block(m, addr, &word, 2);
}

int32_t emu_memory_write_dword(struct emu_memory *m, uint32_t addr, uint32_t dword)
{
	return emu_memory_write_block(m, addr, &dword, 4);
}

int32_t emu_memory_write_block(struct emu_memory *m, uint32_t addr, void *src, size_t len)
{
	uint32_t oaddr = addr; /* save original addr for recursive call */
	addr += m->segment_offset;

	void *address = translate_addr(m, addr);

	if( address == NULL )
	{
		if( page_alloc(m, addr) == -1 )
			return -1;

		address = translate_addr(m, addr);
	}

	if (OFFSET(addr) + len <= PAGE_SIZE)
	{
		memcpy(address, src, len);
		return 0;
	}
	else
	{
		uint32_t cb = PAGE_SIZE - OFFSET(addr);
		memcpy(address, src, cb);
		return emu_memory_write_block(m, oaddr + cb, src + cb, len - cb);
	}

	return 0;

}

void emu_memory_segment_select(struct emu_memory *m, enum emu_segment s)
{
	m->segment_current = s;
	m->segment_offset = m->segment_table[m->segment_current];
}

enum emu_segment emu_memory_segment_get(struct emu_memory *m)
{
	return m->segment_current;
}


int32_t emu_memory_alloc(struct emu_memory *m, uint32_t *addr, size_t len)
{
	*addr = rand();
	return 0;
}
