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
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include "map.h"
#include "tree.h"
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

#define TRUE 1
#define FALSE 0
#define ERROR -1

#define COMMAND_LENGTH 50	//Lunghezza stringa comando

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

char commands[9][7]={"phelp","quit","plist","pnew","pinfo","pclose","pspawn","prmall","ptree"};	//Array di comandi utilizzabili (9 stringhe)
tree* tree_manager;	//Albero di gestione dei processi
map map_manager;	//Mappa per la gestione dei nome-ProcessID
const char fifo_name[] = "fifo/FIFO_SO_PROJECT";	//Fifo

/**
 * Funzioni che contengono l'implementazione dei comandi
 * Dove necessario si prende in input una stringa che contiene
 * il nome del processo su cui operare
*/
int phelp_f();
int plist_f();
int pnew_f(char* name);
int pinfo_f(char* name);
int pclose_f(char* name);
int pspawn_f(char* name);
int prmall_f(char* name);
int ptree_f();
void quit_f();

/**
 *	Funzioni gestione stringhe
*/
	/*
	 * int checkInput(int arguments_number, int command_num, char * token, char* argumentReturn);
	 * Controlla se la stringa di comando contiene il numero corretto di argomenti (0 oppure 1 in base al comando)
	 * e restituisce l'eventuale argomento
	 * Parametri:
	 * - arguments_number : Numero di parametri che ci si aspetta
	 * - command_num : Numero progressivo che rappresenta il comando (indice di commands[9])
	 * - token: token generato dalla funzione strtok di string.h
	 * - argumentReturn stringa contenente l'eventuale argomento (In base al comando)
	 *	Return:
	 * - FALSE: Se si è verificato un errore (Troppi argomenti o troppo pochi)
	 * - TRUE: Se non ci sono stati errori
	*/
int checkInput(int arguments_number, int command_num, char * token, char* argumentReturn);
void all_lowercase(char* word); //Rende lowercase ogni carattere alfabetico di word
int string_equals( char* first , char* second);	//Restituisce TRUE se le stringhe sono uguali, FALSE altrimenti

void overridden_tree_delete(tree **t);
void child_death_wait(int sig);

int main( int argc, char *argv[] ){
	signal(SIGURG, child_death_wait);

	tree_init(&tree_manager);
	tree* nmanager = tree_insert(&tree_manager,getpid(),"manager");

	map_init(&map_manager);
	map_add(&map_manager,nmanager->name,nmanager);

	//apro fifo per spawn
 	mknod(fifo_name,S_IFIFO,0);
 	chmod(fifo_name,0660);

	/**
	 * Flag che indica se il software legge comandi da terminale (STANDARD) oppure se si leggono
	 * da un file testuale (MODALITA' TEST)
	 * 0-STANDARD / 1-TEST
	*/
 	int file_flag = 0;

	/**	Fase di lettura degli argomenti */
	if(argc == 2){	//Se c'è esattamente un argomento viene aperto il file che corrisponde al path "argv[1]"
		file_flag = 1;	//file_flag = 1 significa che la modalità di test è attiva
		freopen(argv[1], "r", stdin); //Apre il file con path argv[1] e lo sostituisce allo standard input
	} else if( argc > 2 ){	//Se gli argomenti sono più di due c'è un errore
		printf("\tInseriti troppi argomenti (Max 1) \n");
		return(1);
	}

	/**
	 *	HEADER SHELL
	*/
	printf("\n\t%s╔═════╗╔═════╗ ╔════╗╔═══╗ ╔════╗╔═════╗╔═════╗╔═════╗%s\n",KBLU,KNRM);
	printf("\t%s║ ═══ ║║ ║ ║ ║╔╩═══ ║║ ║ ║╔╩═══ ║║ ═══ ║║  ═══╣║  ╔══╝%s\n",KBLU,KNRM);
	printf("\t%s║  ╔══╝║ ║ ║ ║║ ═══ ║║ ║ ║║ ═══ ║╠═══  ╣║  ═══╣║  ║%s\n",KBLU,KNRM);
	printf("\t%s╚══╝   ╚═╩═╩═╝╚═════╝╚═╩═╝╚═════╝╚═════╝╚═════╝╚══╝%s\n\n",KBLU,KNRM);

	/**
	 *	Fase di lettura dei comandi da shell
	*/
	char command[COMMAND_LENGTH];	//Stringa che deve contenere il comando letto
	int command_num = -1; //Codice riferito al comando letto

	do{
		printf("%s> %s",BGRN,KNRM);	//Stampa la parentesi uncinata come indicatore nel terminale
		fgets(command,COMMAND_LENGTH,stdin);	//Prende tutta la stringa dall'input fino al backspace

		if(file_flag == 1) //Se il programma è in modalità test bisogna stampare il comando letto dal file
			printf("%s%s %s",BGRN,command,KNRM);

		/*
		 * Se il "comando inserito" è un semplice carattere di backspace
		 * le successive 4 righe di codice risolvono un problema di fgets
		 * che restituirebbe una stringa che porta ad un segmentation fault
		*/
		if(strlen(command) <= 1){
			command[0] = '\0';
			strcpy(command,	"space\n"); 	//Space è un "comando" aggiuntivo che manda semplicemente a capo il terminale
		}

		/**
		 *	Sostituzione dell'invio al termine della stringa con il carattere terminatore '\0'
		*/
		int found_flag = FALSE;
		for(int i = 0;(i < COMMAND_LENGTH) && (found_flag == FALSE); i++){
			if(command[i] == '\n'){
				command[i] = '\0';				//Sostituisce l'invio con il terminatore della stringa
				found_flag = TRUE;
			}
		}

		all_lowercase(command);	//Conversione della stringa comando in lower case

		/**
		 *  Fase di riduzione della stringa del comando in tokens per dividere il comando da eventuali argomenti
		 *  Dopo l'esecuzione di questa funzione i tokens sono stringhe che contengono una singola parola (ognuno)
		 *  della stringa di comando. Il carattere separatore delle parole è un blank
		*/
		char* token = strtok(command, " ");	//Tokenizer, divide in tokens la stringa dove trova blank

		//Controlla se il primo token corrisponde a uno dei comandi nell'array di comandi e assegna command num al valore corispondente
		for (int j = 0; j < 9; ++j)
			if (string_equals(token,commands[j]) == TRUE)
				command_num = j;	//Per ogni comando, se la stringa è uguale ad esso, command num viene impostato all'indice di command[]
		//Se invece il token equivale al "comando aggiuntivo" space, si setta command
		if(string_equals(token,"space"))
			command_num = 9; //SPACE command

		/**
		 * Switch-case fondamentale
 		 * In base a command_num viene chiamata la funzione corrispondente
 		 * facendo controllo di errore sugli argomenti dove necessario
		*/
		switch(command_num){
			case 0:{ //phelp
				if(checkInput(0,command_num,token,NULL)){
					if(phelp_f() == FALSE){
						fprintf(stderr,"\t%sERRORE:%s comportamento inaspettato della funzione phelp\n",BRED,KNRM);
						return 1;
					}
				}	//Se checkInput ritorna FALSE non bisogna terminare il programma poichè si tratta di un errore non bloccante e già segnalato dalla funzione via terminale
				break;
			}
			case 1:{ //quit
				if(checkInput(0,command_num,token,NULL)){
                    quit_f();
					return 0;
				}	//Se checkInput ritorna FALSE non bisogna terminare il programma poichè si tratta di un errore non bloccante e già segnalato dalla funzione via terminale
				break;
			}
			case 2:{ //plist
				if(checkInput(0,command_num,token,NULL)){
					if(plist_f() == FALSE){
						fprintf(stderr,"\t%sERRORE:%s comportamento inaspettato della funzione plist\n",BRED,KNRM);
						return 1;
					}
				}	//Se checkInput ritorna FALSE non bisogna terminare il programma poichè si tratta di un errore non bloccante e già segnalato dalla funzione via terminale
				break;
			}
			case 3:{ //pnew
				char argumentReturn[COMMAND_LENGTH];
				if(checkInput(1,command_num,token,argumentReturn)){
					if(pnew_f(argumentReturn) == FALSE){
						fprintf(stderr,"\t%sERRORE:%s comportamento inaspettato della funzione pnew\n",BRED,KNRM);
						return 1;
					}
				}	//Se checkInput ritorna FALSE non bisogna terminare il programma poichè si tratta di un errore non bloccante e già segnalato dalla funzione via terminale
				break;
			}
			case 4:{ //pinfo
				char argumentReturn[COMMAND_LENGTH];
				if(checkInput(1,command_num,token,argumentReturn)){
					if(pinfo_f(argumentReturn) == FALSE){
						fprintf(stderr,"\t%sERRORE:%s comportamento inaspettato della funzione pinfo\n",BRED,KNRM);
						return 1;
					}
				}	//Se checkInput ritorna FALSE non bisogna terminare il programma poichè si tratta di un errore non bloccante e già segnalato dalla funzione via terminale
				break;
			}
			case 5:{ //pclose
				char argumentReturn[COMMAND_LENGTH];
				if(checkInput(1,command_num,token,argumentReturn)){
					if(pclose_f(argumentReturn) == FALSE){
						fprintf(stderr,"\t%sERRORE:%s comportamento inaspettato della funzione pclose\n",BRED,KNRM);
						return 1;
					}
				}	//Se checkInput ritorna FALSE non bisogna terminare il programma poichè si tratta di un errore non bloccante e già segnalato dalla funzione via terminale
				break;
			}
			case 6:{ //pspawn
				char argumentReturn[COMMAND_LENGTH];
				if(checkInput(1,command_num,token,argumentReturn)){
					if(pspawn_f(argumentReturn) == FALSE){
						fprintf(stderr,"\t%sERRORE:%s comportamento inaspettato della funzione pspawn\n",BRED,KNRM);
						return 1;
					}
				}	//Se checkInput ritorna FALSE non bisogna terminare il programma poichè si tratta di un errore non bloccante e già segnalato dalla funzione via terminale
				break;
			}
			case 7:{ //prmall
				char argumentReturn[COMMAND_LENGTH];
				if(checkInput(1,command_num,token,argumentReturn)){
					if(prmall_f(argumentReturn) == FALSE){
						fprintf(stderr,"\t%sERRORE:%s comportamento inaspettato della funzione prmall\n",BRED,KNRM);
						return 1;
					}
				}	//Se checkInput ritorna FALSE non bisogna terminare il programma poichè si tratta di un errore non bloccante e già segnalato dalla funzione via terminale
				break;
			}
			case 8:{ //ptree
				if(checkInput(0,command_num,token,NULL)){
					if(ptree_f() == FALSE){
						fprintf(stderr,"\t%sERRORE:%s comportamento inaspettato della funzione ptree\n",BRED,KNRM);
						return 1;
					}
				}	//Se checkInput ritorna FALSE non bisogna terminare il programma poichè si tratta di un errore non bloccante e già segnalato dalla funzione via terminale
				break;
			}
			case 9:break;	//spazio, non viene eseguito niente
			default:{	//CASE di default, la stringa non è valida
				printf("\t%sATTENZIONE:%s Stringa non valida,reinserire.%s\n\tPer ulteriori informazioni sull' utilizzo di questo programma digitare %s\"phelp\"%s\n",BYEL,KYEL,KNRM,BNRM,KNRM);
				break;
			}
		}

		//reset dei dati di lettura parziali
		strcpy(token,"");
		strcpy(command,"");
		command_num = -1;

		if(file_flag == 1)	//Se il programma è in modalità test si inserisce un delay tra i comandi per rendere più leggibile l'output
			usleep(300000);

	}while(1);
	return 0;
}

/**
 *	FUNZIONI DELLA SHELL
*/

int phelp_f(){	//Descrizione dei comandi utilizzabili
	printf("\n\t%sPMANAGER 0.0.1\n",BYEL);
	printf("\t%sUsage:\t\t%scommand [parameters]\n",BNRM,KNRM);
	printf("\t%squit​:\t\t%sesce dalla shell custom\n",BNRM,KNRM);
	printf("\t%sphelp:\t\t%sstampa questa guida\n",BNRM,KNRM);
	printf("\t%splist:\t\t%selenca i processi generati da questo programma\n",BNRM,KNRM);
	printf("\t%spnew %s<nome>%s:\t%screa un nuovo processo con nome %s<nome>%s\n",BNRM,BGRN,BNRM,KNRM,BGRN,KNRM);
	printf("\t%spinfo %s<nome>​%s:\t%sfornisce informazioni sul processo %s<nome>%s\n",BNRM,BGRN,BNRM,KNRM,BGRN,KNRM);
	printf("\t%spclose %s<nome>%s​:\t%schiede al processo %s<nome>%s di chiudersi\n",BNRM,BGRN,BNRM,KNRM,BGRN,KNRM);
	printf("\t%spspawn %s<nome>%s:\t%schiede al processo %s<nome>%s di clonarsi creando %s<nome_i>%s, con i progressivo\n",BNRM,BGRN,BNRM,KNRM,BGRN,KNRM,BGRN,KNRM);
	printf("\t%sprmall %s<nome>%s:\t%schiedi al processo %s<nome>%s di chiudersi chiudendo anche eventuali cloni\n",BNRM,BGRN,BNRM,KNRM,BGRN,KNRM);
	printf("\t%sptree:\t\t%smostra la gerarchia completa dei processi generati attivi\n\n",BNRM,KNRM);
	return TRUE;
}

/**
 *
*/
int plist_f(){
	printf("\n\tRichiesta di informazioni su tutti i processi attivi\n");
	tree_print_list(tree_manager);	//Funzione di stampa dell'albero di gestione
	printf("\n");
	return TRUE;
}

/**
 *	Funzione di chiusura del terminale
 *  Prima di chiudere il programma si chiudono tutti i processi generati e la fifo viene scollegata
*/
void quit_f() {
	int toclose = tree_getNumberOfChildren(tree_manager);
	tree *t, *tmp;
	t = tree_manager->child;
	for(int i = 0; i < toclose; i++)
	{
		tmp = t;
		t = t->sibling;
		overridden_tree_delete(&tmp);
	}
    unlink(fifo_name);
}
/**
 *
*/
int pnew_f(char* name){
	printf("\n\tRichiesta di creazione nuovo processo con nome %s\"%s\"%s.\n",BBLU,name,KNRM);
	if(strcmp(name,"manager") == 0 || strcmp(name,"pmanager") == 0)	//controlla che non si voglia dare il nome del manager
 	{
 		fprintf(stderr, "\t%sERRORE:%s il nuovo processo non può avere il nome del manager.\n",BRED,KNRM);
 		return TRUE;
 	}
	if (map_lookup(map_manager,name) == NULL) {	//controlla che il nome sia "libero"
		int fd[2];
		pipe(fd);	//pipe anonima per sincronizzazione
		int f = fork();
		if(f < 0)
		{
			fprintf(stderr, "\t%sERRORE:%s processo non generato, errore durante il fork.\n",BRED,KNRM);
			return FALSE;
		}
		else if(f == 0)	//figlio
		{
			close(fd[0]);
			char pipe_child[10];	//buffer per contenere l'id della pipe
			sprintf(pipe_child, "%d", fd[1]);	//conversione in stringa dell'id della pipe
			char *const parmList[] = {"processo", pipe_child ,NULL};	//array di argomenti da passare ad exec, contenente l'id della pipe
			execv("./processo",parmList);
			return FALSE;	//non dovrebbe mai essere eseguito
		}
		else	//padre (manager)
		{
			char message[2];	//buffer per ricevere il messaggio di sincronizzazione
			close(fd[1]);
			read(fd[0], message, 2);	//aspetto il messaggio di sincronizzazione
			close(fd[0]);
			tree* added = tree_insert(&tree_manager,f,name);	//aggiungo il nuovo processo all'albero
			map_add(&map_manager,name,added);	//aggiungo il nuovo processo alla mappa
			printf("\tIl processo %s\"%s\"%s e' stato creato con successo.\n",BBLU,name,KNRM);
		}


		return TRUE;
	} else {
		//il processo "nome" esiste gia'
		printf("\t%sERRORE:%s il processo %s\"%s\"%s esiste gia'.\n",BRED,KNRM,BBLU,name,KNRM);
		return TRUE; //in questo caso si ritorna true perchè l'errore NON DEVE ESSERE BLOCCANTE ed è già stato segnalato sopra
	}
}

/**
 *
*/
int pinfo_f(char* name){
	printf("\n\tRichiesta di informazioni sul processo con nome %s\"%s\"%s.\n",BBLU,name,KNRM);

	tree* toprint = map_lookup(map_manager,name);
	if (toprint != NULL) {
		//stampo, il processo esiste
		printf("\tStampo le informazioni relative al processo %s\"%s\"%s.\n\n",BBLU,name,KNRM);
		tree_print_info(toprint);
		printf("\n");
		return TRUE;
	} else {
		//non esiste il processo "nome"
		printf("\t%sERRORE:%s il processo %s\"%s\"%s non esiste.\n",BRED,KNRM,BBLU,name,KNRM);
		return TRUE;
	}
}

/**
 *
*/
int pclose_f(char*name){
	printf("\n\tRichiesta di chiusura processo con nome %s\"%s\"%s\n",BBLU,name,KNRM);
	if(strcmp(name,"manager") == 0 || strcmp(name,"pmanager") == 0)
 	{
 		fprintf(stderr, "\t%sERRORE:%s per chiudere il pmanager utilizzare il comando %s\"quit\"%s.\n",BRED,KNRM,BNRM,KNRM);
 		return TRUE;
 	}
	tree* toremove = map_lookup(map_manager,name);
	if (toremove != NULL) {
		//provo a rimuovere, il processo esiste
		printf("\tTentativo di rimozione del processo %s\"%s\"%s\n",BBLU,name,KNRM);
		if (tree_remove(toremove) == 1){
			map_remove(&map_manager,name);
			printf("\tProcesso %s\"%s\"%s rimosso con successo\n",BBLU,name,KNRM);
		} else {
			printf("\t%sERRORE:%s processo %s\"%s\"%s non rimosso\n",BRED,KNRM,BBLU,name,KNRM);
		}
		return TRUE;
	} else {
		//il processo non esiste
		printf("\t%sERRORE:%s il processo %s\"%s\"%s non esiste.\n",BRED,KNRM,BBLU,name,KNRM);
		return TRUE;
	}
}

/**
 *
*/
int pspawn_f(char* name){
  	printf("\n\tRichiesta di clonazione del processo con nome %s\"%s\"%s\n",BBLU,name,KNRM);

 	if(strcmp(name,"manager") == 0 || strcmp(name,"pmanager") == 0)	//controllo che non si tenti di clonare il manager
 	{
 		fprintf(stderr, "\t%sERRORE:%s vietato clonare il manager\n",BRED,KNRM);
 		return TRUE;
 	}
 	tree *toclone = map_lookup(map_manager,name);	//ricerco il processo da clonare
 	if(toclone == NULL) //non esiste
 	{
 		fprintf(stderr, "\t%sERRORE:%s il processo %s\"%s\"%s non esiste.\n\tImpossibile Clonare\n",BRED,KNRM,BBLU,name,KNRM);
 		return TRUE;
 	}
 	else
 	{
 		char *newname = malloc(sizeof(char)*30);	//creo il nome del nuovo processo: proc_i
 		strcpy(newname,name);
 		int figli = toclone->child_number;
 		char tmp[10];
 		sprintf(tmp, "%d", figli);
 		strcat(newname, "_");
 		strcat(newname,tmp);						//fine creazione nome
 		int k = kill(toclone->pid, SIGUSR2);		//invio segnale di clonazione al processo
        if(k == -1) //errore nel kill
        {
            fprintf(stderr, "%sERRORE:%s fallita comunicazione con il processo %s\"%s\"%s (%s%d%s)\n",BRED,KNRM,BBLU,name,KNRM,BBLU,toclone->pid,KNRM);
            return FALSE;
        }
        else
        {
            printf("\tRichiesta di clonazione inviata al processo %s\"%s\"%s\n",BBLU,name,KNRM);
            int fd;
            do{
              	fd = open(fifo_name, O_RDONLY);		//apro fifo per comunicare col processo
            }while(fd == -1);
            char fromchild[10];
            if(read(fd, fromchild, sizeof(fromchild)) == -1) 	//leggo il pid del nuovo processo generato
            {
            	fprintf(stderr, "%sERRORE:%s fallita comunicazione con il processo %s\"%s\"%s (%s%d%s)\n",BRED,KNRM,BBLU,name,KNRM,BBLU,toclone->pid,KNRM);
                return FALSE;
            }
            int newpid;
            sscanf(fromchild, "%d", &newpid);
            tree *insered = tree_insert(&toclone,newpid,newname);	//inserisco il nuovo processo nell'albero
            map_add(&map_manager,newname,insered);					//inserisco il nuovo processo nella mappa
            //read for synch
            read(fd, fromchild, sizeof(fromchild));					//lettura dalla fifo per sincronizzazione con il processo
            close(fd);
        }
 	}
  	return TRUE;
 }

/**
 *
*/
int prmall_f(char* name){
	printf("\n\tRichiesta di chiusura ricorsiva sul processo con nome %s\"%s\"%s\n",BBLU,name,KNRM);
	if(strcmp(name,"manager") == 0 || strcmp(name,"pmanager") == 0)
 	{
 		fprintf(stderr, "\t%sERRORE:%s la chiusura del manager si attua con il comando %s\"quit\"%s\n",BRED,KNRM,BNRM,KNRM);
 		return TRUE;
 	}
	tree* todelete = map_lookup(map_manager,name);
	if (todelete != NULL) {
		//provo a rimuovere, il processo esiste
		overridden_tree_delete(&todelete);
		printf("\tProcesso %s\"%s\"%s e figli rimossi con successo\n",BBLU,name,KNRM);
		return TRUE;
	} else {
		//il processo non esiste
 		fprintf(stderr, "\t%sERRORE:%s il processo %s\"%s\"%s non esiste.\n",BRED,KNRM,BBLU,name,KNRM);
		return TRUE;
	}
}

/**
 *
*/
int ptree_f(){
	printf("\n\tRichiesta di informazioni sulla gerarchia dei processi\n\n");
	tree_print_tree(tree_manager);
	printf("\n");
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
				printf("\n\t%sERRORE%s: Il comando %s\"%s\"%s può avere un solo argomento\n",BRED,KNRM,BNRM,commands[command_num],KNRM);
				res = FALSE;
			}else{
				strcpy(argumentReturn,possibileTokenCorretto);
				res = TRUE;
			}

		} else {	//In questo caso è stato usato il comando senza argomento
			printf("\n\t%sERRORE%s: Il comando %s\"%s\"%s deve essere utilizzato con un argomento\n",BRED,KNRM,BNRM,commands[command_num],KNRM);
			res = FALSE;
		}
	} else if(arguments_number == 0){
		token = strtok(NULL, " "); //passa al token successivo
		if(token){
			printf("\n\t%sERRORE%s: Il comando %s\"%s\"%s non ammette argomenti\n",BRED,KNRM,BNRM,commands[command_num],KNRM);
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

void overridden_tree_delete(tree **t) {
	/*recursively call the delete on all the children of the node.
	the when a node has no children remove it. There is no need to
	search all the siblings because as you delete the child of a node,
	the siblings takes it place. (I love this function, is really magical)
	*/
	while (!tree_empty((*t)->child)) {
		overridden_tree_delete(&(*t)->child);
	}

	map_remove(&map_manager,(*t)->name);
	tree_remove(*t);
}


void child_death_wait(int sig) {
    int status;
    wait(&status);
}
