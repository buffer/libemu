/* @header@ */

#include <string.h>
#include <stdlib.h>


#include "emu/emu_string.h"


struct emu_string *emu_string_new()
{
    struct emu_string *s = (struct emu_string *)malloc(sizeof(struct emu_string));
    memset(s, 0, sizeof(struct emu_string));
    return s;
}

void emu_string_free(struct emu_string *s)
{
    free(s->data);
    free(s);
}

char *emu_string_char(struct emu_string *s)
{
    return (char *)s->data;
}
