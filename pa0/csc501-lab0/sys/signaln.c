/* signaln.c - signaln */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <sem.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 *  signaln -- signal a semaphore n times
 *------------------------------------------------------------------------
 */
SYSCALL signaln(int sem, int count)
{
	unsigned long beginTime = ctr1000;
	STATWORD ps;    
	struct	sentry	*sptr;

	disable(ps);
	if (isbadsem(sem) || semaph[sem].sstate==SFREE || count<=0) {
		restore(ps);
		if(traceFlag == 1) {
			sysCallFreq[currpid][17]++;
			sysCallTimeSum[currpid][17] += ctr1000 - beginTime;
		}
		return(SYSERR);
	}
	sptr = &semaph[sem];
	for (; count > 0  ; count--)
		if ((sptr->semcnt++) < 0)
			ready(getfirst(sptr->sqhead), RESCHNO);
	resched();
	restore(ps);
	if(traceFlag == 1) {
		sysCallFreq[currpid][17]++;
        	sysCallTimeSum[currpid][17] += ctr1000 - beginTime;
	}
	return(OK);
}
