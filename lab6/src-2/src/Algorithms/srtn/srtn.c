
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

// the total runtime counted by the scheduler
int runtime = 0;

static int handle;

static struct task *head = NULL, *tail = NULL;

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
	runtime++;

	// t is the task to add
	if(t->scheduler_data == nil){
		metadata* mt = calloc(1, sizeof(metadata));
		mt->last_guess = 2*(4 - t->task_info->priority);
		mt->last_runtime = 0;
		t->scheduler_data = mt;
	}else{
		metadata* mt = (metadata*)t->scheduler_data;
		mt->last_runtime = t->task_info->run_time - runtime;
	}

	// add t to the queue
	if(head == NULL){
		head = t;
		t->next = head;
		t->prev = head;

		return head;
	}else{
		head->prev->next = t;
		t->prev = head->prev;
		head->prev = t;
		t->next = head;
	}
	
	// estimate run times and find lowest time
	int lowest_guess = -1;
	struct task* lowest = nil;
	struct task* cursor = head;
	
	while(1){
		if(cursor) == nil
			break;
		
		metadata* mt = (metadata*) t->scheduler_data;
		int new_guess = 0.2 * mt->last_guess + 0.8 * mt->last_runtime;
		
		if(lowest_guess < 0){
			lowest = cursor;
			lowest_guess = new_guess;
		}else if(new_guess < lowest_guess){
			lowest = cursor;
			lowest_guess = new_guess;
		}
		
		cursor = cursor->next;
	}
	
	// make the lowest element the new head
	if(head == lowest)
		return head;
	
	if(lowest->next != nil){
		
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
	.period = 1
};
