
#include <stdlib.h>
#include <stdio.h>

#include "debug.h"
#include "task.h"
#include "scheduler_algorithm.h"

#define SCHEDULER_NAME "PRR"
#define nil ((void*)0)

const struct scheduler_operations sops;

int verbose_flag = 1;

static int handle;

static struct task *head0 = nil, *head1 = nil, *head2 = nil, *head3 = nil;

int init_module(void)
{
	handle = register_scheduler(SCHEDULER_NAME, &sops);
	if(handle < 0){
		eprintf("Failed registering scheduler PRR with error %d\n", handle);
		return handle;
	}
	else{
		dprintf("Registered scheduler PRR\n");
	}
	return 0;
}

void cleanup_module(void)
{
	int rv = unregister_scheduler(handle);
	if(rv < 0) eprintf("Error in unregister_scheduler: %d\n", rv);
}

struct task* 
prr_enqueue(struct task* r, struct task* t)
{
	struct task** h;
	if(t->task_info->priority == 0)
		h = &head0;
	else if(t->task_info->priority == 1)
		h = &head1;
	else if(t->task_info->priority == 2)
		h = &head2;
	else
		h = &head3;

	if(*h == NULL){
		*h = t;
		t->next = *h;
		t->prev = *h;
	}else{
		(*h)->prev->next = t;
		t->prev = (*h)->prev;
		(*h)->prev = t;
		t->next = (*h);
	}
	return *h;
}

struct task*
prr_dequeue(struct task* r)
{
	// select a head to use
	struct task** h;
	if(r->task_info->priority == 0)
		h = &head0;
	else if(r->task_info->priority == 1)
		h = &head1;
	else if(r->task_info->priority == 2)
		h = &head2;
	else
		h = &head3;

	if(r->next == r)
		*h = NULL;
	else{
		r->prev->next = r->next;
		r->next->prev = r->prev;
		*h = r->next;
	}
	return *h;
}

struct task*
prr_periodic_timer(struct task* r)
{
	if(head3){
		head3 = head3->next;
		return head3;
	}else if(head2){
		head2 = head2->next;
		return head2;
	}else if(head1){
		head1 = head1->next;
		return head1;
	}else if(head0){
		head0 = head0->next;
		return head0;
	}
	
	return nil;
}

const struct scheduler_operations sops =
{
	.task_enqueue = prr_enqueue,
	.task_dequeue = prr_dequeue,
	.periodic_timer = prr_periodic_timer,
	.period = 1
};
