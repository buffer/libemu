/* @header@ */

#ifndef HAVE_EMU_QUEUE_H
#define HAVE_EMU_QUEUE_H


struct emu_queue_item
{
	struct emu_queue_item *next;
	void *data;
};

struct emu_queue_item *emu_queue_item_new();
void emu_queue_item_free(struct emu_queue_item *eqi);

struct emu_queue
{
	struct emu_queue_item *front;
	struct emu_queue_item *back;
};

struct emu_queue *emu_queue_new();
void emu_queue_free(struct emu_queue *eq);

void *emu_queue_front(struct emu_queue *eq);
void  emu_queue_enqueue(struct emu_queue *eq, void *data);
void *emu_queue_dequeue(struct emu_queue *eq);
bool  emu_queue_empty(struct emu_queue *eq);

#endif
