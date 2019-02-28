/* sleep.c - sleep */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <sleep.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 * sleep  --  delay the calling process n seconds
 *------------------------------------------------------------------------
 */
SYSCALL	sleep(int n)
{
	unsigned long beginTime = ctr1000;
	STATWORD ps;    
	if (n<0 || clkruns==0) {
		if(traceFlag == 1) {
			sysCallFreq[currpid][18]++;
	        	sysCallTimeSum[currpid][18] += ctr1000 - beginTime;
		}
		return(SYSERR);
	}
	if (n == 0) {
	        disable(ps);
		resched();
		restore(ps);
		if(traceFlag == 1) {
			sysCallFreq[currpid][18]++;
        		sysCallTimeSum[currpid][18] += ctr1000 - beginTime;
		}
		return(OK);
	}
	while (n >= 1000) {
		sleep10(10000);
		n -= 1000;
	}
	if (n > 0)
		sleep10(10*n);
	if(traceFlag == 1) {
		sysCallFreq[currpid][18]++;
        	sysCallTimeSum[currpid][18] += ctr1000 - beginTime;
	}
	return(OK);
}
