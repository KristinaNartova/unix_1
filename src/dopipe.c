#include <stdio.h>  
#include <stdlib.h> 
#include <string.h> 
#include <sys/signal.h>
#include <unistd.h>
#include <errno.h>

void handlerStd(int signum, siginfo_t* info, void* f) {
    printf("%d | %d\n", info->si_signo, info->si_pid);
}

void doPipe() {
    struct sigaction sigpipe;
    sigpipe.sa_flags = SA_SIGINFO;
    sigpipe.sa_sigaction = handlerStd;
    if (sigaction(SIGPIPE, &sigpipe, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }
    
    pid_t child = fork();
    char test[] = "test";
    int fd[2];
    
    if (pipe(fd) < 0) {
        perror("Pipe");
        exit(1);
    };
    
    if (child == 0) {
        close(fd[1]);
        close(fd[0]);
        exit(0);
    } else if (child > 0) {
        close(fd[0]);
        sleep(7);
        if (write(fd[1], test, (strlen(test)+1)) == -1) {
            perror("Writing failed");
            exit(1);
        };
    } else {
        perror("fork");
        exit(1);
    }
}