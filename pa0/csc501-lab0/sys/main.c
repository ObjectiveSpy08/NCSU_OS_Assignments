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

prcha(a)
char a;
{
	int i;
	sleep(5);
}

prchb(b)
char b;
{
	int i;
for(i=0;i<4;i++) sleep(1);
}


int prX, prA, prB;
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
	
	kprintf("\nTest case2: should print info for process \"main\" as before and process \"proc A\" with at least sys_sleep(count:1,average execution time:5000) and sys_sleep10(count:1,average execution time:5000) \n");
	syscallsummary_start();
	resume(prA = create(prcha,2000,20,"proc A",1,'A'));	
	sleep(10);
	syscallsummary_stop();	
	printsyscallsummary();

	kprintf("\nTest case3: should print info for process \"main\" and \"proc A\" as in the 2nd test case and \"proc B\" with at least sys_sleep(count:4,average execution time:1000) and sys_sleep10(count:4,average execution time:1000) \n");
	syscallsummary_start(); 
	resume(prA = create(prcha,2000,20,"proc A",1,'A'));	
	resume(prB = create(prchb,2000,20,"proc B",1,'B'));		
	sleep(10);
	syscallsummary_stop();	
	printsyscallsummary();
/*
	syscallsummary_start();
	resume(prX = create(prch,2000,20,"proc X",1,'A'));
	sleep(10);
	syscallsummary_stop();
	printsyscallsummary();
*/	return 0;
}
