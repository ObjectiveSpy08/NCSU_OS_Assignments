/* sleep100.c - sleep100 */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <sleep.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 * sleep100  --  delay the caller for a time specified in 1/100 of seconds
 *------------------------------------------------------------------------
 */
SYSCALL sleep100(int n)
{
	unsigned long beginTime = ctr1000;
	STATWORD ps;    

	if (n < 0  || clkruns==0) {
		if(traceFlag == 1) {
			sysCallFreq[currpid][20]++;
        		sysCallTimeSum[currpid][20] += ctr1000 - beginTime;
		}
	         return(SYSERR);
	}
	disable(ps);
	if (n == 0) {		/* sleep100(0) -> end time slice */
	        ;
	} else {
		insertd(currpid,clockq,n*10);
		slnempty = TRUE;
		sltop = &q[q[clockq].qnext].qkey;
		proctab[currpid].pstate = PRSLEEP;
	}
	resched();
        restore(ps);
	if(traceFlag == 1) {
		sysCallFreq[currpid][20]++;
        	sysCallTimeSum[currpid][20] += ctr1000 - beginTime;
	}
	return(OK);
}