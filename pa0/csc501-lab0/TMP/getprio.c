/* getprio.c - getprio */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 * getprio -- return the scheduling priority of a given process
 *------------------------------------------------------------------------
 */
SYSCALL getprio(int pid)
{
	unsigned long beginTime = ctr1000;
	STATWORD ps;    
	struct	pentry	*pptr;

	disable(ps);
	if (isbadpid(pid) || (pptr = &proctab[pid])->pstate == PRFREE) {
		restore(ps);
		if(traceFlag == 1) {
			sysCallFreq[currpid][3]++;
	        	sysCallTimeSum[currpid][3] += ctr1000 - beginTime;
		}
		return(SYSERR);
	}
	restore(ps);
	if(traceFlag == 1) {
		sysCallFreq[currpid][3]++;
        	sysCallTimeSum[currpid][3] += ctr1000 - beginTime;
	}
	return(pptr->pprio);
}
