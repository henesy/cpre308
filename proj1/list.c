#include "shell.h"

// Create a new list
List
mklist()
{
	return (List){nil, 0};
}

// Append to a list
void
ladd(List* l, Proc* p)
{
	int i;
	Node* new = malloc(sizeof(Node));
	new->dat = (void*)p;
	new->next = nil;
	Node* n = l->root;
	Node* prev = nil;
	
	if(l->size == 0){
		l->root = new;
		new->next = nil;
		l->size++;
		return;
	}

	for(i = 0; i < l->size; i++){
		if(i != l->size-1)
			n = n->next;
	}
	
	n->next = new;
	new->next = nil;
	l->size++;
	return;
	
}

// Search → delete from a list
int
ldel(List* l, void* tofind, int(*comp)(void *, void *))
{
	int i;
	Node* n = l->root;
	Node* prev = nil;
	
	if(l->size == 0){
		return false;
	}

	for(i = 0; i < l->size; i++){
		if((*comp)(n->dat, tofind)){
			if(l->size == 1){
				// 1 node
				free(n->dat);
				free(n);
				l->root = nil;
			}else if(i == 0){
				// 0 nodes
				l->root = n->next;
				free(n->dat);
				free(n);
			}else if(i == l->size-1){
				// We are the last node
				/* BUG: segfault in finding if sleep 10 → sleep 5; ok if sleep 5 → sleep 10 
					Temporary fix: comment out line and break spec for list.	*/
				//prev->next = nil;
				free(n->dat);
				free(n);
			}else{
				// this is probably unsafe
				prev->next = n->next;
				free(n->dat);
				free(n);
			}
			l->size--;
			return true;
		}
		
		if(i != l->size-1)
			n = n->next;
		Node* prev = n;
			
	}
	return false;
}
