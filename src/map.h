#ifndef MAP_H
#define MAP_H
typedef struct node_t * map;
struct node_t
{
	char *name;
	int pid;
	map sx, dx;
};
void map_init(map *m);
void map_add(map *m, char *name, int pid); 	//se già esistente sovrascrive
map map_lookup_getnode(map m, char *name);
int map_lookup(map m, char *name);	//-1 se non trovato
void map_delete(map *m, char *name);
#endif