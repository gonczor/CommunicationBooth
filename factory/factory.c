#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>

#include "booth.h"

int id = 0;

void create_message(message* msg){
	msg->type = 1;
	msg->id = id++;
	strncpy(msg->contents, "Hello, world!", 1024);
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

	printf("Factory\n");
	get_factory_key(&key);
	get_shmid(&shmid, key);
	attach(&msg, shmid);

	create_message(msg);
	inform_dispatcher(atoi(argv[1]));
	sleep(1);
	create_message(msg);
	inform_dispatcher(atoi(argv[1]));
	shmdt(msg);

	exit(0);
}
