/* sleep10.c - sleep10 */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <sleep.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 * sleep10  --  delay the caller for a time specified in tenths of seconds
 *------------------------------------------------------------------------
 */
SYSCALL	sleep10(int n)
{
	unsigned long beginTime = ctr1000;
	STATWORD ps;    
	if (n < 0  || clkruns==0) {
		if(traceFlag == 1) {
			sysCallFreq[currpid][19]++;
        		sysCallTimeSum[currpid][19] += ctr1000 - beginTime;
		}
	        return(SYSERR);
	}
	disable(ps);
	if (n == 0) {		/* sleep10(0) -> end time slice */
	        ;
	} else {
		insertd(currpid,clockq,n*100);
		slnempty = TRUE;
		sltop = &q[q[clockq].qnext].qkey;
		proctab[currpid].pstate = PRSLEEP;
	}
	resched();
        restore(ps);
	if(traceFlag == 1) {
		sysCallFreq[currpid][19]++;
        	sysCallTimeSum[currpid][19] += ctr1000 - beginTime;
	}
	return(OK);
}
