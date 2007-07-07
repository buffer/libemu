/* @header@ */
#ifndef HAVE_EMU_H
#define HAVE_EMU_H

#include <stdio.h>
#include <stdint.h>

#define        MIN(a,b) (((a)<(b))?(a):(b))
#define        MAX(a,b) (((a)>(b))?(a):(b))


struct emu;
struct emu_logging;
struct emu_cpu;
struct emu_fpu;


/**
 * Create a new emu.
 * 
 * @return on success: the new emu
 *         on failure: NULL
 */
struct emu *emu_new();

/**
 * Free the emu
 * 
 * @param e      the emu to free
 */
void emu_free(struct emu *e);

/**
 * Retrieve a pointer to the emu's emu_memory.
 * 
 * @param e      the emu
 * 
 * @return The pointer to the emu's emu_memory.
 */
struct emu_memory *emu_memory_get(struct emu *e);

/**
 * Retrieve a pointer to the emu's logging facility.
 * 
 * @param e      the emu
 * 
 * @return pointer to the emu's emu_logging.
 */
struct emu_logging *emu_logging_get(struct emu *e);

/**
 * Retrieve a pointer to the emu's emu_cpu
 * 
 * @param e      the emu
 * 
 * @return pointer to the emu's emu_cpu.
 */
struct emu_cpu *emu_cpu_get(struct emu *e);

/**
 * Set the emu's internal errno
 * 
 * @param e      the emu
 * @param err
 */
void emu_errno_set(struct emu *e, int err);

/**
 * Retrieve the emu's errno
 * 
 * @param c      the emu
 * 
 * @return the emu's errno
 */
int emu_errno(struct emu *c);

/**
 * Set the emu's strerror message.
 * 
 * @param e      the emu
 * @param format the errormessage format
 */
void emu_strerror_set(struct emu *e, const char *format, ...);

/**
 * Retrieve the emu's strerror
 * 
 * @param e      the emu
 * 
 * @return the strerror
 */
const char *emu_strerror(struct emu *e);


/*int32_t emu_parse(struct emu *e);
int32_t emu_step(struct emu *e);*/
#endif // HAVE_EMU_H
