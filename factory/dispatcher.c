#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>

#include "booth.h"

typedef struct message_queue{
	pid_t *clients_served;
	message msg;
	struct message_queue *next;
} message_queue;

int stop = 0;
message *msg;
message_queue *msg_queue_head = NULL;
message_queue *msg_queue_tail = NULL;
unsigned int number_of_clients = 0;
pid_t *clients_list;

message_queue* new_queue_element(){
	int i;
	message_queue *new_element = malloc(sizeof(message_queue));

	new_element->clients_served = malloc(number_of_clients*sizeof(number_of_clients));
	new_element->msg = *msg;
	new_element->next = NULL;
	for(i = 0; i < number_of_clients; i++){
		new_element->clients_served[i] = 0;
	}

	return new_element;
}

void append_queue(){
	printf("Appending with message with id: %d\n", msg->id);
	message_queue *new_element = new_queue_element();
	if(msg_queue_head){
		msg_queue_tail->next = new_element;
		msg_queue_tail = new_element;
	}
	else{
		msg_queue_head = msg_queue_tail = new_element;
	}
}

void show_queue(){
	message_queue *element = msg_queue_head;
	while(element != NULL){
		printf("Element has message id: %d\n", element->msg.id);
		element = element->next;
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
	clients_list = (int*) malloc(number_of_clients * sizeof(unsigned int));

	fp = fopen("clients.txt", "r");
	if (fp == NULL)
		exit(1);

	// fill clients_list with clients PIDs
	while ((read = getline(&line, &len, fp)) != -1) {
		clients_list[i] = (pid_t) atoi(line);
		i++;
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
		printf("SUGUSR2 received by process: %d\n", getpid());
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
    shmdt(msg);
	shmctl(shmid, IPC_RMID, NULL);
    exit(0);
}
