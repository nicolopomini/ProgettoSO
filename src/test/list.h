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

list* list_init(); //Inizializza la lista a NULL
void add_name(list **l, char * n); //Inserirsce un nuovo nome nella lista solo se quel nome non esiste già
void delete_name(list *l, char * n); //ELimina un nome (n) dalla lista (l)
list* list_lookup(list *l, char * n); //Ritorna l'elemento della lista se esiste altrimenti ritorna NULL
int list_size(list *l); //Ritorna la lunghezza della lista (0 se è NULL)
list* n_elem_lista(list *l,int n); //Ritorna l' n-esimo elemento della lista
void print_list(list* l); //Stampa la lista

#endif
