#include "shell.h"

// Create a new list
List
mklist()
{
	return (List){nil, 0};
}

// Append to a list
int
ladd(List* l, int pid, char* name)
{
	
}

// Search → delete from a list
int
ldel(List* l, void* tofind, int(*comp)(const void *, const void *))
{
	int i;
	Node* n = l->root;
	Node* prev = l->root;

	for(i = 0; i < l->size; i++){
		if((*comp)(n->dat, tofind)){
			if(l->size == 1){
				free(n);
				l->root = nil;
			}else{
				
			}
			l->size--;
			return true;
		}
		n = n->next;
	}
	return false;
}
