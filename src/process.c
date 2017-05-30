#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

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

int pipe_creation;
const char fifo_name[] = "fifo/FIFO_SO_PROJECT";
void handler_sigusr1(int signal) {	//handler di SIGUSR1 per la chiusura del processo
	kill(getppid(),SIGURG);			//invio un segnale al padre di imminente chiusura
	kill(getpid(),SIGKILL);			//mi chiudo
}
void clone(int signal)			//handler di SIGUSR2 per la clonazione del processo
{
	int pipe_clone[2];			
	pipe(pipe_clone);			//pipe anonima per gestire la sincronizzazione tra il processo e il clone
	int f = fork();
	if(f < 0)
	{
		fprintf(stderr, "\t%sERRORE:%s processo non generato, errore durante il fork.\n", BRED, KNRM);
		exit(1);
	}
	else if(f == 0)			//clone
	{
		printf("\tProcesso %s%d%s avviato\n",BBLU,getpid(),KNRM);
		close(pipe_clone[0]);
		write(pipe_clone[1], "1", 2);	//terminato il suo avvio scrive sulla pipe perchè ha terminato
		close(pipe_clone[1]);
	}
	else //padre
	{
		close(pipe_clone[1]);
		char tosend[10];
		sprintf(tosend, "%d", f);
		int fd;
		do{
		fd = open(fifo_name, O_WRONLY);		//apre fifo per comunicare con il manager
		}while(fd == -1);
		write(fd, tosend, sizeof(tosend));	//invia al manager il pid del clone appena generato
		read(pipe_clone[0], tosend, 2);		//attende che il clone termini il suo avvio
		close(pipe_clone[0]);
		printf("\tClonazione avvenuta: processo %s%d%s generato\n",BBLU,f,KNRM);
		write(fd, "1", 2);					//invia messaggio al manager per sincronizzazione
		close(fd);
	}
}
void external_close(int sig) {}		//serve per evitare chiusure "esterne"

void child_death_wait(int sig) {	//handler per gestire la chiusura di un figlio, per evitare processi zombie
    int status;
    wait(&status);
}

int main(int argc, char *argv[]){
	signal(SIGUSR1,handler_sigusr1); 	//handler pclose
	signal(SIGUSR2,clone); 				//handler pspawn
	signal(SIGURG, child_death_wait);	//closing a child
    signal(SIGTERM,external_close);		//per evitare chiusure esterne
    signal(SIGINT,external_close);
    pipe_creation = atoi(argv[1]);		//pipe per sincronizzazione con il padre
	printf("\tProcesso %s%d%s avviato\n",BBLU,getpid(),KNRM);
	write(pipe_creation, "1", 2);		//invio segnale per sincronizzazione
	close(pipe_creation);
	while(pause() == -1);
	return 0;
}