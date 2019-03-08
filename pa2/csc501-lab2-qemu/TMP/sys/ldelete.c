#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <lock.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 *  * ldelete  --  delete a rw lock by releasing its table entry
 *   *------------------------------------------------------------------------
 *    */
SYSCALL ldelete(int lock)
{
	STATWORD ps;    
	struct	lentry	*lptr;

	disable(ps);
	if (isbadLock(lock) || rwlocks[lock].lstate != LUSED) {
		restore(ps);
		return(SYSERR);
	}
	lptr = &rwlocks[lock];
	sdelete(lptr->lr_mutex);
	sdelete(lptr->lrw_mutex);
	lptr->lstate = LFREE;
	nextLock = lock;
	restore(ps);
	return(OK);
}

