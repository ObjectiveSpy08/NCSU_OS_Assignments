/* setnok.c - setnok */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 *  setnok  -  set next-of-kin (notified at death) for a given process
 *------------------------------------------------------------------------
 */
SYSCALL	setnok(int nok, int pid)
{
	unsigned long beginTime = ctr1000;
	STATWORD ps;    
	struct	pentry	*pptr;

	disable(ps);
	if (isbadpid(pid)) {
		restore(ps);
		if(traceFlag == 1){
			sysCallFreq[currpid][14]++;
			sysCallTimeSum[currpid][14] += ctr1000 - beginTime;
		}
		return(SYSERR);
	}
	pptr = &proctab[pid];
	pptr->pnxtkin = nok;
	restore(ps);
	if(traceFlag == 1) {
		sysCallFreq[currpid][14]++;
        	sysCallTimeSum[currpid][14] += ctr1000 - beginTime;
	}
	return(OK);
}
