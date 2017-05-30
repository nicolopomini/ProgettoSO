#ifndef MAP_H
#define MAP_H

#include "tree.h"

typedef struct node_t * map;

struct node_t
{
	char *name;		//nome del processo
	tree* pid;		//puntatore al processo nell'albero dei processi
	map sx, dx;		
};

void map_init(map *m);		//inizializza la mappa
int map_add(map *m, char *name, tree* pid);  //aggiunge il processo nome alla mappa, se già presente ritorna 0
tree* map_lookup(map m, char *name);	//cerca il processo nome nella mappa, ritornando un puntatore all'albero, NULL se non trovato
void map_remove(map *m, char *name);	//rimuove il processo nome

#endif
