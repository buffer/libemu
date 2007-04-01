/* @header@ */
#ifndef EMU_HASHTABLE_H
#define EMU_HASHTABLE_H


#include <stdint.h>
#include <emu/emu_list.h>

/* 
 * generics
 */

struct emu_hashtable_item 
{
	void *key;
	void *value;
};
struct emu_hashtable_item *emu_hashtable_item_new(void *key, void *value);
void emu_hashtable_item_free(struct emu_hashtable_item *ehi);

typedef bool (*emu_hashtable_cmp_cb)(void *a, void *b);
typedef uint32_t (*emu_hashtable_hash_cb)(void *key);
typedef void (*emu_hashtable_destructor)(void *data);


/* 
 * bucket hashing
 */

header_list_typedefs(emu_hashtable_bucket_item_root,emu_hashtable_bucket_item,emu_hashtable_bucket_link);
struct emu_hashtable_bucket_item
{
	struct emu_hashtable_item *item; // use a pointer so we can return a emu_hashtable_item
	emu_hashtable_bucket_link link;
};
header_list_functions(emu_hashtable_bucket_items,emu_hashtable_bucket_item_root, emu_hashtable_bucket_item, link);

struct emu_hashtable_bucket_item *emu_hashtable_bucket_item_new(void *key, void *value);


struct emu_hashtable_bucket
{
	emu_hashtable_bucket_item_root *items;
};

struct emu_hashtable_bucket *emu_hashtable_bucket_new();
void emu_hashtable_bucket_free(struct emu_hashtable_bucket *ehb);
void emu_hashtable_bucket_item_add(struct emu_hashtable_bucket *ehb, struct emu_hashtable_bucket_item *ehbi);


/*
 * the hashtable
 */

struct emu_hashtable
{
	uint32_t size;
	uint32_t filled;
	uint32_t item_count;

	struct emu_hashtable_bucket **buckets;

	emu_hashtable_hash_cb hash;
	emu_hashtable_cmp_cb cmp;

	emu_hashtable_destructor key_destructor;
	emu_hashtable_destructor value_destructor;
};


struct emu_hashtable *emu_hashtable_new(uint32_t size, 
											   emu_hashtable_hash_cb hash, 
											   emu_hashtable_cmp_cb cmp);

void emu_hashtable_free(struct emu_hashtable *eh);
struct emu_hashtable_item *emu_hashtable_search(struct emu_hashtable *eh, void *key);

struct emu_hashtable_item *emu_hashtable_insert(struct emu_hashtable *eh, void *key, void *data);

bool emu_hashtable_delete(struct emu_hashtable *eh, void *key);

#endif // EMU_HASHTABLE_H
