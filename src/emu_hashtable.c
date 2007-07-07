/* @header@ */

#include <string.h>

#include "emu/emu_hashtable.h"



source_list_functions(emu_hashtable_bucket_items,emu_hashtable_bucket_item_root, emu_hashtable_bucket_item, link);




struct emu_hashtable *emu_hashtable_new(uint32_t size, 
											   emu_hashtable_hash_cb hash, 
											   emu_hashtable_cmp_cb cmp)
{
	
	struct emu_hashtable *eh = (struct emu_hashtable *)malloc(sizeof(struct emu_hashtable));
	if( eh == NULL )
	{
		return NULL;
	}
	memset(eh, 0, sizeof(struct emu_hashtable));

	eh->size = size;
	eh->hash = hash;
	eh->cmp = cmp;

	eh->buckets = (struct emu_hashtable_bucket **) malloc(sizeof(struct emu_hashtable_bucket *) * size);
	memset(eh->buckets, 0, sizeof(struct emu_hashtable_bucket *) * size);
	
	return eh;
}

void emu_hashtable_free(struct emu_hashtable *eh)
{
	uint32_t i;
	struct emu_hashtable_bucket *ehb;
	struct emu_hashtable_bucket_item *ehbi;

	for (i=0;i<eh->size;i++)
	{
		if ((ehb = eh->buckets[i]) != NULL)
		{
			for (ehbi = emu_hashtable_bucket_items_first(ehb->items); !emu_hashtable_bucket_items_attail(ehbi); ehbi = emu_hashtable_bucket_items_next(ehbi))
			{
				if (eh->key_destructor != NULL)
				{
					eh->key_destructor(ehbi->item.key);
				}
				if (eh->value_destructor != NULL)
				{
					eh->value_destructor(ehbi->item.value);
				}
			}
	
			emu_hashtable_bucket_free(ehb);
		}
	}

	free(eh->buckets);
	free(eh);
}

struct emu_hashtable_item *emu_hashtable_search(struct emu_hashtable *eh, void *key)
{
	uint32_t first_hash = eh->hash(key) % eh->size;

	struct emu_hashtable_bucket *ehb = 	eh->buckets[first_hash];
	if (ehb == NULL)
		return NULL;

	struct emu_hashtable_item *ehi;
	struct emu_hashtable_bucket_item *ehbi;

	for (ehbi = emu_hashtable_bucket_items_first(ehb->items); !emu_hashtable_bucket_items_attail(ehbi); ehbi = emu_hashtable_bucket_items_next(ehbi))
	{
		ehi = &ehbi->item;
		if (eh->cmp(ehi->key, key) == true)
			return ehi;
	}

	return NULL;
}

struct emu_hashtable_item *emu_hashtable_insert(struct emu_hashtable *eh, void *key, void *data)
{
	struct emu_hashtable_item *ehi;
	if ((ehi = emu_hashtable_search(eh, key)) == NULL)
	{
		struct emu_hashtable_bucket_item *ehbi = emu_hashtable_bucket_item_new(key, data);
		ehi = &ehbi->item;

		uint32_t first_hash = eh->hash(key) % eh->size;

		struct emu_hashtable_bucket *ehb;
		if ((ehb = eh->buckets[first_hash]) == NULL)
		{
			ehb = emu_hashtable_bucket_new();
			eh->buckets[first_hash] = ehb;
		}

		emu_hashtable_bucket_items_insert_last(ehb->items,ehbi);
	}else
	{
		ehi->value = data;
	}
	
	return ehi;
}

bool emu_hashtable_delete(struct emu_hashtable *eh, void *key)
{
	uint32_t first_hash = eh->hash(key) % eh->size;

	struct emu_hashtable_bucket *ehb;
	if ((ehb = eh->buckets[first_hash]) != NULL)
	{
		struct emu_hashtable_bucket_item *ehbi;

		for (ehbi = emu_hashtable_bucket_items_first(ehb->items); !emu_hashtable_bucket_items_attail(ehbi); ehbi = emu_hashtable_bucket_items_next(ehbi))
		{
			struct emu_hashtable_item *ehi;
			ehi = &ehbi->item;
			if (eh->cmp(ehi->key, key) == true)
			{
				if (eh->value_destructor != NULL)
                	eh->value_destructor(ehi->value);

				if (eh->key_destructor != NULL)
					eh->key_destructor(ehi->key);
				
				emu_hashtable_bucket_items_remove(ehbi);
				return true;
			}
		}
	}

	return false;
}



struct emu_hashtable_bucket *emu_hashtable_bucket_new()
{
	struct emu_hashtable_bucket *ehb = (struct emu_hashtable_bucket *)malloc(sizeof(struct emu_hashtable_bucket));
	if( ehb == NULL )
	{
		return NULL;
	}
	memset(ehb, 0, sizeof(struct emu_hashtable_bucket));
	ehb->items = emu_hashtable_bucket_items_create();
	return ehb;
}

void emu_hashtable_bucket_free(struct emu_hashtable_bucket *ehb)
{
	emu_hashtable_bucket_items_destroy(ehb->items);
	free(ehb);
}

struct emu_hashtable_bucket_item *emu_hashtable_bucket_item_new(void *key, void *value)
{
	struct emu_hashtable_bucket_item *ehbi = (struct emu_hashtable_bucket_item *)malloc(sizeof(struct emu_hashtable_bucket_item));
	if( ehbi == NULL )
	{
		return NULL;
	}
	memset(ehbi, 0, sizeof(struct emu_hashtable_bucket_item));

	emu_hashtable_bucket_items_init_link(ehbi);

	return ehbi;
}
