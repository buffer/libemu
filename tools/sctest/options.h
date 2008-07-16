struct run_time_options
{
	int verbose;
	int nasm_force;
	uint32_t steps;
	int testnumber;
	int getpc;
	char *graphfile;
	bool from_stdin;
	char *from_argos_csi;
	unsigned char *scode;
	uint32_t size;
	int offset;
	char *profile_file;
	bool interactive;

	struct 
	{
		struct
		{
			char *host;
			int port;
		}connect;
		struct 
		{
			char *host;
			int port;
		}bind;

		struct 
		{
			struct emu_hashtable *commands;
		}commands;

	}override;
};

extern struct run_time_options opts;


bool cmp(void *a, void *b);
uint32_t hash(void *key);
bool string_cmp(void *a, void *b);
uint32_t string_hash(void *key);


