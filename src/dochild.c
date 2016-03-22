#include <stdio.h>  
#include <stdlib.h> 
#include <sys/signal.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>

void fullInfoHandler(int signum, siginfo_t* info, void* f) {
    printf("PID: %d\n", info->si_pid);
    printf("Signo: %d\n", info->si_value);
    printf("Errno: %d\n", info->si_errno);
    printf("UID: %d\n", info->si_uid);
    printf("SIADDR: %d\n", info->si_addr);
    printf("Status: %d\n", info->si_status);
    printf("SIBAND: %d\n", info->si_band);
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
        int status;
        if (0<wait(&status)) {
		      int exit_way = WIFEXITED(status);
		      int exit_code = WEXITSTATUS(status);
		      fprintf(stderr, "Exit Way: %d, Exit Code: %d\n", exit_way, exit_code);
		      exit(0);
	    } else {
		      perror("Failed handling child");
		      exit(1);
	    }
    } else {
        perror("fork");
        exit(1);
    }
}