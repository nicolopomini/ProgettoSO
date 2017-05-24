#include "list.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

list* list_init()
{
	return NULL;
}

void add_name(list **l, char * n){
	if( (*l) == NULL){
		(*l) =(list*)malloc(sizeof(list));
		(*l)->name_process=(char*)malloc(sizeof(char)*(strlen(n)));
		strcpy((*l)->name_process,n);
		(*l)->pred = NULL;
		(*l)->succ = NULL;
		(*l)->i = 0;
	}else{
		if(list_lookup((*l), n)==NULL){
			list* tmp = (list*)malloc(sizeof(list));
			tmp->name_process=(char*)malloc(sizeof(char)*(strlen(n)));
			strcpy(tmp->name_process,n);
			while((*l)->succ != NULL){
				(*l) = (*l)->succ;
			}
			tmp->pred = (*l);
			(*l)->succ = tmp;
			tmp->succ= NULL;
		}
		else{
			printf("Nome gia' presente\n");
		}
	}
}

void delete_name(list *l, char * n){
	list* tmp=list_lookup(l,n);
	if(tmp!=NULL){
		(tmp->pred)->succ = tmp->succ;
		(tmp->succ)->pred = tmp->pred;
		free(tmp->name_process);
		free(tmp);
	}
}

list* list_lookup(list *l, char * n){
	list* tmp=l;
	while(tmp != NULL){
		if(strcmp(n,tmp->name_process)==0){
		return tmp;
		}
		tmp=tmp->succ;
	}
	return NULL;
}

int list_size(list *l){
	list* tmp=l;
	int i=0;
	while( tmp != NULL){
		i++;
		tmp=tmp->succ;
	}
	return i;
}

list* n_elem_lista(list *l, int n){
	if( n > list_size(l) ){
		return NULL;
	}
	list* tmp = l;
	for(int i=0; i<n ; i++){
		tmp = tmp->succ;
	}
	return tmp;
}

void print_list(list* l){
	while(l!=NULL){
		printf("%s\n", l->name_process);
		l = l->succ;
	}
}
