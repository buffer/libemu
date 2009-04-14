#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

struct nanny_file
{
	bool active;
	char		*path;
	uint32_t	emu_file;
	FILE		*real_file;
};

/*
struct nanny_socket
{
	bool active;
	int socket;
};

struct nanny_children
{
	int 	pid;
};
*/

struct nanny
{
	struct emu_hashtable *files;
/*	struct emu_hashtable *sockets; 
	struct emu_hashtable *children;
*/
};

struct nanny *nanny_new(void);
void nanny_free(struct nanny *nanny);

struct nanny_file *nanny_add_file(struct nanny *na, const char *path, uint32_t *emu_file, FILE *real_file);
struct nanny_file *nanny_get_file(struct nanny *na, uint32_t emu_file);
bool nanny_del_file(struct nanny *na, uint32_t emu_file);


