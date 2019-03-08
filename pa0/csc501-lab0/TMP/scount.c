/* scount.c - scount */

#include <conf.h>
#include <kernel.h>
#include <sem.h>
#include<proc.h>
/*------------------------------------------------------------------------
 *  scount  --  return a semaphore count
 *------------------------------------------------------------------------
 */
SYSCALL scount(int sem)
{
	unsigned long beginTime = ctr1000;	
extern	struct	sentry	semaph[];

	if (isbadsem(sem) || semaph[sem].sstate==SFREE){
		if(traceFlag == 1) {
			sysCallFreq[currpid][10]++;
	        	sysCallTimeSum[currpid][10] += ctr1000 - beginTime;
		}
		return(SYSERR);
	}
	if(traceFlag == 1) {
		sysCallFreq[currpid][10]++;
        	sysCallTimeSum[currpid][10] += ctr1000 - beginTime;
	}
	return(semaph[sem].semcnt);
}
