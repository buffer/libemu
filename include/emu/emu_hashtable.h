/* @header@ */
#ifndef EMU_HASHTABLE_H
#define EMU_HASHTABLE_H


#include <stdint.h>
#include <emu/emu_list.h>

/**
 * The emu_hashtable_item stores the key/value pair.
 * 
 * @see emu_hashtable
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


header_list_typedefs(emu_hashtable_bucket_item_root,emu_hashtable_bucket_item,emu_hashtable_bucket_link);

struct emu_hashtable_bucket_item
{
	struct emu_hashtable_item *item; // not using a pointer would fragmentate the heap 'a little less' TODO
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


/**
 * the hashtable
 * 
 * @see emu_hashtable_new
 * @see emu_hashtable_free
 * @see emu_hashtable_insert
 * @see emu_hashtable_delete
 * @see emu_hashtable_search
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


/**
 * Create a bucket hashtable
 * Due to the problems of double hashing when it comes to deleting 
 * elements we choose bucket hashtables.
 * If used wrong, as slow als linked lists, if used right, as fast as a
 * real (doublehash) hashtable.
 * 
 * @param size   the size
 * @param hash   pointer to the hash function
 * @param cmp    pointer to the compare function
 * 
 * @return on success, pointer to the hashtable
 *         on failure, NULL
 */
struct emu_hashtable *emu_hashtable_new(uint32_t size, 
											   emu_hashtable_hash_cb hash, 
											   emu_hashtable_cmp_cb cmp);

/**
 * Free the hashtable,
 * if the emu_hashtable_destructors key_/value_destructor 
 * within the emu_hashtable are set, freeing the hashtable will free the
 * data too, if not, only the hashtable itself is free'd.
 * 
 * @param eh     the hashtable
 */
void emu_hashtable_free(struct emu_hashtable *eh);


/**
 * Search the hashtable for a key
 * 
 * @param eh     the hashtable
 * @param key    the key to look for
 * 
 * @return on success, pointer to the emu_hashtable_item holding the data
 *         on failure (key not found), NULL
 */
struct emu_hashtable_item *emu_hashtable_search(struct emu_hashtable *eh, void *key);

/**
 * Insert a key/data pair into the hashtable
 * 
 * @param eh     the hashtable
 * @param key    the key
 * @param data   the data
 * 
 * @return on success: pointer to the emu_hashtable_item
 *         on failure: NULL
 */
struct emu_hashtable_item *emu_hashtable_insert(struct emu_hashtable *eh, void *key, void *data);

/**
 * delete a key/data pair from the hashtable
 * 
 * @param eh     the hashtable
 * @param key    the key to the key/data pair to remove
 * 
 * @return on success: true
 *         on failure: false
 */
bool emu_hashtable_delete(struct emu_hashtable *eh, void *key);

#endif // EMU_HASHTABLE_H
