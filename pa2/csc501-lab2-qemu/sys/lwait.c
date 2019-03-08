/* lwait.c - lwait */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <sem.h>
#include <stdio.h>

LOCAL void cascade_priority(int proc, int newpinh);

/*------------------------------------------------------------------------
 *  * lwait  --  make current process wait on a semaphore
 *   *------------------------------------------------------------------------
 *    */
SYSCALL	lwait(int sem, int priority, int lock)
{
	STATWORD ps;    
	struct	sentry	*sptr;
	struct	pentry	*pptr;

	disable(ps);
	if (isbadsem(sem) || (sptr= &semaph[sem])->sstate==SFREE) {
		restore(ps);
		return(SYSERR);
	}
	
	if (--(sptr->semcnt) < 0) {
		if(proctab[currpid].pinh > proctab[sptr->owner].pinh) {
			cascade_priority(sptr->owner, proctab[currpid].pinh);
		}
		(pptr = &proctab[currpid])->pstate = PRWAIT;
		pptr->psem = sem;
		pptr->waitfor = sptr->owner;
		linsert(currpid, sptr->sqhead, sptr->sqtail, priority, lock);
		pptr->pwaitret = OK;
		resched();
		restore(ps);
		return pptr->pwaitret;
	} else {
		sptr->owner = currpid;
		proctab[currpid].waitfor = -1;
	}
	restore(ps);
	return(OK);
}

LOCAL void cascade_priority(int proc, int newpinh)
{
	chprio(proc, newpinh);
	if(proctab[proc].waitfor != -1){
		cascade_priority(proctab[proc].waitfor, newpinh);
	}
}
