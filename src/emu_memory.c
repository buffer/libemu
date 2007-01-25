#include <string.h>
#include <stdlib.h>

#include <emu/emu.h>
#include <emu/log.h>
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
	struct emu *ctx;
/*	enum emu_allocation_mode alloc_mode;*/
/*	struct page *pages;*/
	void **page_map;
};

struct emu_memory *emu_memory_new(struct emu *e)
{
	struct emu_memory *em = (struct emu_memory *)malloc(sizeof(struct emu_memory));
	memset(em, 0, sizeof(struct emu_memory));
	
/*	printf("memory page size is %d bytes\n", PAGE_SIZE);*/
	
	em->ctx = e;
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
	if( em->page_map[PAGE(addr)] == NULL )
		return -1;
	else
		return 0;
}

static inline void *translate_addr(struct emu_memory *em, uint32_t addr)
{
	/*printf("va 0x%08x maps to page 0x%08x with offset 0x%08x, effective addr 0x%08x\n",
		addr, PAGE(addr), OFFSET(addr), (uint32_t)(em->page_map[PAGE(addr)] + OFFSET(addr)));*/
	return em->page_map[PAGE(addr)] + OFFSET(addr);
}

uint32_t emu_memory_read_byte(struct emu_memory *m, uint32_t addr, uint8_t *byte)
{
	void *address = translate_addr(m, addr);
	
	if( address == NULL )
		return -1;
	
	*byte = *((uint8_t *)address);		
	
	return 0;
}

uint32_t emu_memory_write_byte(struct emu_memory *m, uint32_t addr, uint8_t byte)
{
	if( m->page_map[PAGE(addr)] == NULL )
		if( page_alloc(m, addr) == -1 )
			return -1;

	void *address = translate_addr(m, addr);
	
	*((uint8_t *)address) = byte;
	
	return 0;
}

