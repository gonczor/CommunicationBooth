#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <signal.h>
#include<string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include <fcntl.h>

#define BUFSIZE 1024
#define PERM    0644

typedef struct message{
	int id;
	int type;
	char contents[BUFSIZE];
	unsigned long len;
} message;

char *get_client_com_file_name(pid_t pid);
void prepare_client_com(pid_t pid);
void prepare_factory_com();
void inform_dispatcher(int dispatcher_id);
message message_request();
void get_client_key(key_t *key, pid_t client_pid);
void get_factory_key(key_t *key);
void get_shmid(int *shmid, const key_t key);
void create_shmid(int *shmid, const key_t key);
void attach(message **msg, const int shmid);
