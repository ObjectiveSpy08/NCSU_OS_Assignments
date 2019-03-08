#include<kernel.h>
#include<proc.h>
#include<q.h>
#include<lock.h>

struct lentry rwlocks[NLOCKS];
int nextLock;

void linit(){
	int i;
	struct lentry *lptr;
	for (i = 0; i < NLOCKS ; i++) {	/* initialize read write locks */
		(lptr = &rwlocks[i])->lstate = LFREE;
		lptr->lseq = 0;
	}
	nextLock = 0;
}
