#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>

#include "essentials.h"

int stop = 0;
message *msg;
unsigned int number_of_clients;

typedef struct message_queue
{
	int *clients_served;
	message msg;
	struct message_queue *next;
} message_queue;

void append_queue()
{

}

void read_clients_pids()
{

}

void prepare_shm(message **msg,
        	 key_t *key,
        	 int *shmid)
{
	get_key(key);
        get_shmid(shmid, *key);
        attach(msg, *shmid);
}


void signal_request_handler(int sig, siginfo_t *siginfo, void *context)
{
	if(sig == SIGUSR1)
	{
		printf("sigusr1\n");
		printf("Dispatcher process with PID: %d received following message:\n", getpid());
		printf("Message id: %d\n", msg->id);
		printf("Message contents: %s\n", msg->contents);
		printf("This signal was called by process id: %d\n", siginfo->si_pid);
	}
	else if(sig == SIGUSR2)
	{
		printf("Dispatcher process with PID: %d received following message:\n", getpid());
		printf("Message id: %d\n", msg->id);
		printf("Message contents: %s\n", msg->contents);
	}
	else if(sig == SIGTERM)
	{
		printf("SIGTERM sent\n");
		stop = 1;
	}	
}

int main(int argc, char *argv[])
{
        key_t key;
        int shmid;
	static struct sigaction sa;

	prepare_shm(&msg, &key, &shmid);

     	sa.sa_sigaction = *signal_request_handler;
	sa.sa_flags |= SA_SIGINFO;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGUSR1, &sa, NULL);
	while(!stop)
		;

        shmdt(msg);
	shmctl(shmid, IPC_RMID, NULL);
        exit(0);
}
