#include<stdio.h>
#include<kernel.h>
#include<proc.h>

void printprocstks(int priority) {
	kprintf("\nvoid printprocstcks(int priority)");
	const long register *esp asm("esp");
	int i = 0;
	for(; i < NPROC; i++) {
		if(proctab[i].pprio > priority) {
			kprintf("\nProcess [%s]", proctab[i].pname);
			kprintf("\n\tpid: %d", i);
			kprintf("\n\tpriority: %d", proctab[i].pprio);
			kprintf("\n\tbase: 0x%08x", proctab[i].pbase);
			kprintf("\n\tlimit: 0x%08x", proctab[i].plimit);
			kprintf("\n\tlen: %d", proctab[i].pstklen);
			kprintf("\n\tpointer: 0x%08x", i == currpid ? esp : proctab[i].pesp);
		}
	}
}
