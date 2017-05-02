/*
	Da compilare assieme a list.c e const.c
*/
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include "list.h"
#include "const.h"

list children;
int pid_manager = -1;	//progressivo: indice dei figli
/*
	Inizializza un nuovo processo
	Assegna il nuovo nome liberando la memoria di quello vecchio
	Azzera il progressivo
	Cancella la vecchia lista inizializzandone una nuova	
*/
void init()
{
	if(pid_manager == -1)
		pid_manager = getppid();
	if(!list_empty(children))
		list_delete(&children);
	list_init(&children);
	printf("Processo %d avviato.\n", getpid());
}
/*
	Clona il processo, assegnando al figlio il nome 'nomepadre_progressivo'
	Inserisce il processo nella lista del padre
*/
void clone()
{
	int f = fork();
	if(f < 0)
	{
		printf("Error during clonation\n");
		exit(1);
	}
	else if(f == 0) //figlio
	{
		//TODO: Comunicare al manager il mio pid e il mio nome !!!
		init();
	}
	else	//padre
	{
		list_insert(&children, f);
		printf("Processo %d generato.\n", f);
	}
}
int main()
{
	init();
	clone();
	return 0;
}