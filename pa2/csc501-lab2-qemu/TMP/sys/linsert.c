/* insert.c  -  insert */

#include <conf.h>
#include <kernel.h>
#include <q.h>
#include <stdio.h>
#include <proc.h>
#include <lock.h>
/*------------------------------------------------------------------------
 *  * insert.c  --  insert an process into a q list in key order
 *   *------------------------------------------------------------------------
 *    */
extern unsigned long ctr1000;
int linsert(int proc, int head, int tail, int key, int lock)
{
	int	next;			/* runs through list		*/
	int	prev;
	unsigned long currtime;
	next = q[head].qnext;
	while (q[next].qkey < key){
	/* tail has maxint as key	*/
		next = q[next].qnext;
	}
	currtime = ctr1000;	
	if(next != tail && 
	q[next].qkey == key &&
	proctab[proc].locks[lock] == WRITE &&
	proctab[next].locks[lock] == READ &&
	currtime - proctab[next].linsertTime < 600) {
			next = q[next].qnext;
	}
	q[proc].qnext = next;
	q[proc].qprev = prev = q[next].qprev;
	q[proc].qkey  = key;
	q[prev].qnext = proc;
	q[next].qprev = proc;
	proctab[proc].linsertTime = currtime;
	return(OK);
}
