#include <string.h>
#include <stdio.h>

#include "nanny.h"

#include "../../include/emu/emu_hashtable.h"


struct nanny *nanny_new(void)
{
	struct nanny *na = malloc(sizeof(struct nanny));
	memset(na, 0, sizeof(struct nanny));

	na->files = emu_hashtable_new(16, emu_hashtable_ptr_hash, emu_hashtable_ptr_cmp);

	return na;
}

struct nanny_file *nanny_add_file(struct nanny *na, const char *path, uint32_t *emu_file, FILE *real_file)
{
	struct nanny_file *file = malloc(sizeof(struct nanny_file));
	memset(file, 0, sizeof(struct nanny_file));

	*emu_file = rand();

	file->path = strdup(path);
	file->emu_file = *emu_file;
	file->real_file = real_file;

	emu_hashtable_insert(na->files, (void *)(uintptr_t)file->emu_file, file);

	return file;
}

struct nanny_file *nanny_get_file(struct nanny *na, uint32_t emu_file)
{
	 struct emu_hashtable_item *item = emu_hashtable_search(na->files, (void *)(uintptr_t)emu_file);
	 if (item != NULL)
	 {
		 struct nanny_file *file = item->value;
		 return file;
	 }else
		 return NULL;
	 
}

bool nanny_del_file(struct nanny *na, uint32_t emu_file)
{
	struct emu_hashtable_item *item = emu_hashtable_search(na->files, (void *)(uintptr_t)emu_file);
	if (item != NULL)
	{
		free(item->value);
	}
	return emu_hashtable_delete(na->files, (void *)(uintptr_t)emu_file);
}

void nanny_free(struct nanny *nanny)
{

}
