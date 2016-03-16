#include <stdio.h>  
#include <stdlib.h> 
#include <string.h> 
#include <sys/signal.h>
#include <unistd.h>
#include <ctype.h>
#include <getopt.h>
#include <errno.h>

char log[100][100];
int logIndex = 0;

void stdHandler(int signum, siginfo_t* info, void* f) {
    printf("%d | %d\n", info->si_signo, info->si_pid);
}

void fullInfoHandler(int signum, siginfo_t* info, void* f) {
    printf("PID: %d\n", info->si_pid);
    printf("Signo: %d\n", info->si_value);
    printf("Errno: %d\n", info->si_errno);
    printf("UID: %d\n", info->si_uid);
    printf("SIADDR: %d\n", info->si_addr);
    printf("Status: %d\n", info->si_status);
    printf("SIBAND: %d\n", info->si_band);
}


void saveHandler(int signum, siginfo_t* info,void* f) {
    sprintf(log[logIndex], "N: %d  PID: %d  SIGNO: %d  NUMBER: %d ", logIndex+1, getpid(), info->si_signo, info->si_value.sival_int);
    logIndex++;
}

void printLogHandler(int signum, siginfo_t* info, void* f) {
    printf("PARENT:\n");
    for (int i=0;i<logIndex;i++) {
        printf("%s\n", log[i]);
    }
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

void doKill(char* proc, char* sig) {
    if (kill(atoi(proc), atoi(sig)) == -1) {
        perror("kill");
        exit(1);
    };
}

void doPOSIX(char* n) {
    time_t t;
    srand((unsigned) time(&t));
    
    struct sigaction sigusrone;
    struct sigaction sigusrtwo;
    struct sigaction sighup;
    struct sigaction sigchld;
    
    sigusrone.sa_sigaction = saveHandler;
    sigusrtwo.sa_sigaction = saveHandler;
    sighup.sa_sigaction = saveHandler;
    sigchld.sa_sigaction = printLogHandler;
    
    
    sigusrone.sa_flags = SA_SIGINFO;
    sigusrtwo.sa_flags = SA_SIGINFO;
    sighup.sa_flags = SA_SIGINFO;
    sigchld.sa_flags = SA_SIGINFO;
    
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
       
    if (sigaction(SIGCHLD, &sigchld, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }
    
    pid_t child = fork();
    if (child == 0) {
        int amountInt = atoi(n);
        union sigval val;
            
        printf("CHILD:\n");
        for(int i=0;i<amountInt;i++) {
            int sigN = rand()%3;
            int next;
            switch(sigN) {
                case 0:
                    next = SIGUSR1;
                    break;
                case 1:
                    next = SIGUSR2;
                    break;
                case 2:
                    next = SIGHUP;
                    break;
                }
                
            val.sival_int = rand()%100;
            if (sigqueue(getppid(),next,val) == -1) {
                perror("sigqueue");
                exit(1);
            };
            printf("N: %d PID: %d PPID: %d SIGNO: %d NUMBER: %d \n", i+1, getpid(), getppid(), next, val.sival_int);
            sleep(5);
        }
        exit(13);
    } else if (child > 0) {
        while (1) {
            
        }
    } else {
        perror("fork");
        exit(1);
    }
}

void doChild() {
    struct sigaction sigchld;
    sigchld.sa_flags = SA_SIGINFO;
    sigchld.sa_sigaction = fullInfoHandler;
    if (sigaction(SIGCHLD, &sigchld, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }
    
    pid_t child = fork();
    if (child == 0) {
        sleep(5);
        exit(0);
    } else if (child > 0) {
        while (1) {
            
        }
    } else {
        perror("fork");
        exit(1);
    }
}


void doPipe() {
    struct sigaction sigpipe;
    sigpipe.sa_flags = SA_SIGINFO;
    sigpipe.sa_sigaction = stdHandler;
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
        doKill(proc,sig);
    } else if (strcmp(mode,"posix") == 0) {
        doPOSIX(n);
    } else if (strcmp(mode,"child") == 0) {
        doChild();
    } else {
        doPipe();
    }
    
    return 0;
}