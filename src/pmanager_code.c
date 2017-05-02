/*
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
#include "./pmanager_src.h"

#define TRUE 1
#define FALSE 0

char *commands[9]={"phelp","quit","plist","pnew","pinfo","pclose","pspawn","prmall","ptree"};

/**
*	Funzioni di gestione stringhe
*/

		//Trasforma tutta la stringa in lower case
		void all_lowercase(char* word){
			int i=0;
			while(word[i])
			{
				if(word[i]<'a')
					word[i] += 'a'-'A';
				i++;
			}
		}

		/**
		*	Compara le due stringhe passate come argomenti
		*	Risultato:
		*	1 (TRUE) - Stringhe uguali
		* 0 (FALSE)- Stringhe diverse
		*/
		int string_equals( char* first , char* second){
			if(strlen(first) != strlen(second))
				return FALSE;
			else
				for (int i = 0; i < strlen(second); ++i)
					if(first[i] != second[i]) return FALSE;
			return TRUE;
		}
/**			*/

int main( int argc, char *argv[] ){
	/**
	*	Fase di lettura degli argomenti
	*/
	if(argc == 2)	//Se c'è esattamente un argomento viene aperto il file che corrisponde al path "argv[1]"
		stdin = fopen( argv[1]);
	else if( argc > 2 ){
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
	char command[30];	//Stringa che deve contenere il comando letto
	int command_num = -1; //Codice riferito al comando letto


	do{
		printf("> ");
		scanf("%s", command);
  	all_lowercase(command);	//viene convertita la stringa comando in lower case
  	for (int j = 0; j < 9; ++j){
  		if (string_equals(command,commands[j]) == TRUE) {
  			command_num = j;	//Per ogni comando, se la stringa è uguale ad esso, command num viene impostato all'indice di command[]

		switch(command_num){
	  		case 0:{ //phelp
	  			printf("\nPMANAGER 0.0.1\n");
	  			printf("Usage: command [parameters]\n");
	  			printf("quit​ : esce dalla shell custom\n"
	  				"phelp: stampa questa guida\n"
					"plist: elenca i processi generati da questo programma\n"
  					"pnew <nome>: crea un nuovo processo con nome <nome>\n"
  					"pinfo <nome>​ : fornisce informazioni sul processo <nome>\n"
					"pclose <nome>​ : chiede al processo <nome> di chiudersi\n"
					"pspawn <nome> : chiede al processo <nome> di clonarsi creando <nome_i> con i progressivo\n"
					"prmall <nome> : chiedi al processo <nome> di chiudersi chiudendo anche eventuali cloni\n"
					"ptree : mostra la gerarchia completa dei processi generati attivi\n");
	  			break;
	  		}
		  	case 1:{ //quit
		  		break;
		  	}
		  	case 2:{ //plist
		  		break;
		  	}
		  	case 3:{ //pnew
		  		break;
		  	}
		  	case 4:{ //pinfo
		  		break;
		  	}
		  	case 5:{ //pclose
		  		break;
		  	}
		  	case 6:{ //pspawn
		  		break;
		  	}
		  	case 7:{ //prmall
		  		break;
		  	}
		  	case 8:{ //ptree
		  		break;
		  	}
		  	case 9:{ //Si potrebbe aggiungere qualcosa
		  		break;
		  	}
		  	default:{
		  		printf("\tStringa non valida reinserire.\nPer ulteriori informazioni sull' utilizzo di questo programma digitare phelp\n");
		  		break;
		  	}
		}

	}while( command_num != 1);
	return 0;
}
