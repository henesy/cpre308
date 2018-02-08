#ifndef SHELL_H
#define SHELL_H

#include <stdlib.h>
#define true 1
#define false 0
#define nil NULL

/* Process Architecture */
typedef struct Proc Proc;

struct Proc {
	int		pid;
	char*	name;
};

/* List (Queue) Architecture */
typedef struct Node Node;
typedef struct List List;

struct Node {
	Node*	next;
	void*	dat;
};

struct List {
	Node*	root;
	int		size;
};

// Create a new list
List	mklist();

// Append to a list
void ladd(List*, Proc*);

// Search → delete from a list
int ldel(List*, void*, int(*comp)(void *, void *));


#endif
