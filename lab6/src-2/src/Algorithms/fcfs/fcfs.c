
#include <stdlib.h>
#include <stdio.h>

#include "debug.h"
#include "task.h"
#include "scheduler_algorithm.h"

#define SCHEDULER_NAME "FCFS"

const struct scheduler_operations sops;

int verbose_flag = 1;

static int handle;

static struct task *head = NULL, *tail = NULL;

int init_module(void)
{
	handle = register_scheduler(SCHEDULER_NAME, &sops);
	if(handle < 0){
		eprintf("Failed registering scheduler FCFS with error %d\n", handle);
		return handle;
	}
	else{
		dprintf("Registered scheduler FCFS\n");
	}
	return 0;
}

void cleanup_module(void)
{
	int rv = unregister_scheduler(handle);
	if(rv < 0) eprintf("Error in unregister_scheduler: %d\n", rv);
}

struct task* fcfs_enqueue(struct task* r, struct task* t)
{
	t->next = NULL;
	if(tail)
	{
		tail->next = t;
		tail = t;
	}
	else
	{
		head = t;
		tail = t;
	}
	return head;
}

struct task* fcfs_dequeue(struct task* r)
{
	head = head->next;
	if(!head)
		tail = NULL;
	
	return head;
}

const struct scheduler_operations sops =
{
	.task_enqueue = fcfs_enqueue,
	.task_dequeue = fcfs_dequeue
};


