#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void handler_sigusr1(int signal) {
	kill(getpid(),SIGKILL);
}

int main(){
	signal(SIGUSR1,handler_sigusr1);

	while(pause() == -1);
	return 0;
}
