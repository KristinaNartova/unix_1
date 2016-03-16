#include <stdio.h>  
#include <stdlib.h> 
#include <string.h> 
#include <sys/signal.h>
#include <unistd.h>
#include <ctype.h>
#include <getopt.h>
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
        sleep(5);
        raise(SIGHUP);
        raise(SIGUSR1);
        raise(SIGUSR2);
    } 
} 

void doKill() {
    
}

void doPOSIX() {
    
}

void doChild() {
    
}

void doPipe() {
    
}



int main(int argc, char** argv) {
    int c = 0;
    
    char* mode;
    char* sig;
    char* proc;
    char* n;
    
    int option_index = 0;
        
    static struct option long_options[] = {
         {"mode", required_argument, 0,  0 },
         {"signal", required_argument, 0, 0},
         {"pid", required_argument, 0, 0},
         {"amount", required_argument, 0, 0}
    };
    
    while ((c = getopt_long(argc, argv, "m",long_options, &option_index)) != -1) {
        if (strcmp(long_options[option_index].name, "mode") == 0 ) {
            mode = optarg;
        } else if (strcmp(long_options[option_index].name, "signal") == 0) {
            sig = optarg;
        } else if (strcmp(long_options[option_index].name, "pid") == 0) {
            proc = optarg;
        } else if (strcmp(long_options[option_index].name, "amount") == 0 ) {
            n = optarg;
        }
    }
    
    if (strcmp(mode,"std") == 0) {
        doStd();
    } else if (strcmp(mode,"kill") == 0) {
        doKill();
    } else if (strcmp(mode,"posix") == 0) {
        doPOSIX();
    } else if (strcmp(mode,"child") == 0) {
        doChild();
    } else {
        doPipe();
    }
    
    return 0;
}