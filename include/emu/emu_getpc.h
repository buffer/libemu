/* @header@ */


#include <stdint.h>

/**
 * 
 * @param e
 * @param data
 * @param size
 * @param offset
 * 
 * @return 1 for getpc via call or fnstenv
 *         2 for mov withing fs: segment
 */
uint8_t emu_getpc_check(struct emu *e, uint8_t *data, uint32_t size, uint32_t offset);
