#include "booth.h"

static char pid_char[128];
char * get_client_com_file_name(pid_t pid){
  memset(pid_char, 0, 128);
  sprintf(pid_char,"%d", pid);
  printf("pid_char: %s\n", pid_char);
  strcat(pid_char, "_com");
  return pid_char;
}

void inform_dispatcher(int dispatcher_id){
	printf("Informing: %d\n", dispatcher_id);
	kill(dispatcher_id, SIGUSR1);
}

static void get_key(key_t *key, char* file_name){
    if((*key = ftok(file_name, 'R')) == -1){
        int err = errno;
        perror("ftok");
        printf("Error number: %d\a\n", err);
        exit(1);
    }
}

void get_factory_key(key_t *key){
    get_key(key, "factory_com");
}

void get_client_key(key_t *key, pid_t client_pid){
  char buf[128];
  sprintf(buf, "%s", get_client_com_file_name(client_pid));
  get_key(key, buf);
}

void create_shmid(int *shmid, const key_t key){
  if((*shmid = shmget(key,BUFSIZE, PERM | IPC_CREAT)) == -1){
    int err = errno;
    perror("shmget");
    printf("Error number: %d\n", err);
    exit(1);
  }
}

void get_shmid(int *shmid, const key_t key){
    if((*shmid = shmget(key,BUFSIZE, PERM)) == -1){
        int err = errno;
        perror("shmget");
        printf("Error number: %d\n", err);
        exit(1);
    }
}

void attach(message **msg, const int shmid){
    *msg = (message*) shmat(shmid, (void *)0, 0);
    if (msg == (message**)(-1)){
        perror("shmat");
        exit(1);
    }
}

message message_request(pid_t pid){
    kill(SIGUSR2, pid);
    printf("Sent SIGUSR2 to: %d\n", pid);
    message m;
    return m;
}
