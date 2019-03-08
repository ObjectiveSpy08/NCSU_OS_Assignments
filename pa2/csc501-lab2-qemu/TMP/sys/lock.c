/* lock.c - lock */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <lock.h>
#include <sem.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 *  * lock  --  make current process wait on a semaphore
 *   *------------------------------------------------------------------------
 *    */
SYSCALL	lock(int lock, int type, int priority)
{
	STATWORD ps;
	int seq, retval;   
	struct	lentry	*lptr;
	struct	pentry	*pptr;
	struct  sentry  *sptr;
	disable(ps);

	pptr = &proctab[currpid];
	if (isbadLock(lock) || (lptr= &rwlocks[lock])->lstate != LUSED || pptr->pwaitret == DELETED) {
		pptr->pwaitret = OK;
		restore(ps);
		return SYSERR;
	}
	pptr->lockprio[lock] = priority;
	pptr->locks[lock] = type;
	if(type == READ) {
		seq = lptr->lseq;	
		if((retval = lwait(lptr->lr_mutex, priority) == SYSERR) || lptr->lstate != LUSED || lptr->lseq != seq) {
			restore(ps);
			return SYSERR;
		}
		if(retval == DELETED) {
			restore(ps);
			return retval;
		}
		lptr->lread_cnt++;
		sptr = &semaph[lptr->lrw_mutex];
		if(lptr->lread_cnt == 1) {
			lwait(lptr->lrw_mutex, priority, lock);
			lsignal(lptr->lr_mutex);
		} else if(nonempty(sptr->sqhead) && priority <= lastkey(sptr->sqtail)) {
			lwait(lptr->lrw_mutex, priority, lock);
			lsignal(lptr->lr_mutex);
			//lwait(lptr->lrw_mutex, priority);
		} else {
			lsignal(lptr->lr_mutex);
		}	
	} else if(type == WRITE) {
		seq = lptr->lseq;
		if((retval = lwait(lptr->lrw_mutex, priority) == SYSERR) || lptr->lstate != LUSED || lptr->lseq != seq) {
			restore(ps);
			return SYSERR;
		}
		if(retval == DELETED) {
			restore(ps);
			return retval;
		}
	}

	restore(ps);
	return retval;
}

