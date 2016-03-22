#include <stdio.h>  
#include <stdlib.h>  
#include <sys/signal.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>

void stdHandler(int signum, siginfo_t* info, void* f) {
    printf("%d | %d\n", info->si_signo, info->si_pid);
}

void doStd() {
    struct sigaction sigusrone;
    struct sigaction sigusrtwo;
    struct sigaction sighup;
    
    sigusrone.sa_sigaction = stdHandler;
    sigusrtwo.sa_sigaction = stdHandler;
    sighup.sa_sigaction = stdHandler;
    
    
    sigusrone.sa_flags = SA_SIGINFO;
    sigusrtwo.sa_flags = SA_SIGINFO;
    sighup.sa_flags = SA_SIGINFO;
    
    if (sigaction(SIGUSR1, &sigusrone, NULL) == -1) {
        perror("sigaction");
        exit(1);
    };

    
    if (sigaction(SIGUSR2, &sigusrtwo, NULL) == -1) {
        perror("sigaction");
        exit(1);
    };
        
    if (sigaction(SIGHUP, &sighup, NULL)) {
        perror("sigaction");
        exit(1);
    };
    
        
    while (1) {
     /*   sleep(5);
        raise(SIGHUP);
        raise(SIGUSR1);
        raise(SIGUSR2); */ 
    } 
} 