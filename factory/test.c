// send signal to self test - raise()

#include <stdio.h>
#include <signal.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

static int int_count = 0, max_int = 5;
static struct sigaction siga;

static void multi_handler(int sig, siginfo_t *siginfo, void *context) {
    // get pid of sender,
    pid_t sender_pid = siginfo->si_pid;

    if(sig == SIGINT) {
        int_count++;
        printf("INT(%d), from [%d]\n", int_count, (int)sender_pid);
        return;
    } else if(sig == SIGQUIT) {
        printf("Quit, bye, from [%d]\n", (int)sender_pid);
        exit(0);
    }

    return;
}

int raise_test() {
    // print pid
    printf("process [%d] started.\n", (int)getpid());

    // prepare sigaction
    siga.sa_sigaction = *multi_handler;
    siga.sa_flags |= SA_SIGINFO; // get detail info

    // change signal action,
    if(sigaction(SIGINT, &siga, NULL) != 0) {
        printf("error sigaction()");
        return errno;
    }
    if(sigaction(SIGQUIT, &siga, NULL) != 0) {
        printf("error sigaction()");
        return errno;
    }

    // use "ctrl + c" to send SIGINT, and "ctrl + \" to send SIGQUIT,
    int sig;
    while(1) {
        if(int_count < max_int) {
            sig = SIGINT;
        } else {
            sig  = SIGQUIT;
        }
        raise(sig); // send signal to itself,

        sleep(1); // sleep a while, note that: SIGINT will interrupt this, and make program wake up,
    }

    return 0;
}

int main(int argc, char *argv[]) {
    raise_test();
    return 0;
}
