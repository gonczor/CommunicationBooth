#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>

#include "booth.h"

#define MAX_CLIENTS 100

typedef struct message_queue{
	pid_t *clients_served;
	message msg;
	struct message_queue *next;
} message_queue;

int stop = 0;
static message *msg = NULL;
message_queue message_vector[MAX_CLIENTS];
unsigned int number_of_clients = 0;
static pid_t *clients_list = NULL;

message_queue* new_queue_element(){
	int i;
	message_queue *new_element = (message_queue*) malloc(sizeof(message_queue));

	memcpy(&new_element->msg, msg, sizeof(message_queue));
	new_element->clients_served = (int*) malloc(number_of_clients*sizeof(number_of_clients));
	new_element->next = NULL;
	for(i = 0; i < number_of_clients; i++){
		new_element->clients_served[i] = 0;
	}

	return new_element;
}

void append_queue(){
	int index;
	printf("Appending with message with id: %d\n", msg->id);
	message_queue *new_element = new_queue_element();
	index = new_element->msg.type;

	if(&message_vector[index] == NULL){
		message_vector[index] = *new_element;
	}
	else {
		message_queue *mq = &message_vector[index];
		while(mq->next != NULL){
			mq = mq->next;
		}
		mq->next = new_element;
	}
}

void show_queue(){
	int i = 0;
	message_queue *mq;
	for(i = 0; i < MAX_CLIENTS; i++){
		printf("message type: %d\n", i);
		mq = &message_vector[i];
		while(mq != NULL){
			printf("ID: %d, contents: %s\n", mq->msg.id, mq->msg.contents);
			mq = mq->next;
		}
	}
}

void read_clients_pids(){
	FILE * fp;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
	int i = 0;

	fp = fopen("clients.txt", "r");
	if (fp == NULL)
		exit(1);

	// get number of clients
	while ((read = getline(&line, &len, fp)) != -1) {
		number_of_clients++;
	}

	fclose(fp);
	clients_list = (int*)malloc(number_of_clients * sizeof(unsigned int));

	fp = fopen("clients.txt", "r");
	if (fp == NULL)
		exit(1);

	// fill clients_list with clients PIDs
	i = 0;
	while ((read = getline(&line, &len, fp)) != -1) {
		clients_list[i++] = (pid_t) atoi(line);
	}

	fclose(fp);
	printf("Dispatcher will work with following clients:\n");
	for(i = 0; i < number_of_clients; i++)
		printf("%d\n", clients_list[i]);

	if (line)
		free(line);
}

void prepare_factory_shm(message **msg, key_t *key, int *shmid){
		get_factory_key(key);
    get_shmid(shmid, *key);
    attach(msg, *shmid);
}

void give_message_to_client(pid_t client){
	printf("Sending message to: %d\n", (int)client);
}

void signal_request_handler(int sig, siginfo_t *siginfo, void *context){
	/*
	SIGUSR1 is used for handling communication with factory
	SIGUSR2 is used for handling communication with clients.
		if number_of_clients equals to 0, we read new clients list.
		otherwise we give newest message still not read by client. If all messages have been read NULL is set
	SIGTERM sets stop variable to 1 break loop in main() and clean up mess.
	*/
	if(sig == SIGUSR1){
		printf("SIGUSR1 received by: %d sent by: %d\n", getpid(), siginfo->si_pid);
		append_queue();
	}
	else if(sig == SIGUSR2){
		printf("SIGUSR2 received by process: %d\n", getpid());
		if(number_of_clients == 0){
			read_clients_pids();
		}
		else{
			give_message_to_client(siginfo->si_pid);
		}
	}
	else if(sig == SIGTERM){
		printf("SIGTERM received by: %d\n", getpid());
		stop = 1;
	}
}

void prepare_sigaction(){
	static struct sigaction factory_action;
	static struct sigaction client_action;
	static struct sigaction terminate_action;

	factory_action.sa_sigaction = *signal_request_handler;
	factory_action.sa_flags |= SA_SIGINFO;
	sigemptyset(&factory_action.sa_mask);
	sigaction(SIGUSR1, &factory_action, NULL);

	client_action.sa_sigaction = *signal_request_handler;
	client_action.sa_flags |= SA_SIGINFO;
	sigemptyset(&client_action.sa_mask);
	sigaction(SIGUSR2, &client_action, NULL);

	terminate_action.sa_sigaction = *signal_request_handler;
	terminate_action.sa_flags |= SA_SIGINFO;
	sigemptyset(&terminate_action.sa_mask);
	sigaction(SIGTERM, &terminate_action, NULL);
}

void free_message_vector(){
	int i;
	message_queue *mq, *mq_next;;
	for(i = 0; i < MAX_CLIENTS; i++){
		printf("Freeing: %d\n", i);
		mq = message_vector[i].next;
		while(mq != NULL){
			printf("Freeing id: %d at position: %p\n", mq->msg.id, mq);
			mq_next = mq->next;
			free(mq->clients_served);
			free(mq);
			mq = mq_next;
		}
	}
}

int main(int argc, char *argv[]){
  key_t key;
	int shmid;

	read_clients_pids();
	prepare_factory_shm(&msg, &key, &shmid);
	prepare_sigaction();

	// wait for termination signal
	while(!stop)
		;

	// clean up
	show_queue();
	free_message_vector();

  shmdt(msg);
	shmctl(shmid, IPC_RMID, NULL);
  exit(0);
}
