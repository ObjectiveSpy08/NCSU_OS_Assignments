/* resched.c  -  resched */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>

unsigned long currSP;	/* REAL sp of current process */
extern int ctxsw(int, int, int, int);
/*-----------------------------------------------------------------------
 * resched  --  reschedule processor to highest priority ready process
 *
 * Notes:	Upon entry, currpid gives current process id.
 *		Proctab[currpid].pstate gives correct NEXT state for
 *			current process if other than PRREADY.
 *------------------------------------------------------------------------
 */

void increase_priority() {
	int curr = q[rdyhead].qnext;
	while (curr != rdytail){ // Loop until we reach the null proces. That process will have the lowest priority
		if(curr != NULLPROC) {
			q[curr].qkey++;
		}
		curr = q[curr].qnext;
	}
}

void recalculate_quantums() {
	int i;
	for(i = 1; i < NPROC; i++) {
		if(proctab[i].pstate == PRFREE) continue;
		proctab[i].ticks_left = (proctab[i].ticks_left >> 1) + proctab[i].pprio;
		proctab[i].goodness = proctab[i].pprio + proctab[i].ticks_left;
	}
}

int ll_sched() {
	register struct	pentry	*optr = &proctab[currpid];
	register struct pentry  *nptr;
	int i, start_new_epoch = 1, max_goodness = 0, next_process = NULLPROC;
		
	// Time slice expired
	if(preempt <= 0) {
		optr->ticks_left = 0;
		optr->goodness = 0;
		preempt = 0;
	}
	// Update ticks left for current process	
	optr->ticks_left = preempt;
	int old_priority = optr->goodness - optr->ticks_left;
	// Recalculate current process goodness: Priority + remaining ticks
    optr->goodness = old_priority + optr->ticks_left;
	
	// Re-insert current process into ready queue if it intends to
	if (optr->pstate == PRCURR) {
                optr->pstate = PRREADY;
                insert(currpid,rdyhead,optr->pprio);
        }
	// Find if a new epoch has to start
	for(i = q[rdyhead].qnext; i != rdytail; i = q[i].qnext) {
		if(i != NULLPROC && proctab[i].ticks_left > 0) {
			start_new_epoch = 0;
			break;
		}
	}
	if(start_new_epoch == 1) {
		recalculate_quantums();
	}
	for(i = q[rdyhead].qnext; i != rdytail; i = q[i].qnext) {
		if(i != NULLPROC && max_goodness < proctab[i].goodness) {
			max_goodness = proctab[i].goodness;
			next_process = i;	
		}
	}
	nptr = &proctab[next_process];
	dequeue(next_process);
	currpid = next_process;
	proctab[currpid].pstate = PRCURR;
	preempt = proctab[currpid].ticks_left;
	ctxsw((int)&optr->pesp, (int)optr->pirmask, (int)&nptr->pesp, (int)nptr->pirmask);
	return OK;
}

int resched()
{
	register struct	pentry	*optr;	/* pointer to old process entry */
	register struct	pentry	*nptr;	/* pointer to new process entry */

	/* no switch needed if current process priority higher than next*/
	int curr, next_process = NULLPROC, max_priority = 0, sched_class = getschedclass();
	if(sched_class == AGESCHED) {
		increase_priority();
		curr = q[rdyhead].qnext;
        	while (curr != rdytail){
			if(max_priority < q[curr].qkey) {
				max_priority = q[curr].qkey;
				next_process = curr;
			}
			curr = q[curr].qnext;
		}
		if ( ( (optr= &proctab[currpid])->pstate == PRCURR) &&
		   (max_priority < q[currpid].qkey)) {
			return(OK);
		}
		
		
		/* force context switch */

		if (optr->pstate == PRCURR) {
			optr->pstate = PRREADY;
			insert(currpid, rdyhead, q[currpid].qkey);
		}

		/* remove highest priority process*/
		dequeue(next_process);
		
		nptr = &proctab[ (currpid = next_process) ];
		nptr->pstate = PRCURR;		/* mark it currently running	*/
		#ifdef	RTCLOCK
		preempt = QUANTUM;		/* reset preemption counter	*/
		#endif
		
		ctxsw((int)&optr->pesp, (int)optr->pirmask, (int)&nptr->pesp, (int)nptr->pirmask);
		
		/* The OLD process returns here when resumed. */
		return OK;
	}
	else return ll_sched();
}
