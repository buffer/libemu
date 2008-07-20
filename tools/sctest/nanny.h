#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

struct nanny_socket
{
	int socket;
};

struct nanny_file
{
	char		*path;
	uint32_t	emu_file;
	FILE		*real_file;
};

struct nanny_children
{
	int 	pid;
};

struct nanny
{
	struct emu_hashtable *sockets;
	struct emu_hashtable *files;
	struct emu_hashtable *children;
};

struct nanny *nanny_new();
//struct nanny_socket *nanny_add_socket(struct nanny *nanny, int socket);
struct nanny_file *nanny_add_file(struct nanny *na, const char *path, uint32_t *emu_file, FILE *real_file);

//struct nanny_socket *nanny_get_socket(struct nanny *nanny, uint32_t emu_file);
struct nanny_file *nanny_get_file(struct nanny *na, uint32_t emu_file);
bool nanny_del_file(struct nanny *na, uint32_t emu_file);
void nanny_free(struct nanny *nanny);
