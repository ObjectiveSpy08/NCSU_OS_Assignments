/* lcreate.c - lcreate, newLock */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <lock.h>
#include <stdio.h>

LOCAL int newLock();

/*------------------------------------------------------------------------
 *  * lcreate  --  create and initialize a reader write lock, returning its id
 *   *------------------------------------------------------------------------
 *    */
SYSCALL lcreate()
{
	STATWORD ps;    
	int	lock;

	disable(ps);
	lock = newLock();
	restore(ps);
	return lock;
}

/*------------------------------------------------------------------------
 *  * newLock  --  allocate an unused lock and return its index
 *   *------------------------------------------------------------------------
 *    */
LOCAL int newLock()
{
	int	lock;
	int	i;
	struct  lentry *lptr;

	for (i = 0 ; i < NLOCKS ; i++) {
		lock = nextLock;
		if (++nextLock >= NLOCKS)
			nextLock = 0;
		lptr = &rwlocks[lock];
		if (lptr->lstate == LFREE) {
			lptr->lstate = LUSED;
			lptr->lread_cnt = 0;
			lptr->lrw_mutex = screate(1);
			lptr->lr_mutex = screate(1);
			lptr->lseq++;
			return(lock);
		}
	}
	return(SYSERR);
}

