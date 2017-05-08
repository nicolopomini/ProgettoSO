/**
*    ______  _______  _______  _______  _______  _______  _______  ______
*   |   __ \|   |   ||   _   ||    |  ||   _   ||     __||    ___||   __ \
*   |    __/|       ||       ||       ||       ||    |  ||    ___||      <
*   |___|   |__|_|__||___|___||__|____||___|___||_______||_______||___|__|
*
*   Progetto Sistemi Operativi 2016/2017 - PMANAGER software
*   Comandi:
*   phelp​ : stampa un elenco dei comandi disponibili
*   plist​ : elenca i processi generati dalla shell custom
*   pnew <nome>​ : crea un nuovo processo con nome <nome>
*   pinfo <nome>​ : fornisce informazioni sul processo <nome> (almeno ​ pid ​ e ​ ppid ​ )
*   pclose: esce dalla shell custom

	Da compilare gcc pmanager_code.c list.c map.c
	E prima compilare anche gcc process.c list.c -o processo
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include "list.h"
#include "map.h"

#define TRUE 1
#define FALSE 0
#define ERROR -1

#define COMMAND_LENGTH 30

char *commands[9]={"phelp","quit","plist","pnew","pinfo","pclose","pspawn","prmall","ptree"};

list children;
map names;

int phelp_f();	// "_f" perchè altrimenti pclose è in conflitto con una funzione pclose sulle pipe
int plist_f();
int pnew_f(char* name);
int pinfo_f(char* name);
int pclose_f(char* name);
int pspawn_f(char* name);
int prmall_f(char* name);
int ptree_f();


/**
 *	Funzioni gestione stringhe
*/
int checkInput(int arguments_number, int command_num, char * token, char* argumentReturn);
void all_lowercase(char* word);
int string_equals( char* first , char* second);



int main( int argc, char *argv[] ){
	list_init(&children);
	map_init(&names);
	/**
	 * 	Fase di lettura degli argomenti
	*/
	if(argc == 2){	//Se c'è esattamente un argomento viene aperto il file che corrisponde al path "argv[1]"
		/*FILE * testFile;
		testFile = fopen (argv[1],"r");*/	//Funziona, ma dà warning per variabile non utilizzata
	} else if( argc > 2 ){
		printf("Inseriti troppi argomenti (Max 1) \n");
		return(1);
	}

	/**
	 *	HEADER SHELL
	*/
	printf("\n\t*-*-*-*-*-*-*-*-*\n\tPMANAGER software\n\t*-*-*-*-*-*-*-*-*\n");

	/**
	 *	Fase di lettura dei comandi da shell
	*/
	char command[COMMAND_LENGTH];	//Stringa che deve contenere il comando letto
	int command_num = -1; //Codice riferito al comando letto
	//char nome_processo[50];

	do{
		printf("> ");
		fgets(command,COMMAND_LENGTH,stdin);	//Prende tutto l'input fino all'invio

		//BISOGNA RISOLVERE BUG:
		//Quando si inserisce lo spazio c'è un bug

		if(strlen(command) <= 1){	//SE IL COMANDO NON E' SOLO UN INVIO
			command[0] = '\0';
			strcpy(command,	"space\n");
		}

		int found_flag = FALSE;
		for(int i = 0;(i < COMMAND_LENGTH) && (found_flag == FALSE); i++){
			if(command[i] == '\n'){
				command[i] = '\0';				//Sostituisce l'invio con il terminatore della stringa
				found_flag = TRUE;
			}
		}

		all_lowercase(command);	//viene convertita la stringa comando in lower case

		/**
		 *  Fase di riduzione della stringa del comando in tokens per dividere il comando da eventuali argomenti
		*/
		char* token = strtok(command, " ");	//Tokenizer, divide in tokens la stringa dove trova blank

		for (int j = 0; j < 9; ++j)
			if (string_equals(token,commands[j]) == TRUE)
				command_num = j;	//Per ogni comando, se la stringa è uguale ad esso, command num viene impostato all'indice di command[]
		if(string_equals(token,"space"))
			command_num = -1;

		switch(command_num){
			case -1:break;	//spazio
			case 0:{ //phelp
				if(checkInput(0,command_num,token,NULL)){
					if(phelp_f() == FALSE){
							fprintf(stderr, "%s", "ERROR in phelp function\n");
							return 1;
					}
				}
				break;
			}
			case 1:{ //quit
				if(checkInput(0,command_num,token,NULL)){
					return 0;
				}
				break;
			}
			case 2:{ //plist
				if(checkInput(0,command_num,token,NULL)){
					if(plist_f() == FALSE){
							fprintf(stderr, "%s", "ERROR in plist function\n");
							return 1;
					}
				}
				break;
			}
			case 3:{ //pnew
				char argumentReturn[COMMAND_LENGTH];
				if(checkInput(1,command_num,token,argumentReturn)){
					if(pnew_f(argumentReturn) == FALSE){
						fprintf(stderr, "%s", "ERROR in pnew function\n");
						return 1;
					}
				}
				break;
			}
			case 4:{ //pinfo
				char argumentReturn[COMMAND_LENGTH];
				if(checkInput(1,command_num,token,argumentReturn)){
					if(pinfo_f(argumentReturn) == FALSE){
						fprintf(stderr, "%s", "ERROR in pinfo function\n");
						return 1;
					}
				}
				break;
			}
			case 5:{ //pclose
				char argumentReturn[COMMAND_LENGTH];
				if(checkInput(1,command_num,token,argumentReturn)){
					if(pclose_f(argumentReturn) == FALSE){
						fprintf(stderr, "%s", "ERROR in pclose function\n");
						return 1;
					}
				}
				break;
			}
			case 6:{ //pspawn
				char argumentReturn[COMMAND_LENGTH];
				if(checkInput(1,command_num,token,argumentReturn)){
					if(pspawn_f(argumentReturn) == FALSE){
						fprintf(stderr, "%s", "ERROR in pspawn function\n");
						return 1;
					}
				}
				break;
			}
			case 7:{ //prmall
				char argumentReturn[COMMAND_LENGTH];
				if(checkInput(1,command_num,token,argumentReturn)){
					if(pspawn_f(argumentReturn) == FALSE){
						fprintf(stderr, "%s", "ERROR in pspawn function\n");
						return 1;
					}
				}
				break;
			}
			case 8:{ //ptree
				if(checkInput(0,command_num,token,NULL)){
					if(ptree_f() == FALSE){
						fprintf(stderr, "%s", "ERROR in ptree function\n");
						return 1;
					}
				}
				break;
			}
			case 9:{ //Si potrebbe aggiungere qualcosa
				break;
			}
			default:{
				printf("\nStringa non valida,reinserire.\nPer ulteriori informazioni sull' utilizzo di questo programma digitare phelp\n");
				break;
			}
		}
	}while(1);
	return 0;
}

/**
 *	FUNZIONI DELLA SHELL
*/

int phelp_f(){

	printf("\n\nPMANAGER 0.0.1\n");
	printf("Usage: command [parameters]\n");
	printf("quit​ : esce dalla shell custom\n"
		"phelp: stampa questa guida\n"
	"plist: elenca i processi generati da questo programma\n"
		"pnew <nome>: crea un nuovo processo con nome <nome>\n"
		"pinfo <nome>​ : fornisce informazioni sul processo <nome>\n"
	"pclose <nome>​ : chiede al processo <nome> di chiudersi\n"
	"pspawn <nome> : chiede al processo <nome> di clonarsi creando <nome_i> con i progressivo\n"
	"prmall <nome> : chiedi al processo <nome> di chiudersi chiudendo anche eventuali cloni\n"
	"ptree : mostra la gerarchia completa dei processi generati attivi \n\n");
	return TRUE;
}



/**
 *
*/
int plist_f(){
	printf("Chiamato plist\n");
	if(list_empty(children))
		printf("Manager non ha generato nessun processo\n");
	else
	{
		printf("Manager ha generato i seguenti processi:\n");
		list now = children;
		while(now != NULL)
		{
			printf("\t%d\n", now->pid);
			now = now->next;
		}
	}
	return TRUE;
}

/**
 *
*/
int pnew_f(char* name){
	printf("Chiamato pnew con nome \"%s\"\n",name);
	printf("Richiesta di creazione di un nuovo processo con nome \"%s\"\n", name);
	int f = fork();
	if(f < 0)
		return FALSE;
	else if(f == 0)
	{
		execv("./processo",NULL);
		return FALSE;	//non dovrebbe mai essere eseguito
	}
	else
	{
		list_insert(&children, f);
		map_add(&names, name, f);
	}
	return TRUE;
}

/**
 *
*/
int pinfo_f(char* name){
	printf("Chiamato pinfo con nome \"%s\"\n",name);
	return TRUE;
}

int pclose_f(char*name){
	printf("Chiamato pclose con nome \"%s\"\n",name);

	return TRUE;
}

/**
 *
*/
int pspawn_f(char* name){
	printf("Chiamato pspawn con nome \"%s\"\n",name);

	return TRUE;
}

/**
 *
*/
int prmall_f(char* name){
	printf("Chiamato prmall con nome \"%s\"\n",name);

	return TRUE;
}

/**
 *
*/
int ptree_f(){
	printf("Chiamato ptree\n");

	return TRUE;
}


int checkInput(int arguments_number, int command_num, char * token, char* argumentReturn){
	int res = FALSE;

	if(arguments_number == 1){
		token = strtok(NULL, " "); //passa al token successivo
		if(token){
			char possibileTokenCorretto[COMMAND_LENGTH];
			strcpy(possibileTokenCorretto,token);
			token = strtok(NULL, " "); //passa all'eventuale token successivo

			if(token){
				printf("ERRORE: Il comando %s può avere un solo argomento\n",commands[command_num]);
				res = FALSE;
			}else{
				strcpy(argumentReturn,possibileTokenCorretto);
				res = TRUE;
			}

		} else {	//In questo caso è stato usato il comando senza argomento
			printf("ERRORE: Il comando %s deve essere utilizzato con un argomento\n",commands[command_num]);
			res = FALSE;
		}
	} else if(arguments_number == 0){
		token = strtok(NULL, " "); //passa al token successivo
		if(token){
			printf("ERRORE: Il comando %s non ammette argomenti\n",commands[command_num]);
			res = FALSE;
		}else{
			argumentReturn = NULL;
			res = TRUE;
		}
	}
	return res;
}




/**
 *	Funzioni di gestione stringhe
*/

/**
 * Trasforma tutta la stringa in lower case
*/
void all_lowercase(char* word){
	int i=0;
	while(word[i])
	{
		if((word[i] >= 'A')&& (word[i] <= 'Z'))
			word[i] += 'a'-'A';
		i++;
	}
}

/**
 *	Compara le due stringhe passate come argomenti
 *	Risultato:
 *	1 (TRUE) - Stringhe uguali
 *  0 (FALSE)- Stringhe diverse
*/
int string_equals( char* first , char* second){
	if(strlen(first) != strlen(second))
		return FALSE;
	else
		for (int i = 0; i < strlen(second); ++i)
			if(first[i] != second[i]) return FALSE;
	return TRUE;
}
