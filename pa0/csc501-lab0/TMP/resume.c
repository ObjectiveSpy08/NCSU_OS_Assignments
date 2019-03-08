/* resume.c - resume */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 * resume  --  unsuspend a process, making it ready; return the priority
 *------------------------------------------------------------------------
 */
SYSCALL resume(int pid)
{
	unsigned long beginTime = ctr1000;
	STATWORD ps;    
	struct	pentry	*pptr;		/* pointer to proc. tab. entry	*/
	int	prio;			/* priority to return		*/

	disable(ps);
	if (isbadpid(pid) || (pptr= &proctab[pid])->pstate!=PRSUSP) {
		restore(ps);
		if(traceFlag == 1) {
			sysCallFreq[currpid][9]++;
	        	sysCallTimeSum[currpid][9] += ctr1000 - beginTime;
		}
		return(SYSERR);
	}
	prio = pptr->pprio;
	ready(pid, RESCHYES);
	restore(ps);
	if(traceFlag == 1) {
		sysCallFreq[currpid][9]++;
        	sysCallTimeSum[currpid][9] += ctr1000 - beginTime;
	}
	return(prio);
}
