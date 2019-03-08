#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <stdio.h>
#include <lock.h>

#define DEFAULT_LOCK_PRIO 20


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

int main()
{
        test4();
        test5();
        /* The hook to shutdown QEMU for process-like execution of XINU.
         * This API call terminates the QEMU process.
         */
        shutdown();
}

