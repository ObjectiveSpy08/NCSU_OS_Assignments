/* send.c - send */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 *  send  --  send a message to another process
 *------------------------------------------------------------------------
 */
SYSCALL	send(int pid, WORD msg)
{
	unsigned long beginTime = ctr1000;
	STATWORD ps;    
	struct	pentry	*pptr;

	disable(ps);
	if (isbadpid(pid) || ( (pptr= &proctab[pid])->pstate == PRFREE)
	   || pptr->phasmsg != 0) {
		restore(ps);
		if(traceFlag == 1) {
			sysCallFreq[currpid][12]++;
			sysCallTimeSum[currpid][12] += ctr1000 - beginTime;
		}
		return(SYSERR);
	}
	pptr->pmsg = msg;
	pptr->phasmsg = TRUE;
	if (pptr->pstate == PRRECV)	/* if receiver waits, start it	*/
		ready(pid, RESCHYES);
	else if (pptr->pstate == PRTRECV) {
		unsleep(pid);
		ready(pid, RESCHYES);
	}
	restore(ps);
	if(traceFlag == 1) {
		sysCallFreq[currpid][12]++;
        	sysCallTimeSum[currpid][12] += ctr1000 - beginTime;
	}
	return(OK);
}
