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

#include <stdlib.h>
#include <string.h>

#include "emu/emu.h"
#include "emu/environment/win32/emu_env_w32_dll.h"
#include "emu/environment/win32/emu_env_w32_dll_export.h"
#include "emu/emu_hashtable.h"

struct emu_env_w32_dll *emu_env_w32_dll_new()
{
	struct emu_env_w32_dll *dll = (struct emu_env_w32_dll *)malloc(sizeof(struct emu_env_w32_dll));
	memset(dll,0,sizeof(struct emu_env_w32_dll));
    return dll;
}

void emu_env_w32_dll_free(struct emu_env_w32_dll *dll)
{
	emu_hashtable_free(dll->exports_by_fnptr);
	emu_hashtable_free(dll->exports_by_fnname);
	free(dll->exports);
	free(dll->dllname);
	free(dll);
}

bool dll_export_fnptr_cmp(void *a, void *b)
{
	if ((uint32_t)a == (uint32_t)b)
		return true;

	return false;
}

uint32_t dll_export_fnptr_hash(void *key)
{
	uint32_t ukey = (uint32_t)key;
	ukey++;
	return ukey;
}


bool dll_export_fnname_cmp(void *a, void *b)
{
	if (strcmp((char *)a, (char *)b) == 0)
		return true;

	return false;
}

#define HASH_KEY                       13
uint32_t dll_export_fnname_hash(void *key)
{
    uint32_t hash = 0;
    char *c = (char *)key;
/*
    while ( *c != 0 )
    {
        asm ("ror DWORD hash,13");
        
        hash += *c++;
    }
*/
	while (*c != 0)
	{
		hash = hash << 19 | hash >> 13;
		hash += *c;
		c++;
	}

    return hash;
}


void emu_env_w32_dll_exports_copy(struct emu_env_w32_dll *to,struct emu_env_w32_dll_export *from)
{
	uint32_t size;
	uint32_t i;
	for (i=0;from[i].fnname != 0; i++);

	size = i;

	to->exports = (struct emu_env_w32_dll_export *)malloc(sizeof(struct emu_env_w32_dll_export) * size);
	memcpy(to->exports, from, sizeof(struct emu_env_w32_dll_export) * size);

	to->exports_by_fnptr = emu_hashtable_new(size, dll_export_fnptr_hash, dll_export_fnptr_cmp);
	to->exports_by_fnname = emu_hashtable_new(size, dll_export_fnname_hash, dll_export_fnname_cmp);

	for (i=0;from[i].fnname != 0; i++)
	{
		struct emu_env_w32_dll_export *ex = &to->exports[i];
		emu_hashtable_insert(to->exports_by_fnptr, (void *)from[i].virtualaddr, ex);
		emu_hashtable_insert(to->exports_by_fnname, (void *)from[i].fnname, ex);
	}
}
