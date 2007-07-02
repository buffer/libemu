/* @header@ */

#include <string.h>

#include "emu/emu_list.h"

source_list_functions(emu_list, emu_list_root, emu_list_item, link);

struct emu_list_item *emu_list_item_create()
{
	struct emu_list_item *eli = malloc(sizeof(struct emu_list_item));
	memset(eli, 0, sizeof(struct emu_list_item));
	emu_list_init_link(eli);
	return eli;
}

