#include "map.h"
#include <stdlib.h>
#include <string.h>

const int MAP_STRING_MAX_LEN = 100;

map map_lookup_getnode(map m, char *name)
{
	map res;
	if(m == NULL)
		res = NULL;
	else
	{
		int cmp = strcmp(name,m->name);
		if(cmp == 0)
			res = m;
		else if(cmp < 0)
			res = map_lookup_getnode(m->sx,name);
		else
			res = map_lookup_getnode(m->dx,name);
	}
	return res;
}

void map_delete(map *m, char *name)
{
	if(*m != NULL)
	{
		int cmp = strcmp(name, (*m)->name);
		if(cmp == 0)
		{
			if((*m)->sx != NULL && (*m)->dx != NULL)
			{
				map min = (*m)->dx;
				while(min->sx != NULL)
					min = min->sx;
				(*m)->pid = min->pid;
				strcpy((*m)->name,min->name);
				map_delete(&(*m)->dx,(*m)->name);
			}
			else if((*m)->sx == NULL && (*m)->dx != NULL)
			{
				map tmp = *m;
				*m = tmp->dx;
				free(tmp);
			}
			else if((*m)->sx != NULL && (*m)->dx == NULL)
			{
				map tmp = *m;
				*m = tmp->sx;
				free(tmp);
			}
			else
			{
				free(*m);
				*m = NULL;
			}
		}
		else if(cmp < 0)
			map_delete(&(*m)->sx,name);
		else
			map_delete(&(*m)->dx,name);
	}
}

void map_init(map *m)
{
	*m = NULL;
}
int map_add(map *m, char *name, int pid)
{
	int res, ret = 1;
	if(*m != NULL)
		res = strcmp(name,(*m)->name);
	if(*m == NULL)
	{
		map n = (map)malloc(sizeof(struct node_t));
		n->name = (char*)malloc(sizeof(char)*(MAP_STRING_MAX_LEN + 1));
		strcpy(n->name,name);
		n->pid = pid;
		n->sx = NULL;
		n->dx = NULL;
		*m = n;
	}
	else if(res == 0)
		ret = 0;
	else if(res < 0)
		ret = map_add(&(*m)->sx, name,pid);
	else
		ret = map_add(&(*m)->dx, name,pid);
	return ret;
}
int map_lookup(map m, char *name)
{
	map tmp = map_lookup_getnode(m,name);
	return tmp == NULL ? -1 : tmp->pid;
}
/*
void printmap(map m)
{
	if(m != NULL)
	{
		printmap(m->sx);
		printf("%s %d\n", m->name, m->pid);
		printmap(m->dx);
	}
}
int main()
{
	char *a = "A", *b = "B", *c = "C";
	map m;
	map_init(&m);
	map_add(&m,b,1);
	map_add(&m,c,12);
	map_add(&m,a,123);
	printmap(m);
	map_delete(&m,b);
	printf("%d\n", map_lookup(m,a));
	printmap(m);
	//printf("%d\n", strcmp(a,b));
	return 0;
}
*/
