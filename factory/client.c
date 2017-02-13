#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "booth.h"

int main(int argc, char* argv[]){
	if(argc != 3){
		printf("Usage: ./client.exe <message_type> <dispatcher_pid>\n" );
		exit(1);
	}
	printf("%d going to sleep\n", getpid());
	sleep(1);
	printf("Woke up pid: %d\n", getpid());
	const char *cs = get_client_com_file_name(getpid());
	printf("Client com file: %s\n", cs);
	printf("Requesting message from: %d\n", atoi(argv[2]));
	message_request(atoi(argv[2]));

	return 0;
}
