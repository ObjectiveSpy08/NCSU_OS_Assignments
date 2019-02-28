/* setdev.c - setdev */

#include <conf.h>
#include <kernel.h>
#include <proc.h>

/*------------------------------------------------------------------------
 *  setdev  -  set the two device entries in the process table entry
 *------------------------------------------------------------------------
 */
SYSCALL	setdev(int pid, int dev1, int dev2)
{
	unsigned long beginTime = ctr1000;
	short	*nxtdev;

	if (isbadpid(pid)) {
		if(traceFlag == 1) {
			sysCallFreq[currpid][13]++;
			sysCallTimeSum[currpid][13] += ctr1000 - beginTime;
		}
		return(SYSERR);
	}
	nxtdev = (short *) proctab[pid].pdevs;
	*nxtdev++ = dev1;
	*nxtdev = dev2;
	if(traceFlag == 1) {
		sysCallFreq[currpid][13]++;
        	sysCallTimeSum[currpid][13] += ctr1000 - beginTime;
	}
	return(OK);
}
