/* user.c - main */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <stdio.h>
#include <lock.h>

#define DEFAULT_LOCK_PRIO 20

#define assert(x,error) if(!(x)){ \
            kprintf(error);\
            return;\
            }

int mystrncmp(char* des,char* target,int n){
    int i;
    for (i=0;i<n;i++){
        if (target[i] == '.') continue;
        if (des[i] != target[i]) return 1;
    }
    return 0;
}

void reader1 (char *msg, int lck)
{
	lock (lck, READ, DEFAULT_LOCK_PRIO);
	kprintf ("  %s: acquired lock, sleep 2s\n", msg);
	sleep (2);
	kprintf ("  %s: to release lock\n", msg);
	releaseall (1, lck);
}

void test1 ()
{
	int	lck;
	int	pid1;
	int	pid2;

	kprintf("\nTest 1: readers can share the rwlock\n");
	lck  = lcreate ();
	assert (lck != SYSERR, "Test 1 failed");

	pid1 = create(reader1, 2000, 20, "reader a", 2, "reader a", lck);
	pid2 = create(reader1, 2000, 20, "reader b", 2, "reader b", lck);

	resume(pid1);
	resume(pid2);
	
	sleep (5);
	ldelete (lck);
	kprintf ("Test 1 ok\n");
}

char output2[10];
int count2;
void reader2 (char msg, int lck, int lprio)
{
        int     ret;

        kprintf ("  %c: to acquire lock\n", msg);
        lock (lck, READ, lprio);
        output2[count2++]=msg;
        kprintf ("  %c: acquired lock, sleep 3s\n", msg);
        if(msg=='B'){ sleep(2);}
        else {sleep (3);}
        output2[count2++]=msg;
        kprintf ("  %c: to release lock\n", msg);
	releaseall (1, lck);
}

void writer2 (char msg, int lck, int lprio)
{
	kprintf ("  %c: to acquire lock\n", msg);
        lock (lck, WRITE, lprio);
        output2[count2++]=msg;
        kprintf ("  %c: acquired lock, sleep 3s\n", msg);
	sleep (3);
        output2[count2++]=msg;
        kprintf ("  %c: to release lock\n", msg);
        releaseall (1, lck);
}


void test2 ()
{
        count2 = 0;
        int     lck;
        int     rd1, rd2, rd3, rd4;
        int     wr1;

        kprintf("\nTest 2: wait on locks with priority. Expected order of"
		" lock acquisition is: reader A, reader B, reader D, writer C & reader E\n");
        lck  = lcreate ();
        assert (lck != SYSERR, "Test 2 failed");

	rd1 = create(reader2, 2000, 20, "reader1", 3, 'A', lck, 20);
	rd2 = create(reader2, 2000, 20, "reader2", 3, 'B', lck, 30);
	rd3 = create(reader2, 2000, 20, "reader3", 3, 'D', lck, 25);
	rd4 = create(reader2, 2000, 20, "reader4", 3, 'E', lck, 20);
        wr1 = create(writer2, 2000, 20, "writer2", 3, 'C', lck, 28);
	
        kprintf("-start reader A, then sleep 1s. lock granted to reader A\n");
        resume(rd1);
        sleep (1);

        kprintf("-start writer C, then sleep 1s. writer waits for the lock\n");
        resume(wr1);
        sleep10 (1);


        kprintf("-start reader B, D, E. reader B is granted lock.\n");
        resume (rd2);
	resume (rd3);
	resume (rd4);


        sleep (15);
        kprintf("output=%s\n", output2);
        assert(mystrncmp(output2,"ABABCCDEED",10)==0,"Test 2 FAILED\n");
        kprintf ("Test 2 OK\n");
}

/*----------------------------------Test 3---------------------------*/
void reader3 (char *msg, int lck)
{
        int     ret;

        kprintf ("  %s: to acquire lock\n", msg);
        lock (lck, READ, DEFAULT_LOCK_PRIO);
        kprintf ("  %s: acquired lock\n", msg);
        kprintf ("  %s: to release lock\n", msg);
        releaseall (1, lck);
}

void writer3 (char *msg, int lck)
{
        kprintf ("  %s: to acquire lock\n", msg);
        lock (lck, WRITE, DEFAULT_LOCK_PRIO);
        kprintf ("  %s: acquired lock, sleep 10s\n", msg);
        sleep (10);
        kprintf ("  %s: to release lock\n", msg);
        releaseall (1, lck);
}


void test3 ()
{
        int     lck;
        int     rd1, rd2;
        int     wr1;

        kprintf("\nTest 3: test the basic priority inheritence\n");
        lck  = lcreate ();
        assert (lck != SYSERR, "Test 3 failed");

        rd1 = create(reader3, 2000, 25, "reader3", 2, "reader A", lck);
        rd2 = create(reader3, 2000, 30, "reader3", 2, "reader B", lck);
        wr1 = create(writer3, 2000, 20, "writer3", 2, "writer", lck);

        kprintf("-start writer, then sleep 1s. lock granted to write (prio 20)\n");
        resume(wr1);
        sleep (1);

        kprintf("-start reader A, then sleep 1s. reader A(prio 25) blocked on the lock\n");
        resume(rd1);
        sleep (1);
	assert (getprio(wr1) == 25, "Test 3 failed");

        kprintf("-start reader B, then sleep 1s. reader B(prio 30) blocked on the lock\n");
        resume (rd2);
	sleep (1);
	assert (getprio(wr1) == 30, "Test 3 failed");
	
	kprintf("-kill reader B, then sleep 1s\n");
	kill (rd2);
	sleep (1);
	assert (getprio(wr1) == 25, "Test 3 failed");

	kprintf("-kill reader A, then sleep 1s\n");
	kill (rd1);
	sleep(1);
	assert(getprio(wr1) == 20, "Test 3 failed");

        sleep (10);
        kprintf ("Test 3 OK\n");
}

void p1(char *msg, int lck) {
	while(1) kprintf("W");
}

void p2(char *msg, int lck) {
        kprintf ("  %s: to acquire lock\n", msg);
        wait(lck);
        kprintf ("  %s: acquired lock\n", msg);
        kprintf ("  %s: to release lock\n", msg);
        signal(lck);
}


void p3(char *msg, int lck)
{
        kprintf ("  %s: to acquire lock\n", msg);
        wait(lck);
        kprintf ("  %s: acquired lock, sleep 10s\n", msg);
        sleep(1);
        kprintf ("  %s: to release lock\n", msg);
        signal(lck);
}

void test4 ()
{
        int     lck;
        int     po1, po2, po3;

        kprintf("\nTest 4: test the no priority inheritance with simple semaphore\n");
        lck  = screate (1);

        po1 = create(p1, 2000, 18, "p1", 2, "p1", lck);
        po2 = create(p2, 2000, 19, "p2", 2, "p2", lck);
        po3 = create(p3, 2000, 16, "p3", 2, "p3", lck);

        kprintf("-start p3, then sleep. lock granted\n");
        resume(po3);
        sleep1000(2);

        kprintf("-start p1, infinte loop\n");
        resume(po1);
        sleep1000(1);

        kprintf("-start p2, blocked on the lock\n");
        resume (po2);
        sleep1000(1);

        kprintf ("Test 4 OK\n");
}

void p4(char *msg, int lck) {
        while(1) kprintf("W");
}

void p5(char *msg, int lck) {
        kprintf ("  %s: to acquire lock\n", msg);
        lock (lck, WRITE, DEFAULT_LOCK_PRIO);
        kprintf ("  %s: acquired lock\n", msg);
        kprintf ("  %s: to release lock\n", msg);
        releaseall(1, lck);
}


void p6(char *msg, int lck)
{
        kprintf ("  %s: to acquire lock\n", msg);
        lock (lck, WRITE, DEFAULT_LOCK_PRIO);
        kprintf ("  %s: acquired lock, sleep 10s\n", msg);
        sleep1000(1);
        kprintf ("  %s: to release lock\n", msg);
        releaseall(1,lck);
}

void test5 ()
{
        int     lck;
        int     po4, po5, po6;

        kprintf("\nTest 5: test the no priority inheritance with simple semaphore\n");
        lck  = lcreate (1);

        po4 = create(p4, 2000, 18, "p4", 2, "p4", lck);
        po5 = create(p5, 2000, 19, "p5", 2, "p5", lck);
        po6 = create(p6, 2000, 16, "p6", 2, "p6", lck);

        kprintf("-start p6, then sleep. lock granted\n");
        resume(po6);
        sleep1000(1);

        kprintf("-start p4. Infinite loop\n");
        resume(po4);
        sleep100(2);

        kprintf("-start p5, blocked on the lock\n");
        resume (po5);
        sleep100(1);

        kprintf ("Test 5 OK\n");
}

/*------------------------------------------------------------------------
 *  main  --  user main program
 *------------------------------------------------------------------------
 */
int main()
{
	kprintf("\n\nHello World, Xinu@QEMU lives\n\n");

//	test1();
//	test2();
//	test3();
	test5();
        /* The hook to shutdown QEMU for process-like execution of XINU.
         * This API call terminates the QEMU process.
         */
        shutdown();
}
