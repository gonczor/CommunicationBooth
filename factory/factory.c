 #include <stdio.h>
 #include <stdlib.h>
 #include <sys/ipc.h>
 #include <sys/types.h>
 #include <sys/shm.h>
 #include <errno.h>
 #include <unistd.h>
 
 #include "essentials.h"
 
 #define BUFSIZE 1024
 #define PERM    0644
 int main(int argc, char *argv[])
 {
         key_t key;
         int shmid;
         message *msg;
 
         int pid;
         get_key(&key);
         get_shmid(&shmid, key);
         attach(&msg, shmid);
         printf("msg pointer: %p\n", msg);
         if((pid = fork()) == 0)
         {
                 printf("Process: %d received: %d\n", getpid(), msg->id);
                 shmdt(msg);
         }
         else
         {
                 printf("Process: %d is writing: %d\n", getpid(), 666);
                 msg->id = 666;
                 wait(&pid);
                 shmdt(msg);
                 shmctl(shmid, IPC_RMID, NULL);
         }
         exit(0);
 }
