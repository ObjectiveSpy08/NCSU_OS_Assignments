/* releaseall.c - releaseall */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include<sem.h>
#include <lock.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 *  * releaseall  --  release all locks
 *   *------------------------------------------------------------------------
 *    */
SYSCALL releaseall(int nargs, int args)
{
	STATWORD ps;    
	register struct	lentry	*lptr;
	struct sentry *srwptr;
	struct pentry *pptr;
	unsigned long *a;
	int lock, flag=0, retval=OK;
	disable(ps);
	
	pptr = &proctab[currpid];
	
	a = (unsigned long *)(&args) + (nargs - 1);
	for(; nargs > 0; nargs--) {
		lock = *a--;
		if (isbadLock(lock) || (lptr= &rwlocks[lock])->lstate != LUSED) {
			restore(ps);
        	        return(SYSERR);
        	}

			srwptr = &semaph[lptr->lrw_mutex];
			//kprintf("\n%d", pptr->lockprio[lock]);
			//kprintf("\n%d", lastkey(srwptr->sqtail));
			//kprintf("\n%d",nonempty(srwptr->sqhead) );
		if(pptr->locks[lock] == 0) {
			retval = SYSERR;
		} else if(pptr->locks[lock] == WRITE) {
			lsignal(lptr->lrw_mutex);
			pptr->locks[lock] = 0;
			pptr->lockprio[lock] = MININT;
			retval = OK;
		} else if(pptr->locks[lock] == READ) {
			if(nonempty(srwptr->sqhead) && pptr->lockprio[lock] > lastkey(srwptr->sqtail)) {
				flag = 1;
				lsignal(lptr->lrw_mutex);
			}
			retval = lwait(lptr->lr_mutex, pptr->lockprio[lock]);
			lptr->lread_cnt--;
			if(flag == 0 && lptr->lread_cnt == 0 ) {
				lsignal(lptr->lrw_mutex);
				lsignal(lptr->lr_mutex);
			} else {
				lsignal(lptr->lr_mutex);
			}
			pptr->locks[lock] = 0;
			pptr->lockprio[lock] = MININT;
		}
	}
	restore(ps);
	return retval;
}

