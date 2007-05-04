/* @header@ */

#ifndef HAVE_EMU_SHELLCODE_H
#define HAVE_EMU_SHELLCODE_H

#include <stdint.h>

struct emu;

int32_t emu_shellcode_test(struct emu *e, uint8_t *data, uint16_t size);

#endif

