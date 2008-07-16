struct instr_vertex
{
	uint32_t    eip;
	struct emu_string  *instr_string;
	struct emu_env_w32_dll *dll;
	struct emu_env_linux_syscall *syscall;
};


struct instr_vertex *instr_vertex_new(uint32_t theeip, const char *instr_string);
void instr_vertex_free(struct instr_vertex *iv);

struct instr_vertex *instr_vertex_copy(struct instr_vertex *from);

void instr_vertex_destructor(void *data);

int graph_draw(struct emu_graph *graph);

