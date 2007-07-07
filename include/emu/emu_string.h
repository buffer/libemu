/* @header@ */
#ifndef HAVE_EMU_STRING_H
#define HAVE_EMU_STRING_H

#include <inttypes.h>
#include <sys/types.h>

/**
 * The struct used for the string implementation.
 */
struct emu_string
{
    uint32_t    size;
    void        *data;
	uint32_t	allocated;

};

/**
 * Create a new, empty string.
 * 
 * @return on success, pointer to the new and empty string, on failure NULL
 */
struct emu_string *emu_string_new();

/**
 * Free the string, free the bytes which got allocated.
 * 
 * @param s the string to free
 */
void emu_string_free(struct emu_string *s);


/**
 * cast the string to char *
 * 
 * @param s      the string
 * 
 * @return char * of the string
 */
char *emu_string_char(struct emu_string *s);

/**
 * append the string by some data
 * 
 * @param s      the string
 * @param data   the data to append
 */
void emu_string_append_char(struct emu_string *s, const char *data);


/**
 * append the string by some formatted string
 * 
 * @param s      the string
 * @param format the format
 */
void emu_string_append_format(struct emu_string *s, const char *format, ...);

#endif


