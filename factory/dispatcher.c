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

typedef struct 
{
	int clients_server[number_of_clients];
	message msg;
	message_queue *next;
} message_queue;

void append_queue()
{
	
}

void read_clients_pids()
{

}

int main(int argc, char *argv[])
{
	message *msg;
        key_t key;
        int shmid;
        int pid;
	get_key(&key);
        get_shmid(&shmid, key);
        attach(&msg, shmid);
	
	printf("Dispatcher process with PID: %d received following message:\n", getpid());
	printf("Message id: %d\n", msg->id);
	printf("Message contents: %s\n", msg->contents);

        shmdt(msg);
        exit(0);
}
