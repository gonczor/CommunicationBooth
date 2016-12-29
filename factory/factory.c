#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>

#include "essentials.h"

int id = 5;

void create_message(message* msg)
{
	msg->id = id++;
	strncpy(msg->contents, "Hello, world!", 1024);
	// msg->contents = 'a';
	msg->len =  strlen(msg->contents);
	printf("Factory process with PID: %d created following message:\n", getpid());
	printf("Message id: %d\n", msg->id);
	printf("Message contents: %s\n", msg->contents);
}

int main(int argc, char *argv[])
{
        key_t key;
        int shmid;
        message *msg;
        int pid;

        get_key(&key);
	create_shmid(&shmid, key);
        attach(&msg, shmid);
	
	create_message(msg);
        
	shmdt(msg);
	sleep(100);
	shmctl(shmid, IPC_RMID, NULL);
        
	exit(0);
}
