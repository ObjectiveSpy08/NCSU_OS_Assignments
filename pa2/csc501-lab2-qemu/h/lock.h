#ifndef _LOCK_H_
#define _LOCK_H_

#ifndef	NLOCKS
#define	NLOCKS		50	/* number of locks, if not defined	*/
#endif

#define	READ	'\01'		/* acquire lock for reading		*/
#define	WRITE	'\02'		/* acquire lock for writers 		*/

/* states */
#define LFREE	'\03'		/* lock is avaiable			*/
#define	LUSED	'\04'		/* lock is used				*/
#define LDELETED '\05'		/* lock is deleted			*/

struct	lentry	{		/* lock table entry			*/
	char	lstate;		/* the state				*/
	int	lread_cnt;	/* number of readers			*/
	int 	lrw_mutex;	/* semaphore for writers sync.		*/
	int	lr_mutex;	/* mutex for reader count		*/
	int	lseq;		/* lock sequence for deletion		*/
};
extern	struct	lentry	rwlocks[];
extern	int	nextLock;

#define	isbadLock(l)	(l < 0 || l >= NLOCKS)

int lcreate();
void linit();
#endif

