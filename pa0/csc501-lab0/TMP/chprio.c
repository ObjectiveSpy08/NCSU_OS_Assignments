/* chprio.c - chprio */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 * chprio  --  change the scheduling priority of a process
 *------------------------------------------------------------------------
 */
SYSCALL chprio(int pid, int newprio)
{
	unsigned long beginTime = ctr1000;
	STATWORD ps;    
	struct	pentry	*pptr;

	disable(ps);
	if (isbadpid(pid) || newprio<=0 ||
	    (pptr = &proctab[pid])->pstate == PRFREE) {
		restore(ps);
		if(traceFlag == 1) {
			sysCallFreq[currpid][1]++;
	        	sysCallTimeSum[currpid][1] += ctr1000 - beginTime;
		}
		return(SYSERR);
	}
	pptr->pprio = newprio;
	restore(ps);
	if(traceFlag == 1) {
		sysCallFreq[currpid][1]++;
        	sysCallTimeSum[currpid][1] += ctr1000 - beginTime;
	}
	return(newprio);
}
