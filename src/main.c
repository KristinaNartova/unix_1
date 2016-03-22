#include <stdio.h>  
#include <stdlib.h> 
#include <string.h> 
#include <sys/signal.h>
#include <unistd.h>
#include <ctype.h>
#include <getopt.h>
#include <errno.h>
#include <handlers.h>

/*char arr[100][100];
int i = 0;

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
    sprintf(arr[i], "N: %d  PID: %d  SIGNO: %d  NUMBER: %d ", i+1, getpid(), info->si_signo, info->si_value.sival_int);
    i++;
}

void printLogHandler(int signum, siginfo_t* info, void* f) {
    printf("PARENT:\n");
    for (int j=0;j<i;j++) {
        printf("%s\n", arr[j]);
    }
} */

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
