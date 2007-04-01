/* @header@ */
#ifndef HAVE_EMU_STRING_H
#define HAVE_EMU_STRING_H

#include <inttypes.h>
#include <sys/types.h>

struct emu_string
{
    uint32_t    size;
    void        *data;
	uint32_t	allocated;

};

struct emu_string *emu_string_new();
void emu_string_free(struct emu_string *s);
char *emu_string_char(struct emu_string *s);

void emu_string_append_char(struct emu_string *s, const char *data);

#endif


