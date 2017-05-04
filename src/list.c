#include "const.h"
#include "list.h"
#include <stdlib.h>
#include <string.h>

void list_init(list *l)
{
	*l = NULL;
}
int list_empty(const list l)
{
	return l == NULL;
}
void list_insert(list *l, int pid)
{
	node *toadd = (node*)malloc(sizeof(node));
	toadd->pid = pid;
	toadd->next = *l;
	*l = toadd;
}
void list_remove(list *l, int pid)
{
	if((*l)->pid == pid)	//elimino in testa
	{
		node *del = *l;
		*l = (*l)->next;
		free(del);
	}
	else
	{
		node *now = *l;
		while((*l)->next != NULL && (*l)->pid != pid)
		{
			now = (*l)->next;
		}
		if(now != NULL)
			free(now);
	}
}
void list_delete(list *l)
{
	list tmp;
	while(*l != NULL)
	{
		tmp = (*l)->next;
		free(*l);
		*l = tmp;
	}
}
/*
void printlist(list l)
{
	if (l == NULL) {
        printf("la lista è vuota\n");
    }
    while (l != NULL) {
        printf("%d\n", l->pid);
        l = l->next;
    }
}

int main()
{
	list l;
	list_init(&l);
	printf("Vuota: %d\n", list_empty(l));
	//char *a = "AAA";
	list_insert(&l,12);
	printf("Vuota: %d\n", list_empty(l));
	printlist(l);
	printf("\n");
	//char *b = "AB";
	list_insert(&l,2);
	printlist(l);
	printf("\n");
	list_remove(&l,12);
	printlist(l);
	printf("\n");
	list_remove(&l,12);
	printlist(l);
	printf("\n");
	list_delete(&l);
	printf("Vuota: %d\n", list_empty(l));
	return 0;
}
*/
