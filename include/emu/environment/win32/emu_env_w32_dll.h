/* @header@ */
#include <stdint.h>

struct emu_env_w32_dll_export;

struct emu_env_w32_dll
{
	char 		*dllname;

	char 		*image;
	uint32_t	imagesize;

	uint32_t	baseaddr;

	struct emu_env_w32_dll_export *exports;
	struct emu_hashtable *exports_by_fnptr;
	struct emu_hashtable *exports_by_fnname;
};

struct emu_env_w32_dll *emu_env_w32_dll_new();
void emu_env_w32_dll_free(struct emu_env_w32_dll *dll);
void emu_env_w32_dll_exports_copy(struct emu_env_w32_dll *to, struct emu_env_w32_dll_export *from);


struct emu_env_w32_known_dll_segment
{
	uint32_t address;
	const char *segment;
	uint32_t	segment_size;
};

struct emu_env_w32_known_dll
{
	const char *dllname;
	uint32_t 	baseaddress;
	uint32_t	imagesize;
	struct emu_env_w32_dll_export *exports;
	struct emu_env_w32_known_dll_segment *memory_segments;
};

