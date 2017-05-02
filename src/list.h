#ifndef LIST_H
#define LIST_H

typedef struct node_s node;
typedef struct node_s
{
	int pid;
	node *next;
} node;
typedef node* list;

void list_init(list *l);
int list_empty(const list l);
void list_insert(list *l, int pid);
void list_remove(list *l, int pid);
void list_delete(list *l);
#endif