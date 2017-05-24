#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdarg.h>

#include "tree.h"

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
int tree_remove(tree *t) {
	if (tree_empty(t->child)) {
		if (kill(t->pid,SIGUSR1) == 0) {
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
			//failed to remove, because the signal wasn't delivered in the correct way. 
			return 0;
		}
	} else {
		//failed to remove, because the node has children.
		return 0;
	}
}

void tree_delete(tree **t) {
	/*recursively call the delete on all the children of the node.
	the when a node has no children remove it. There is no need to 
	search all the siblings because as you delete the child of a node,
	the siblings takes it place. (I love this function, is really magical)
	*/
	while (!tree_empty((*t)->child)) {
		tree_delete(&(*t)->child);
	}
	tree_remove(*t);
}

int tree_getNumberOfChildren(tree *t) {
	int child_count = 0;
	tree* tmp = t->child;
	while (!tree_empty(tmp)) {
		tmp = tmp->sibling;
		child_count++;
	}

	return child_count;
}

//a recursive function for printing the tree.
//First setted indicates that the node is the root (used only on the first call)
//Second setted means that the node is the last child, so a different box shape is printed
void printCustomTree(tree *t, int first, int last) {
	if (first == 1) {
		printf(" %s\n",t->name);
	} else {
		for (int i = 0; i < t->depth-1; ++i) {
			printf("  \u2502 ");	
		}

		if (last == 1) {
			printf("  \u2514\u2500%s\n",t->name);
		} else {
			printf("  \u251C\u2500%s\n",t->name);
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

void printNewTree(tree *t) {
	
}

//the wrapper of the recursive function
void tree_print_tree(tree* t) {
	//printCustomTree(t,1,1);
	
}

//Prints in a really cool way some info on the tree node passed
void tree_print_info(tree* t) {
	printf("Process: %s - ID: %d.\n", t->name, t->pid);
	if (t->parent != NULL) {
		printf("Parent Process ID: %d.\n", t->parent->pid);
	}
	if (t->depth != 0) {
		int module = t->depth%10;
		char *num = (char*) malloc(sizeof(char)*3);
		if (module == 1)
			strcpy(num,"st");
		else if (module == 2)
			strcpy(num,"nd");
		else if (module == 3)
			strcpy(num,"rd");
		else
			strcpy(num,"th");

		printf("I am a child of the %d%s generation.\n", t->depth,num);
	} else {
		printf("I am the pmanager.\n");
	}

	printf("Number of actual direct children: %d.\n", tree_getNumberOfChildren(t));
}

//Prints in another really cool way the structure of the tree, with all
//the node divided by their tree level.
void tree_print_list(tree* t) {
	list *q = (list*) malloc(sizeof(list));
	q->element = t;
	q->next = NULL;
	int last_depth = t->depth;

	while (q != NULL) {
		tree* tmp = q->element;

		tree* tmp_insert = tmp->child;
		list* tmpq = q;

		while (tmp_insert != NULL) {
			while(tmpq->next != NULL)
				tmpq = tmpq->next;

			tmpq->next = (list*) malloc(sizeof(list));
			tmpq->next->element = tmp_insert;
			tmpq->next->next = NULL;

			tmp_insert = tmp_insert->sibling;
		}
		if (last_depth == tmp->depth-1) {
			
			int module = tmp->depth%10;
			char *num = (char*) malloc(sizeof(char)*3);
			if (module == 1)
				strcpy(num,"ST");
			else if (module == 2)
				strcpy(num,"ND");
			else if (module == 3)
				strcpy(num,"RD");
			else
				strcpy(num,"TH");

			printf("\nCHILDREN OF THE %d%s GENERATION:\n",tmp->depth,num);
			last_depth = tmp->depth;
		}

		printf("Process: %s - ID: %d.\n", tmp->name, tmp->pid);
		
		list* n = q->next;
		free(q);
		q = n;
	}
}
