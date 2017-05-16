#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
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
	}
	else //padre
	{
		char tosend[10], pipe_name[10];
		sprintf(tosend, "%d", f);
		sprintf(pipe_name, "%d", getpid());
		int fd;
		do{
		fd = open(pipe_name, O_WRONLY);
		}while(fd == -1);
		write(fd, tosend, sizeof(tosend));
		close(fd);
		printf("Clonazione avvenuta: processo %d generato\n", f);
	}
}
int main(){
	signal(SIGUSR1,handler_sigusr1);
	signal(SIGUSR2,clone);
	printf("Processo %d avviato\n", getpid());
	while(pause() == -1);
	return 0;
}
