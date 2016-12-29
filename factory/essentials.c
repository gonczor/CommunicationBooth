 #include "essentials.h"

 void get_key(key_t *key)
 {       
         if((*key = ftok("file", 'R')) == -1)
         {
                 int err = errno;
                 perror("ftok");
                 printf("Error number: %d\n", err);
                 exit(1);
         }
 }

 void create_shmid(int *shmid, const key_t key)
 {
         if((*shmid = shmget(key,BUFSIZE, PERM | IPC_CREAT)) == -1)
         {
                 int err = errno;
                 perror("shmget");
                 printf("Error number: %d\n", err);
                 exit(1);
         }
 }

 void get_shmid(int *shmid, const key_t key)
 {
         if((*shmid = shmget(key,BUFSIZE, PERM)) == -1)
         {
                 int err = errno;
                 perror("shmget");
                 printf("Error number: %d\n", err);
                 exit(1);
         }
 }
 
 void attach(message **msg, const int shmid)
 {
         *msg = (message*) shmat(shmid, (void *)0, 0);
         if (msg == (message**)(-1))
         {
                 perror("shmat");
                 exit(1);
         }
 
 }
