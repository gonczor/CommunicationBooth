#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

#include "booth.h"

int main(int argc, char* argv[]){
	if(argc != 3){
		printf("Usage: ./client.exe <message_type> <dispatcher_pid>\n" );
		exit(1);
	}

	printf("client.exe pid: %d\n", getpid());
	const char *cs = get_client_com_file_name(getpid());
	printf("Client com file: %s\n", cs);
	printf("OK\n");
	message_request(atoi(argv[1]));

	return 0;
}
