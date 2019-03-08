/* user.c - main */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <stdio.h>

extern int zfunction(long);
/*------------------------------------------------------------------------
 *  main  --  user main program
 *------------------------------------------------------------------------
 */

int prX;
void halt();

prch(c)
char c;
{
	int i;
	sleep(5);	
}
int main()
{
	kprintf("\nzfunction:\n");
	kprintf("%x\n", zfunction(0xaabbccdd));
	printsegaddress();
	printtos();
	printprocstks(1);
	syscallsummary_start();
	resume(prX = create(prch,2000,20,"proc X",1,'A'));
	sleep(10);
	syscallsummary_stop();
	printsyscallsummary();
	return 0;
}
