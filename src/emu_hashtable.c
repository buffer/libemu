/********************************************************************************
 *                               libemu
 *
 *                    - x86 shellcode emulation -
 *
 *
 * Copyright (C) 2007  Paul Baecher & Markus Koetter
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 * 
 * 
 *             contact nepenthesdev@users.sourceforge.net  
 *
 *******************************************************************************/


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

			while ((ehbi = emu_hashtable_bucket_items_remove_first(ehb->items)) != NULL)
			{
				if (eh->key_destructor != NULL)
				{
					eh->key_destructor(ehbi->item.key);
				}
				if (eh->value_destructor != NULL)
				{
					eh->value_destructor(ehbi->item.value);
				}
				emu_hashtable_bucket_item_free(ehbi);
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



struct emu_hashtable_bucket *emu_hashtable_bucket_new(void)
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

	ehbi->item.key = key;
	ehbi->item.value = value;

	emu_hashtable_bucket_items_init_link(ehbi);

	return ehbi;
}


void emu_hashtable_bucket_item_free(struct emu_hashtable_bucket_item *ehbi)
{
	free(ehbi);
}



/**
 * string hashing function to avoid duplicate code
 * algo is djb2 taken from http://www.cse.yorku.ca/~oz/hash.html
 * 
 * @param data   the string to hash
 * 
 * @return the hash
 */
uint32_t emu_hashtable_string_hash(void *key)
{
#ifdef LSOD_HASH
    uint32_t hash = 0;
    char *c = (char *)key;

	while (*c != 0)
	{
		hash = hash << 19 | hash >> 13;
		hash += *c;
		c++;
	}

    return hash;
#else
	unsigned char *str = key;
	unsigned long hash = 5381;
	int c;

	while ( (c = *str++) != 0 )
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

	return hash;
#endif
}

bool emu_hashtable_string_cmp(void *a, void *b)
{
	if ( strcmp(a, b) == 0 )
		return true;

	return false;
}

uint32_t emu_hashtable_ptr_hash(void *key)
{
	uint32_t ukey = (uint32_t)(uintptr_t)key;
	ukey++;
	return ukey;
}

bool emu_hashtable_ptr_cmp(void *a, void *b)
{
	if ( (uintptr_t)a == (uintptr_t)b )
		return true;

	return false;
}

