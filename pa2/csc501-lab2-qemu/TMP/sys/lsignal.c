/* lsignal.c - lsignal */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <sem.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 *  * lsignal  --  lsignal a semaphore, releasing one waiting process
 *   *------------------------------------------------------------------------
 *    */
SYSCALL lsignal(int sem)
{
	STATWORD ps;    
	register struct	sentry	*sptr;
	int newproc;
	disable(ps);
	if (isbadsem(sem) || (sptr= &semaph[sem])->sstate==SFREE) {
		restore(ps);
		return(SYSERR);
	}
	chprio(currpid, proctab[currpid].pprio);
	if ((sptr->semcnt++) < 0){
		newproc = getlast(sptr->sqtail);
		proctab[newproc].waitfor = -1;
		sptr->owner = newproc;		
		ready(newproc, RESCHYES);
	}
	
	restore(ps);
	return(OK);
}

