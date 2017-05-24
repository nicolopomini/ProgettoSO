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

int pid_manager;
const char fifo_name[] = "fifo/FIFO_SO_PROJECT";
void handler_sigusr1(int signal) {
	kill(getppid(),SIGURG);
	kill(getpid(),SIGKILL);
}
void clone(int signal)
{
	int f = fork();
	if(f < 0)
	{
		fprintf(stderr, "\t%sERRORE:%s processo non generato, errore durante il fork.\n", BRED, KNRM);
		exit(1);
	}
	else if(f == 0)
	{
		printf("\tProcesso %s%d%s avviato\n",BBLU,getpid(),KNRM);
		kill(pid_manager,SIGUSR1);

	}
	else //padre
	{
		char tosend[10];
		sprintf(tosend, "%d", f);
		int fd;
		do{
		fd = open(fifo_name, O_WRONLY);
		}while(fd == -1);
		write(fd, tosend, sizeof(tosend));
		close(fd);
		printf("\tClonazione avvenuta: processo %s%d%s generato\n",BBLU,f,KNRM);
		kill(pid_manager,SIGUSR1);
	}
}
void external_close(int sig) {}

void child_death_wait(int sig) {
    int status;
    wait(&status);
    //printf("Morto %d s:%d\n",wait(&status), status);
}

int main(int argc, char *argv[]){
	signal(SIGUSR1,handler_sigusr1);
	signal(SIGUSR2,clone);
	signal(SIGURG, child_death_wait);
    signal(SIGTERM,external_close);
    signal(SIGINT,external_close);
    pid_manager = getppid();
	printf("\tProcesso %s%d%s avviato\n",BBLU,getpid(),KNRM);
	kill(pid_manager,SIGUSR1);
	while(pause() == -1);
	return 0;
}