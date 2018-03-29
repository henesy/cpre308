
#include <stdlib.h>
#include <stdio.h>

#include "debug.h"
#include "task.h"
#include "scheduler_algorithm.h"

#define SCHEDULER_NAME "RR"

const struct scheduler_operations sops;

int verbose_flag = 1;

static int handle;

static struct task *head = NULL, *tail = NULL;

int init_module(void)
{
	handle = register_scheduler(SCHEDULER_NAME, &sops);
	if(handle < 0){
		eprintf("Failed registering scheduler RR with error %d\n", handle);
		return handle;
	}
	else{
		dprintf("Registered scheduler RR\n");
	}
	return 0;
}

void cleanup_module(void)
{
	int rv = unregister_scheduler(handle);
	if(rv < 0) eprintf("Error in unregister_scheduler: %d\n", rv);
}

struct task* 
rr_enqueue(struct task* r, struct task* t)
{
	if(head == NULL){
		head = t;
		t->next = head;
		t->prev = head;
	}else{
		head->prev->next = t;
		t->prev = head->prev;
		head->prev = t;
		t->next = head;
	}
	return head;
}

struct task*
rr_dequeue(struct task* r)
{
	if(r->next == r)
		head = NULL;
	else{
		r->prev->next = r->next;
		r->next->prev = r->prev;
		head = r->next;
	}
	return head;
}

struct task*
rr_periodic_timer(struct task* r)
{
	if(head)
		head = head->next;
	return head;
}

const struct scheduler_operations sops =
{
	.task_enqueue = rr_enqueue,
	.task_dequeue = rr_dequeue,
	.periodic_timer = rr_periodic_timer,
	.period = 1
};
