/* ready.c - ready */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>

/*------------------------------------------------------------------------
 * ready  --  make a process eligible for CPU service
 *------------------------------------------------------------------------
 */
int ready(int pid, int resch)
{
	register struct	pentry	*pptr;
	int next;
	next = q[rdyhead].qnext;
	//kprintf("\nCurrent: %s", proctab[pid].pname);
	/*while(next!=rdytail && next<NPROC) {
		kprintf("\n%s\n", proctab[next].pname);
		next = q[next].qnext;	
	}*/
	if (isbadpid(pid))
		return(SYSERR);
	pptr = &proctab[pid];
	pptr->pstate = PRREADY;
	insert(pid,rdyhead,pptr->pinh);
	//insert(pid,rdyhead,pptr->pprio);
	if (resch)
		resched();
	return(OK);
}
