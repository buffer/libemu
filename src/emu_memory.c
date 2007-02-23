/* @header@ */
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include <emu/emu.h>
#include <emu/emu_log.h>
#include <emu/emu_memory.h>

#define PAGE_BITS 12 /* 2^12 = 4096 */
#define PAGE_SIZE (1 << 12)

#define PAGE(x) ((x) >> PAGE_BITS)
#define OFFSET(x) (((1 << PAGE_BITS) - 1) & (x))

/*struct page
{
	void *data;
	uint32_t virtual_address;
	struct page *next;
};*/

struct emu_memory
{
	struct emu *emu;
/*	enum emu_allocation_mode alloc_mode;*/
/*	struct page *pages;*/
	void **page_map;
};

struct emu_memory *emu_memory_new(struct emu *e)
{
	struct emu_memory *em = (struct emu_memory *)malloc(sizeof(struct emu_memory));
	memset(em, 0, sizeof(struct emu_memory));
	
/*	printf("memory page size is %d bytes\n", PAGE_SIZE);*/
	
	em->emu = e;
/*	em->alloc_mode = m;
	
	if( m == alloc_mode_map )
	{*/
		em->page_map = malloc((1 << (32 - PAGE_BITS)) * sizeof(void *));
		memset(em->page_map, 0, (1 << (32 - PAGE_BITS)) * sizeof(void *));
/*	}*/
	
	return em;
}

void emu_memory_free(struct emu_memory *em)
{
	int i;
	for (i=0; i < (1 << (32 - PAGE_BITS));i++)
	{
		if (em->page_map[i] != NULL)
        	free(em->page_map[i]);
		
	}
	free(em->page_map);
	free(em);
}

/*static void *translate_addr_list(struct emu_memory *em, uint32_t addr)
{
	struct page *p = em->pages;
	
	while( p != NULL )
	{
		if( p->virtual_address == PAGE(addr) )
			return p + OFFSET(addr);
			
		p = p->next;
	}
	
	return NULL; 
}*/

static inline int page_alloc(struct emu_memory *em, uint32_t addr)
{
	em->page_map[PAGE(addr)] = malloc(PAGE_SIZE);
	
	/*printf("page 0x%08x maps to 0x%08x\n", PAGE(addr), (uint32_t)em->page_map[PAGE(addr)]);*/
	if ( em->page_map[PAGE(addr)] == NULL )
	{
		emu_errno_set(em->emu, errno);
		return -1;
	} else
		return 0;
}

static inline void *translate_addr(struct emu_memory *em, uint32_t addr)
{
	/*printf("va 0x%08x maps to page 0x%08x with offset 0x%08x, effective addr 0x%08x\n",
		addr, PAGE(addr), OFFSET(addr), (uint32_t)(em->page_map[PAGE(addr)] + OFFSET(addr)));*/
	return em->page_map[PAGE(addr)] + OFFSET(addr);
}

int32_t emu_memory_read_byte(struct emu_memory *m, uint32_t addr, uint8_t *byte)
{
	if( m->page_map[PAGE(addr)] == NULL )
	{
		emu_errno_set(m->emu, EFAULT);
		emu_strerror_set(m->emu,"error accessing 0x%08x not mapped\n",addr);
		return -1;
	}

	void *address = translate_addr(m, addr);
	
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
	if( m->page_map[PAGE(addr)] == NULL )
	{
		emu_errno_set(m->emu, EFAULT);
		emu_strerror_set(m->emu,"error accessing 0x%08x not mapped\n",addr);
		return -1;
	}

	if (OFFSET(addr) + len <= PAGE_SIZE)
	{
		void *address = translate_addr(m, addr);
		memcpy(dest, address, len);
		return 0;
	}else
	{
		void *address = translate_addr(m, addr);
		uint32_t cb = PAGE_SIZE - OFFSET(addr);
		memcpy(dest, address, cb);
		return emu_memory_read_block(m, addr + cb, dest + cb, len - cb);
	}
}


int32_t emu_memory_write_byte(struct emu_memory *m, uint32_t addr, uint8_t byte)
{
	if( m->page_map[PAGE(addr)] == NULL )
		if( page_alloc(m, addr) == -1 )
			return -1;

	void *address = translate_addr(m, addr);
	
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
	if( m->page_map[PAGE(addr)] == NULL )
		if( page_alloc(m, addr) == -1 )
			return -1;

	if (OFFSET(addr) + len <= PAGE_SIZE)
	{
		void *address = translate_addr(m, addr);
		memcpy(address, src, len);
		return 0;
	}else
	{
		void *address = translate_addr(m, addr);
		uint32_t cb = PAGE_SIZE - OFFSET(addr);
		memcpy(address, src, cb);
		return emu_memory_write_block(m, addr + cb, src + cb, len - cb);
	}

	return 0;

}

