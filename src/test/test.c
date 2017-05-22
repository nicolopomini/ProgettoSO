#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

//Crea tre file di test:
// Uno normale ( senza casi strani )
// Uno con più casi strani e limite possibile
// Uno con esagerazione di processi


//Lista comandi
const char *commands[8]={"help","plist","pnew","pinfo","pclose","pspawn","prmall","ptree"}; 
const int DIM = 15; //Dimensione massima di un nome di un processo
const int NUM_COMMANDS = 20; //Numero massimo di comandi nel test
#define TRUE 1
#define FALSE 0
#define ERROR -1

int test1();
int test2();
int test3();
char* create_name(); //Restituisce un nome di lunghezza variabile (max DIM) 

int main(){
	if( test1() == 0 )
		printf("La creazione del test1 non e' andata a buon fine\n");
	printf("Bene bene\n");
	return 0;
}

int test1(){
	FILE *test1;	
	test1 = fopen("test1.txt", "w");
	srand(time(NULL));
	
	int r = rand();
	r = rand();
	
	for(int i=0 ; i < (r%20) ; i++){
		int command = rand()%8;
		if( command == 2 || command==3 || command==4 || command==5 || command ==6 ){
			fprintf(test1,"%s %s\n", commands[command], create_name());
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
		char letter = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"[rand() % 52];
		nome_processo[i]=letter;
	}
	return nome_processo;
}