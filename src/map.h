#ifndef MAP_H
#define MAP_H

#include "tree.h"

typedef struct node_t * map;

struct node_t
{
	char *name;
	tree* pid;
	map sx, dx;
};

void map_init(map *m);
int map_add(map *m, char *name, tree* pid);  //0 se gia' presente
tree* map_lookup(map m, char *name);	//NULL se non trovato
void map_delete(map *m, char *name);

#endif
