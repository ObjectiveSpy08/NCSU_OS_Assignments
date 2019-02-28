/* sdelete.c - sdelete */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <sem.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 * sdelete  --  delete a semaphore by releasing its table entry
 *------------------------------------------------------------------------
 */
SYSCALL sdelete(int sem)
{
	unsigned long beginTime = ctr1000;
	STATWORD ps;    
	int	pid;
	struct	sentry	*sptr;

	disable(ps);
	if (isbadsem(sem) || semaph[sem].sstate==SFREE) {
		restore(ps);
		if(traceFlag == 1) {
			sysCallFreq[currpid][11]++;
			sysCallTimeSum[currpid][11] += ctr1000 - beginTime;
		}
		return(SYSERR);
	}
	sptr = &semaph[sem];
	sptr->sstate = SFREE;
	if (nonempty(sptr->sqhead)) {
		while( (pid=getfirst(sptr->sqhead)) != EMPTY)
		  {
		    proctab[pid].pwaitret = DELETED;
		    ready(pid,RESCHNO);
		  }
		resched();
	}
	restore(ps);
	if(traceFlag == 1) {
		sysCallFreq[currpid][11]++;
        	sysCallTimeSum[currpid][11] += ctr1000 - beginTime;
	}
	return(OK);
}
