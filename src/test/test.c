#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "list.h"

//Crea tre file di test:
// Uno normale ( senza casi strani )
// Uno con più casi strani e limite possibile
// Uno con esagerazione di processi

/*TODO things:
Lista dei nomi (implementare list size) e fare ritornare [n]esimo elemento della lista string(?) n_elem_lista()
Creazione e modifica nomi pspawn
*/

//Lista comandi
const char *commands[8]={"phelp","plist","pnew","pinfo","pclose","pspawn","prmall","ptree"};
const int DIM = 15; //Dimensione massima di un nome di un processo
const int NUM_COMMANDS = 100; //Numero massimo di comandi nel test
//
int test1();
int test2();
int test3();
char* create_name(); //Restituisce un nome di lunghezza variabile (max DIM)

int main(){
	if( test1() == 0 )
		printf("La creazione del test1 non e' andata a buon fine\n");

	return 0;
}

int test1(){
	FILE *test1;
	test1 = fopen("test1.txt", "w");
	list *nomi= list_init();
	srand(time(NULL));

	int r = rand();
	r = rand();

	for(int i=0 ; i < (r%NUM_COMMANDS) ; i++){
		int command = rand()%8;
		if (command == 2){
			char* nome = create_name();
			add_name(&nomi, nome);
			fprintf(test1, "%s %s\n", commands[command], (list_lookup(nomi,nome))->name_process);
		}else if( command==3 || command==4 || command ==6 ){
			if(nomi==NULL){
				fprintf(test1,"%s %s\n", commands[command], create_name());

			}else{
				int n = rand()%list_size(nomi);
				fprintf(test1,"%s %s\n", commands[command], (n_elem_lista(nomi,n))->name_process);
			}
		}else if(command == 5){
			if(nomi==NULL){
				fprintf(test1,"%s %s\n", commands[command], create_name());
			}else{
			int n = rand()%list_size(nomi);
			fprintf(test1, "%s %s\n", commands[command], (n_elem_lista(nomi,n))->name_process);
			((n_elem_lista(nomi,n))->i)++;
			}
		}else{
			fprintf(test1,"%s\n", commands[command]);
		}
	}
	fprintf(test1,"quit\n");
	fclose(test1);
	return TRUE;
}

char* create_name(){
	char* nome_processo;
	int c= rand();
	nome_processo =(char*)malloc(c*sizeof(char));
	for( int i =0 ; i < c%DIM ; i++){
		char letter = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"[rand() % 52]; //Per la stringa utilizza soltanto una di queste lettere
		nome_processo[i]=letter;
	}
	return nome_processo;
}
