#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

void signal_handler(int num)
{
	printf("Handler called.\n");
	exit(num);
}

void request_data()
{

}

int main(void)
{
	struct sigaction action;
	action.sa_handler = signal_handler;
	action.sa_flags = 0;
	sigemptyset(&action.sa_flags);
	printf("client.exe pid: %d\n", getpid());	
	return 0;
}
