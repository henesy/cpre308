
#include <stdlib.h>
#include <stdio.h>

#include "debug.h"
#include "task.h"
#include "scheduler_algorithm.h"

#define SCHEDULER_NAME "SRTN"
#define nil ((void*)0)

// for storing data
typedef struct metadata metadata;
struct metadata {
	int last_guess;
	int last_runtime;
};

const struct scheduler_operations sops;

int verbose_flag = 1;

static int handle;

static struct task *head = NULL;

int init_module(void)
{
	handle = register_scheduler(SCHEDULER_NAME, &sops);
	if(handle < 0){
		eprintf("Failed registering scheduler SRTN with error %d\n", handle);
		return handle;
	}
	else{
		dprintf("Registered scheduler SRTN\n");
	}
	return 0;
}

void cleanup_module(void)
{
	int rv = unregister_scheduler(handle);
	if(rv < 0) eprintf("Error in unregister_scheduler: %d\n", rv);
}

struct task* 
srtn_enqueue(struct task* r, struct task* t)
{
	metadata* mt;

	// t is the task to add
	if(t->scheduler_data == nil){
		mt = calloc(1, sizeof(metadata));
		mt->last_guess = 2*(4 - t->task_info->priority);
		mt->last_runtime = 0;
		t->scheduler_data = mt;
	}else{
		mt = (metadata*) t->scheduler_data;
		mt->last_runtime = t->task_info->run_time - mt->last_runtime;
		mt->last_guess = 0.2*mt->last_guess + 0.8*mt->last_runtime;
		t->scheduler_data = mt;
	}
	
	// sort
	if(head == nil){
		head = t;
		t->next = head;
		t->prev	= head;
	}else{
		metadata* mt1;
		mt = (metadata*) t->scheduler_data;
		mt1 = (metadata*) r->scheduler_data;
		
		while(mt1->last_guess < mt->last_guess){
			if(r == head)
				break;
			r = r->next;
			mt1 = (metadata*) r->scheduler_data;
		}

		r->prev->next = t;
		t->prev	= r->prev;
		r->prev = t;
		t->next	= r;
		head = r;
	}

	return head;
}

struct task*
srtn_dequeue(struct task* r)
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

const struct scheduler_operations sops =
{
	.task_enqueue = srtn_enqueue,
	.task_dequeue = srtn_dequeue,
};
