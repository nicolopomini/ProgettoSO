#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
int main(){
	while(pause() == -1);
	return 0;
}
