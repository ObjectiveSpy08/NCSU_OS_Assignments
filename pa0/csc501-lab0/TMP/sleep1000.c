/* sleep1000.c - sleep1000 */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <sleep.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 * sleep1000 --  delay the caller for a time specified in 1/100 of seconds
 *------------------------------------------------------------------------
 */
SYSCALL sleep1000(int n)
{
	unsigned long beginTime = ctr1000;
	STATWORD ps;    

	if (n < 0  || clkruns==0) {
		if(traceFlag == 1) {
			sysCallFreq[currpid][21]++;
	        	sysCallTimeSum[currpid][21] += ctr1000 - beginTime;
		}
	        return(SYSERR);
	}
	disable(ps);
	if (n == 0) {		/* sleep1000(0) -> end time slice */
	        ;
	} else {
		insertd(currpid,clockq,n);
		slnempty = TRUE;
		sltop = &q[q[clockq].qnext].qkey;
		proctab[currpid].pstate = PRSLEEP;
	}
	resched();
        restore(ps);
	if(traceFlag == 1) {
		sysCallFreq[currpid][21]++;
        	sysCallTimeSum[currpid][21] += ctr1000 - beginTime;
	}
	return(OK);
}
