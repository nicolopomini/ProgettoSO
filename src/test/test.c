#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "list.h"

//Crea tre file di test:
// Test1 nomi e funzioni random
// Test2 Numero alto di processi creati


/*TODO things:
Fixare l'abort per numeri processi alti, fare il delite dalla lista quando si chiama il comando delite <nome>
*/

//Lista comandi
const char *commands[8]={"phelp","plist","pnew","pinfo","pclose","pspawn","prmall","ptree"};
const int DIM = 15; //Dimensione di un nome di un processo (senza gli spawn) 
const int NUM_COMMANDS = 100; //Numero massimo di comandi nel test
const int N_MAX_PROCESS = 10000; //Numero di processi da creare nel test2
//
int test1();
int test2();
char* create_name(); //Restituisce un nome di lunghezza variabile (max DIM)

int main(){

	list *prova = list_init();
	if( test2() == 0 )
		printf("La creazione del test non e' andata a buon fine\n");
	//printf("Bene bene\n");//printf("La creazione del test è andata a buon fine");
	return 0;
}

/*int test1(){
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
	fprintf(test1,"ptree\n");
	fprintf(test1,"quit\n");
	fclose(test1);
	return TRUE;
}*/

int test2(){
	FILE *test2;
	test2 = fopen("test2.txt", "w");
	list *nomi= list_init();
	srand(time(NULL));

	char *nom=create_name();
	add_name(&nomi,nom);
	fprintf(test2,"pnew %s\n", nom);
	
	int n_processi_da_creare = (rand()+100)%N_MAX_PROCESS;

	for(int i=0 ; i < n_processi_da_creare ; i++){
		int boh = rand()%2;
		if( boh==0 ){ //pnew: creo il nome e inserisco il nome nella lista
			char* nome = create_name();
			add_name(&nomi, nome);
			fprintf(test2, "%s %s\n", commands[2], (list_lookup(nomi,nome))->name_process);
		}else{ //pspwan: se lista vuota manda il comando con un nome a caso, altrimenti controlla quanti pspawnsono stati fatti su di lui
			if(nomi==NULL){
				fprintf(test2,"%s %s\n", commands[5], create_name());
			}else{
				int n = rand()%list_size(nomi); //Seleziono un elemento n dalla lista dei nomi dei processi creati/spawnati
				fprintf(test2, "%s %s\n", commands[5], (n_elem_lista(nomi,n))->name_process);
				n_elem_lista(nomi,n)->i++; //Incremento il numero di processi spawnati da quel processo
				int trovato = FALSE;
				for(int j=0; j<=(n_elem_lista(nomi,n))->i && trovato == FALSE; j++ ){
					//Creo il nome con gli underscore e il numero di processi spawnati
					char* nome_spawnato=(char*)malloc((strlen(n_elem_lista(nomi,n)->name_process)+4)*sizeof(char));
					snprintf(nome_spawnato,strlen(n_elem_lista(nomi,n)->name_process)+1,"%s",((n_elem_lista(nomi,n))->name_process));
					nome_spawnato[strlen(nome_spawnato)]='_';
					char numero[3];
					snprintf(numero,3,"%i",j);
					strcat(nome_spawnato,numero);
					if(list_lookup(nomi,nome_spawnato) == NULL){
						add_name(&nomi,nome_spawnato);
						trovato=TRUE;
					}
				}
			}
		}
	}

	int r = rand();
	r = rand();

	for(int i=0 ; i < (r%NUM_COMMANDS) ; i++){
		int command = rand()%8;
		if (command == 2){
			char* nome = create_name();
			add_name(&nomi, nome);
			fprintf(test2, "%s %s\n", commands[command], (list_lookup(nomi,nome))->name_process);
		}else if( command==3 || command ==6 ){
			if(nomi==NULL){
				fprintf(test2,"%s %s\n", commands[command], create_name());
			}else{
				int n = rand()%list_size(nomi);
				fprintf(test2,"%s %s\n", commands[command], (n_elem_lista(nomi,n))->name_process);
			}
		}else if(command==4){
			if(nomi==NULL){
				fprintf(test2,"%s %s\n", commands[command], create_name());
			}else{
				int n = rand()%list_size(nomi);
				fprintf(test2,"%s %s\n", commands[command], (n_elem_lista(nomi,n))->name_process);
				delete_name(nomi,(n_elem_lista(nomi,n))->name_process);
			}
		}else if(command == 5){
			if(nomi==NULL){
				fprintf(test2,"%s %s\n", commands[command], create_name());
			}else{
				int n = rand()%list_size(nomi);
				fprintf(test2, "%s %s\n", commands[command], (n_elem_lista(nomi,n))->name_process);
				((n_elem_lista(nomi,n))->i)++;
			}
		}else{
			fprintf(test2,"%s\n", commands[command]);
		}
	}
	fprintf(test2,"ptree\n");
	fclose(test2);
	return TRUE;
}

char* create_name(){
	char* nome_processo;
	int c= rand();
	if(c==0){ c=1; }
	nome_processo =(char*)malloc(c*sizeof(char));
	for( int i =0 ; i < c%DIM ; i++){
		char letter = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"[rand() % 52]; //Per la stringa utilizza soltanto una di queste lettere
		nome_processo[i]=letter;
	}
	return nome_processo;
}
