#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>

void signal_handler(int num)
{
	printf("Handler called.\n");
	exit(num);
}

int main(void)
{
	int child_pid;
	struct sigaction action;
	action.sa_handler = signal_handler;
	action.sa_flags = 0;
	sigemptyset(&action.sa_flags);
	
	if((child_pid = fork()) == 0)
	{
		while(1)
			sigaction(SIGUSR1, &action, NULL);
	}
	else
	{	
		printf("child pid: %d\n", child_pid);
		printf("Waiting: %d\n", wait(0));
	}
	return 0;
}
