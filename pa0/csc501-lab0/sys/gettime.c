/* gettime.c - gettime */

#include <conf.h>
#include <kernel.h>
#include <date.h>
#include<proc.h>

extern int getutim(unsigned long *);

/*------------------------------------------------------------------------
 *  gettime  -  get local time in seconds past Jan 1, 1970
 *------------------------------------------------------------------------
 */
SYSCALL	gettime(long *timvar)
{
	
    	/* long	now; */
	/* FIXME -- no getutim */
	if(traceFlag == 1) {
		sysCallFreq[currpid][4]++;
	}
    	return OK;
}
