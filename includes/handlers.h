#ifndef __LAB__
#define __LAB__
#include <signal.h>

void stdHandler(int signum, siginfo_t* info, void* f);
void fullInfoHandler(int signum, siginfo_t* info, void* f);
void accumulateHandler(int signum, siginfo_t* inf, void* f);

void doStd();
void doKill();
void doPipe();
void doPOSIX();
void doChild();
