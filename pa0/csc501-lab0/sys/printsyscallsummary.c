#include<conf.h>
#include<stdio.h>
#include<kernel.h>
#include<proc.h>

 int sysCallFreq[NPROC][27];
 int sysCallTimeSum[NPROC][27];
int traceFlag = 0;
void syscallsummary_start() {
	int processCounter = 0, sysCallCounter = 0;
	for(processCounter = 0; processCounter < NPROC; processCounter++) {
		for(sysCallCounter = 0; sysCallCounter < 27; sysCallCounter++) {
			sysCallFreq[processCounter][sysCallCounter] = 0;
			sysCallTimeSum[processCounter][sysCallCounter] = 0;
		}
	}

	traceFlag = 1;
}

void syscallsummary_stop() {
	traceFlag = 0;
}

void printsyscallsummary() {
	int processCounter = 0, sysCallCounter = 0, currentFreq, currentSum, sysCallsMade;
int callsInProc[NPROC];
	char* sysCalls[] = {"freemem", "chprio", "getpid", "getprio", "gettime", "kill", "receive", "recvclr", "recvtim", "resume", "scount", "sdelete", "send", "setdev", "setnok", "screate", "signal", "signaln", "sleep", "sleep10", "sleep100", "sleep1000", "sreset", "stacktrace", "suspend", "unsleep", "wait"};
	kprintf("\n\nvoid printsyscallsummary()");
	for(processCounter=0; processCounter < NPROC; processCounter++) {
		sysCallsMade = 0;
		for(sysCallCounter=0; sysCallCounter < 27; sysCallCounter++) {
                	if(sysCallFreq[processCounter][sysCallCounter] > 0) {
		    		sysCallsMade = 1;
			    	break;
			}
       		 }	
		callsInProc[processCounter] = sysCallsMade;
    	}
	for(processCounter = 0; processCounter < NPROC; processCounter++) {
		if(callsInProc[processCounter] == 0) continue;
		kprintf("\nProcess [pid:%d]", processCounter);
		for(sysCallCounter = 0; sysCallCounter < 27; sysCallCounter++) {
			currentFreq = sysCallFreq[processCounter][sysCallCounter];
			currentSum = sysCallTimeSum[processCounter][sysCallCounter];
                        if(currentFreq == 0) continue;
			kprintf("\n\tSyscall: %s, count: %d, average execution time: %d (ms)", sysCalls[sysCallCounter], currentFreq, currentSum/currentFreq);
                }
        }
}
