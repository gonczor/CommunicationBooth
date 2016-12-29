#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>

#define BUFSIZE 1024
#define PERM    0644

typedef struct message
{
	int id;
	char contents[BUFSIZE];
	unsigned int len;
} message;

void get_key(key_t *key);
void get_shmid(int *shmid, const key_t key);
void attach(message **msg, const int shmid);


