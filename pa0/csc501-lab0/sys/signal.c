/* signal.c - signal */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <sem.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 * signal  --  signal a semaphore, releasing one waiting process
 *------------------------------------------------------------------------
 */
SYSCALL signal(int sem)
{
	unsigned long beginTime = ctr1000;
	STATWORD ps;    
	register struct	sentry	*sptr;

	disable(ps);
	if (isbadsem(sem) || (sptr= &semaph[sem])->sstate==SFREE) {
		restore(ps);
		if(traceFlag == 1) {
			sysCallFreq[currpid][16]++;
			sysCallTimeSum[currpid][16] += ctr1000 - beginTime;
		}
		return(SYSERR);
	}
	if ((sptr->semcnt++) < 0)
		ready(getfirst(sptr->sqhead), RESCHYES);
	restore(ps);
	if(traceFlag == 1) {
		sysCallFreq[currpid][16]++;
        	sysCallTimeSum[currpid][16] += ctr1000 - beginTime;
	}
	return(OK);
}
