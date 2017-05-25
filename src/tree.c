#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdarg.h>

#include "tree.h"

#define KNRM  "\x1B[0m"
#define BNRM  "\033[1m\033[37m" 

#define KRED  "\x1B[31m"
#define BRED  "\033[1m\033[31m"

#define KGRN  "\x1B[32m"
#define BGRN  "\033[1m\033[32m"

#define KYEL  "\x1B[33m"
#define BYEL  "\033[1m\033[33m"     

#define KBLU  "\x1B[34m"
#define BBLU  "\033[1m\033[34m"

#define KMAG  "\x1B[35m"
#define BMAG  "\033[1m\033[35m"

#define KCYN  "\x1B[36m"
#define BCYN  "\033[1m\033[36m" 

void tree_init(tree **t) {
	(*t) = NULL;
}

int tree_empty(const tree *t) {
	return t == NULL;
}

tree* tree_insert(tree **t, int pid, char* name) {
	tree* add = (tree*) malloc(sizeof(tree));
	add->pid = pid;
	add->child_number = 0;
	add->active = 1;

	int len = strlen(name);
	add->name = (char*) malloc(sizeof(char)*(len+1));
	strcpy(add->name,name);

	add->sibling = NULL;
	add->child = NULL;

	if (tree_empty(*t)) {
		add->parent = NULL;
		add->depth = 0;

		*t = add;
	} else {
		add->parent = (*t);
		add->depth = (*t)->depth+1;
		if (tree_empty((*t)->child)) {
			//t has no child. link directly to the parent	
			(*t)->child = add;
		} else {
			//t already has a child. find the last sibling and link the node to it
			tree* tmp = (*t)->child;
			while (!tree_empty(tmp->sibling))
				tmp = tmp->sibling;
			tmp->sibling = add;
		}
		(*t)->child_number++;

	}
	return add;
}
int tree_remove(tree *t, int delete) {
	if (tree_empty(t->child)) {
		if (kill(t->pid,SIGUSR1) == 0) {
			if (delete == 1) {
				if (!tree_empty(t->parent)) {
					//we can remove the node
					//we have 2 situations: t is a child or t is a sibling (as in the insert)
					if (t->parent->child == t) {
						t->parent->child = t->sibling; 
					} else {
						tree* prec = t->parent->child;
						while(prec->sibling != t) {
							prec = prec->sibling;
						}
						prec->sibling = t->sibling;
					}

					free(t->name);
					free(t);
					return 1;
				} else {
					free(t->name);
					free(t);
					return 1;
				}
			} else {
				t->active = 0;
				return 1;
			}
		} else {
			//failed to remove, because the signal wasn't delivered in the correct way. 
			return 0;
		}
	} else {
		//failed to remove, because the node has children.
		return 0;
	}
}

void tree_delete(tree **t,int delete) {
	/*recursively call the delete on all the children of the node.
	the when a node has no children remove it. There is no need to 
	search all the siblings because as you delete the child of a node,
	the siblings takes it place. (I love this function, is really magical)
	*/
	while (!tree_empty((*t)->child)) {
		tree_delete(&(*t)->child, delete);
	}
	tree_remove(*t,delete);
}

int tree_getNumberOfChildren(tree *t) {
	int child_count = 0;
	tree* tmp = t->child;
	while (!tree_empty(tmp)) {
		child_count+=tmp->active;
		tmp = tmp->sibling;
	}

	return child_count;
}

//a recursive function for printing the tree.
//First setted indicates that the node is the root (used only on the first call)
//Second setted means that the node is the last child, so a different box shape is printed
void printCustomTree(tree *t, int first, int last) {
	printf("\t");

	if (first == 1) {
		printf(" %s%s%s\n",BYEL,t->name,KNRM);
	} else {
		for (int i = 0; i < t->depth-1; ++i) {
			printf("  \u2502 ");	
		}
		
		if (last == 1) {
			printf("  \u2514\u2500%s%s%s\n",(t->active == 1?BGRN:BRED),t->name,KNRM);
		} else {
			printf("  \u251C\u2500%s%s%s\n",(t->active == 1?BGRN:BRED),t->name,KNRM);
		}	
	}

	tree* tmp = t->child;
	while (!tree_empty(tmp) && !tree_empty(tmp->sibling)) {
		printCustomTree(tmp,0,0);
		tmp = tmp->sibling;
	}

	if (!tree_empty(tmp))
		printCustomTree(tmp,0,1);
}

//the wrapper of the recursive function
void tree_print_tree(tree* t) {
	printCustomTree(t,1,1);
	
}

//Prints in a really cool way some info on the tree node passed
void tree_print_info(tree* t) {
	printf("\t%sProcesso:%s %s \n\t%sProcess ID:%s %d.\n",BCYN,KNRM,t->name,BCYN,KNRM,t->pid);
	if (t->parent != NULL) {
		printf("\t%sProcess ID del padre:%s %d.\n",BCYN,KNRM,t->parent->pid);
	}
	printf("\t%sNumero di figli generati:%s %d.\n",BCYN,KNRM,t->child_number);
	printf("\t%sNumero di figli ancora attivi:%s %d.\n",BCYN,KNRM,tree_getNumberOfChildren(t));

	if (t->depth != 0) {
		printf("\tSono un figlio di %s%da%s generazione.\n",BGRN,t->depth,KNRM);
	} else {
		printf("\tSono il %spmanager%s.\n",BGRN,KNRM);
	}
}

//Prints in another really cool way the structure of the tree, with all
//the node divided by their tree level.
void tree_print_list(tree* t) {
	list *q = (list*) malloc(sizeof(list));
	q->element = t;
	q->next = NULL;
	int last_depth = t->depth;

	printf("\n\t%sPMANAGER%s\n",BYEL,KNRM);

	while (q != NULL) {
		tree* tmp = q->element;

		tree* tmp_insert = tmp->child;
		list* tmpq = q;

		while (tmp_insert != NULL) {
			//if (tmp_insert->active == 1) {
				while(tmpq->next != NULL)
					tmpq = tmpq->next;

				tmpq->next = (list*) malloc(sizeof(list));
				tmpq->next->element = tmp_insert;
				tmpq->next->next = NULL;

				tmp_insert = tmp_insert->sibling;
			//}
		}

		if (tmp->active == 1) {
			if (last_depth == tmp->depth-1) {
				printf("\n\t%sFIGLI DELLA %da GENERAZIONE:%s\n",BYEL,tmp->depth,KNRM);
				last_depth = tmp->depth;
			}

			printf("\t%sNome processo:%s %s - %sProcess ID:%s %d.\n", BCYN,KNRM,tmp->name,BCYN,KNRM, tmp->pid);
		}
		list* n = q->next;
		free(q);
		q = n;
	}
}
