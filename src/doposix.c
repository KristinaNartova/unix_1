#include <stdio.h>  
#include <stdlib.h> 
#include <sys/signal.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <handlers.h>

char arr[100][100];
int lastElemIndex = 0;


void sigchldhandlerPOSIX(int signum, siginfo_t* info, void* f) {
    printf("Parent prints:\n");
    for (int i = 0;i<lastElemIndex;i++) {
        printf("%s\n", arr[i]);
    } 
}

void sigusronehandlerPOSIX(int signum, siginfo_t* info, void* f) {
    sprintf(arr[lastElemIndex], "%d | %d | %d | %d ", lastElemIndex+1, getpid(), info->si_signo, info->si_value.sival_int);
    lastElemIndex++;
}

void sigusrtwohandlerPOSIX(int signum, siginfo_t* info, void* f) {
    sprintf(arr[lastElemIndex], "%d | %d | %d | %d ", lastElemIndex+1, getpid(), info->si_signo, info->si_value.sival_int);
    lastElemIndex++;
}

void sighuphandlerPOSIX(int signum, siginfo_t* info, void* f) {
    sprintf(arr[lastElemIndex], "%d | %d | %d | %d ", lastElemIndex+1, getpid(), info->si_signo, info->si_value.sival_int);
    lastElemIndex++;
}

/*void printLog(int signum, siginfo_t* info, void* f) {
    printf("Parent prints:\n");
    for (int i = 0;i<lastElemIndex;i++) {
        printf("%s\n", arr[i]);
    } 
}

void pushToLog(int signum, siginfo_t* info, void* f) {
    sprintf(arr[lastElemIndex], "%d | %d | %d | %d ", lastElemIndex+1, getpid(), info->si_signo, info->si_value.sival_int);
    lastElemIndex++;
}*/



void doPOSIX(char* n) {
	time_t t;
	srand((unsigned)time(&t));

	pid_t chld_pid = fork();
  if (0 == chld_pid) {
    int amountInt = atoi(n);
    union sigval val;
            
    printf("Child prints:\n");
    for(int i=0;i<amountInt;i++) {
      int nextSignal = rand()%3+1;
      switch(nextSignal) {
        case 0:
          nextSignal = SIGUSR1;
          break;
        case 1:
          nextSignal = SIGUSR2;
          break;
        case 2:
          nextSignal = SIGHUP;
          break;                        
      }
                
      val.sival_int = rand()%100;
      sigqueue(getppid(),nextSignal,val);
      printf("%d | %d | %d | %d | %d\n", i+1, getpid(), getppid(), nextSignal, val.sival_int);
    }
      exit(13);
   } else if (chld_pid > 0) {
      struct sigaction actSIGCHLD;
      struct sigaction otherActions;
           
      actSIGCHLD.sa_sigaction = sigchldhandlerPOSIX;
      actSIGCHLD.sa_flags = SA_SIGINFO;
      if (sigaction(SIGCHLD, &actSIGCHLD, NULL) == -1) {
        perror("Signal handler assignment failed");
        exit(1);
      };
		
            
      struct sigaction actSIGUSRONE;
      struct sigaction actSIGUSRTWO;
      struct sigaction actSIGHUP; 
    
      actSIGUSRONE.sa_sigaction = sigusronehandlerPOSIX;
      actSIGUSRONE.sa_flags = SA_SIGINFO;
      if (sigaction(SIGUSR1, &actSIGUSRONE, NULL) == -1) {
        perror("Signal handler assignment failed");
         exit(1);
      };
    
      actSIGUSRTWO.sa_sigaction = sigusrtwohandlerPOSIX;
      actSIGUSRTWO.sa_flags = SA_SIGINFO;
      if (sigaction(SIGUSR2, &actSIGUSRTWO, NULL) == -1) {
        perror("Signal handler assignment failed");
        exit(1);
      };
        
      actSIGHUP.sa_sigaction = sighuphandlerPOSIX;
      actSIGHUP.sa_flags = SA_SIGINFO;
      if (sigaction(SIGHUP, &actSIGHUP, NULL) == -1) {
        perror("Signal handler assignment failed");
        exit(1);
      }
           
	    sigfillset(&actSIGHUP.sa_mask);
           
      while(1) {
      }
    }
}
