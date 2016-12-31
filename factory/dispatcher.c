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


void client_request_handler(int sig, siginfo_t *siginfo, void *context)
{

}

int main(int argc, char *argv[])
{
	message *msg;
        key_t key;
        int shmid;
	prepare_shm(&msg, &key, &shmid);

	printf("Dispatcher process with PID: %d received following message:\n", getpid());
	printf("Message id: %d\n", msg->id);
	printf("Message contents: %s\n", msg->contents);

        shmdt(msg);
	shmctl(shmid, IPC_RMID, NULL);
        exit(0);
}
