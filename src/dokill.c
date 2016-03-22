#include <stdio.h>  
#include <stdlib.h> 
#include <sys/signal.h>
#include <unistd.h>
#include <errno.h>

void doKill(char* proc, char* sig) {
    if (kill(atoi(proc), atoi(sig)) == -1) {
        perror("kill");
        exit(1);
    };
}
