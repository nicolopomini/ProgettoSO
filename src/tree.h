#ifndef TREE_H
#define TREE_H

/* the main struct of this library.
It's a tree with parent/first_child/right_sibling pointers.
Since each node represent a process which is actually running,
it  contains as data the pid of the process, the name assigned
to the process by the user and his tree level.
*/
typedef struct tree_n tree;
typedef struct tree_n{
	//data
	int pid;
	char* name;
	int depth;
	int child_number;
	
	//tree references
	tree* parent;
	tree* sibling;
	tree* child;
} tree;

/* It's a secondary struct used for the tree_print_list function
All the operation performed by a queue are not implemented, so here we 
have just a list implemented, which is however treated and used like a queue.
*/
typedef struct list_n list;
typedef struct list_n {
	tree* element;
	list* next;
} list;

//initialize the tree.
void tree_init(tree **t);

//returns 1 if the tree which is passed is empty
int tree_empty(const tree *t);

/*It creates a new node with the pid and the name passed
and it assigns it as child of the tree passed. If the tree passed
is empty the created node takes his place. returns a pointer to the new node
*/
tree* tree_insert(tree **t, int pid, char* name);

//It remove the tree node passed as argument from his parent and returns 1.
//It the tree passed has children, nothing happens.
int tree_remove(tree *t);

// It deletes the tree passed, in a recursive way.
void tree_delete(tree **t);

//returns a int that indicates the number of actual (direct) children.
int tree_getNumberOfChildren(tree *t);

/* Some Good Looking Output Functions
print_tree: prints the tree in a sort of genealogical tree of all the processes 
			starting from the tree passed as argument
print_info: prints the information of the specific node passed as argument
print_list: prints a list of all the process active in the pmanager, divided by their 
			level in the tree
*/
void tree_print_tree(tree* t);
void tree_print_info(tree* t);
void tree_print_list(tree* t);

#endif