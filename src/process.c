#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>

int pid_manager;
const char fifo_name[] = "fifo/FIFO_SO_PROJECT";
void handler_sigusr1(int signal) {
	kill(getpid(),SIGKILL);
}
void clone(int signal)
{
	int f = fork();
	if(f < 0)
	{
		fprintf(stderr, "Errore nella clonazione\n");
		exit(1);
	}
	else if(f == 0)
	{
		printf("Processo %d avviato\n", getpid());
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
		printf("Clonazione avvenuta: processo %d generato\n", f);
		kill(pid_manager,SIGUSR1);
	}
}
void external_close(int sig)
{}
int main(int argc, char *argv[]){
	signal(SIGUSR1,handler_sigusr1);
	signal(SIGUSR2,clone);
    signal(SIGTERM,external_close);
    signal(SIGINT,external_close);
    pid_manager = getppid();
	printf("Processo %d avviato\n", getpid());
	kill(pid_manager,SIGUSR1);
	while(pause() == -1);
	return 0;
}
