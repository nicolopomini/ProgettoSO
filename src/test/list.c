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
		list* tmp = (list*)malloc(sizeof(list));
		tmp->i = 0;
		tmp->name_process=(char*)malloc(sizeof(char)*(strlen(n)+1));
		sprintf(tmp->name_process,"%s",n);
		tmp->pred = NULL;
		tmp->succ = NULL;
		(*l)=tmp;	
	}else{
		if(list_lookup((*l), n)==NULL){
			list* tmp = (list*)malloc(sizeof(list));
			tmp->i = 0;
			tmp->name_process=(char*)malloc(sizeof(char)*(strlen(n)+1));
			sprintf(tmp->name_process,"%s",n);
			while((*l)->succ != NULL){
				(*l) = (*l)->succ;
			}
			tmp->pred = (*l);
			(*l)->succ = tmp;
			tmp->succ = NULL;
			while((*l)->pred != NULL){
				(*l) = (*l)->pred;
			}
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
	list* tmp = l;
	while(tmp!=NULL){
		printf("%s\n", tmp->name_process);
		tmp = tmp->succ;
	}
}
