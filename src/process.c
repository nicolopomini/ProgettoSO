#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
char *pipe_name = "pspawn_pipe_SO2k17";
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
	else
	{
		char tosend[10];
		sprintf(tosend, "%d", f);
		mkfifo(pipe_name, 0666);
		int fd = open(pipe_name, O_WRONLY);
		write(fd,tosend,10);
		close(fd);
		unlink(pipe_name);
	}
}
int main(){
	signal(SIGUSR1,handler_sigusr1);
	signal(SIGUSR2,clone);
	while(pause() == -1);
	return 0;
}
