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
unsigned int number_of_clients = 0;
int *clients_list;
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
	printf("READ CLIENTS PIDS\n");
	FILE * fp;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
	int i = 0;

	fp = fopen("clients.txt", "r");
	if (fp == NULL)
		exit(1);

	while ((read = getline(&line, &len, fp)) != -1) {
    		number_of_clients++;
	}

    	fclose(fp);
	clients_list = (int*) malloc(number_of_clients * sizeof(unsigned int));
	fp = fopen("clients.txt", "r");

	
	while ((read = getline(&line, &len, fp)) != -1) {
		clients_list[i] = atoi(line);
		i++;
	}

	fclose(fp);
	for(i = 0; i < number_of_clients; i++)
		printf("Client id: %d\n", clients_list[i]);
    	if (line)
        	free(line);
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
		if(number_of_clients == 0)
		{
			printf("SIGUSR2\n");
			read_clients_pids();
		}
		else
		{
			printf("SIGUSR2 number of clients: %d\n", number_of_clients);
			printf("Dispatcher process with PID: %d received following message:\n", getpid());
			printf("Message id: %d\n", msg->id);
			printf("Message contents: %s\n", msg->contents);
		}
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
	static struct sigaction client_action;
	prepare_shm(&msg, &key, &shmid);

     	sa.sa_sigaction = *signal_request_handler;
	sa.sa_flags |= SA_SIGINFO;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGUSR1, &sa, NULL);

	client_action.sa_sigaction = *signal_request_handler;
	client_action.sa_flags |= SA_SIGINFO;
	sigemptyset(&client_action.sa_mask);
	sigaction(SIGUSR2, &client_action, NULL);

	while(!stop)
		;

        shmdt(msg);
	shmctl(shmid, IPC_RMID, NULL);
        exit(0);
}
