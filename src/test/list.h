#ifndef LIST_H
#define LIST_H
#define TRUE 1
#define FALSE 0
#define ERROR -1
//Lista per i nomi
typedef struct name_list list;
typedef struct name_list{
	char *name_process;
	list *pred; //puntatore alla stringa successiva
	list *succ; //puntatore alla stringa precendente
	int i; //quanti pspawn sono stati fatti su du lui
} list;

list* list_init();
void add_name(list **l, char * n);
void delete_name(list *l, char * n);
list* list_lookup(list *l, char * n);
int list_size(list *l);
list* n_elem_lista(list *l,int n);
void print_list(list* l);

#endif
