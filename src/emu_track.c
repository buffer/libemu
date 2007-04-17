#include <string.h>

#include "emu/emu.h"
#include "emu/emu_cpu.h"
#include "emu/emu_cpu_data.h"
#include "emu/emu_instruction.h"
#include "emu/emu_track.h"
#include "emu/emu_hashtable.h"
#include "emu/emu_graph.h"

struct emu_track_instr_info *emu_track_instr_info_new(struct emu_cpu *cpu, uint32_t eip_before_instruction)
{
	struct emu_track_instr_info *etii = (struct emu_track_instr_info *)malloc(sizeof(struct emu_track_instr_info));
	memset(etii, 0, sizeof(struct emu_track_instr_info));

	etii->eip = eip_before_instruction;
	etii->instrstring = strdup(cpu->instr_string);

	if ( cpu->instr.is_fpu)
	{
		
	}else
	{
		etii->source.has_cond_pos 	= cpu->instr.cpu.source.has_cond_pos;
		etii->source.cond_pos 		= cpu->instr.cpu.source.cond_pos;
		etii->source.norm_pos 		= cpu->instr.cpu.source.norm_pos;

		etii->track.init.eflags 	= cpu->instr.cpu.track.init.eflags;
		memcpy(etii->track.init.reg, cpu->instr.cpu.track.init.reg, sizeof(uint32_t)*8);

		etii->track.need.eflags 	= cpu->instr.cpu.track.need.eflags;
		memcpy(etii->track.need.reg, cpu->instr.cpu.track.need.reg, sizeof(uint32_t)*8);
	}
	return etii;
}

void emu_track_instr_info_free(struct emu_track_instr_info *etii)
{
	if (etii->instrstring != NULL)
		free(etii->instrstring);

	free(etii);
}

bool emu_track_instr_info_cmp(void *a, void *b)
{
	if ((uint32_t)a == (uint32_t)b)
		return true;

	return false;
}

uint32_t emu_track_instr_info_hash(void *key)
{
	uint32_t ukey = (uint32_t)key;
	ukey++;
	return ukey;
}


struct emu_track *emu_track_new()
{
	struct emu_track *et = (struct emu_track *)malloc(sizeof(struct emu_track));
	memset(et, 0, sizeof(struct emu_track));
	et->reg[esp] = 0xffffffff;
	return et;
}

void emu_track_free(struct emu_track *et)
{
	if (et->instrtable != NULL)
		emu_hashtable_free(et->instrtable);

	if (et->trackgraph != NULL)
		emu_graph_free(et->trackgraph);

	free(et);

}

uint32_t emu_track_tree_create(struct emu *e, struct emu_track *et, uint32_t datastart, uint32_t datasize)
{
	printf("tracking from %x to %x\n", datastart, datastart+datasize);
	struct emu_cpu *c = emu_cpu_get(e);

	et->trackgraph = emu_graph_new();
	et->instrtable = emu_hashtable_new(datasize/2, emu_track_instr_info_hash,  emu_track_instr_info_cmp);

	uint32_t i;
	for (i=datastart;i<datastart+datasize;i++)
	{
		emu_cpu_eip_set(c, i);

		if ( emu_cpu_parse(c) != 0)
			continue;

        struct emu_track_instr_info *etii = emu_track_instr_info_new(c,i);
		struct emu_vertex *ev = emu_vertex_new();
		ev->data = etii;
		emu_hashtable_insert(et->instrtable, (void *)i, ev);
		emu_graph_vertex_add(et->trackgraph, ev);
	}

	struct emu_vertex *ev;
	for ( ev = emu_vertexes_first(et->trackgraph->vertexes); !emu_vertexes_attail(ev); ev = emu_vertexes_next(ev) )
	{
		struct emu_track_instr_info *etii = (struct emu_track_instr_info *)ev->data;

		struct emu_hashtable_item *ehi = emu_hashtable_search(et->instrtable, (void *)etii->source.norm_pos);
		if (ehi != NULL)
		{
			struct emu_vertex *to = (struct emu_vertex *)ehi->value;
			emu_vertex_edge_add(ev, to);
			printf("from %08x to %08x\n",((struct emu_track_instr_info *)ev->data)->eip,((struct emu_track_instr_info *)to->data)->eip);
		}else
		{
//			printf("EHI IS UNKNOWN\n");
		}

		if (etii->source.has_cond_pos == 1)
		{
			ehi = emu_hashtable_search(et->instrtable, (void *)etii->source.cond_pos);
			if (ehi != NULL)
			{
				struct emu_vertex *to = (struct emu_vertex *)ehi->value;
				emu_vertex_edge_add(ev, to);
				printf("from %08x to %08x\n",((struct emu_track_instr_info *)ev->data)->eip,((struct emu_track_instr_info *)to->data)->eip);
			}
		}
	}
	return 0;
}

void debug_instruction(struct emu_cpu_instruction *i);

int32_t emu_track_instruction_check(struct emu *e, struct emu_track *et)
{
	struct emu_cpu *c = emu_cpu_get(e);
	int i;

	if (c->instr.is_fpu)
	{

	}else
	{
//		debug_instruction(&c->instr.cpu);
		for (i=0;i<8;i++)
		{
//			printf("0x%08x 0x%08x\n", c->instr.cpu.track.need.reg[i], et->reg[i]);
			if (c->instr.cpu.track.need.reg[i] > et->reg[i])
				return -1;
		}

		for (i=0;i<8;i++)
		{
//			printf("0x%1x 0x%1x\n", (c->instr.cpu.track.need.eflags & 1 << i), (et->eflags & 1 << i));
			if ( (c->instr.cpu.track.need.eflags & 1 << i) > (et->eflags & 1 << i))
				return -1;
		}

		for (i=0;i<8;i++)
		{
//			printf("reg %i before %08x after %08x\n", i, et->reg[i], c->instr.cpu.track.init.reg[i]);
			et->reg[i] |= c->instr.cpu.track.init.reg[i];
		}
		et->eflags |= c->instr.cpu.track.init.eflags;

	}


	return 0;
}
