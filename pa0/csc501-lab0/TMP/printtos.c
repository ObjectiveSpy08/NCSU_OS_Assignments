#include<stdio.h>
#include<kernel.h>

void printtos() {
	kprintf("\n\nvoid printtos()\n");
	const long register *ebp asm("ebp");
	kprintf("Before [0x%08x]: 0x%08x\n", ebp + 1, *(ebp + 1));
	kprintf("After  [0x%08x]: 0x%08x\n",ebp, *ebp);
	int i = 1;
	for(; i<= 4; i++) {
		kprintf("\telement[0x%08x]: 0x%08x\n", *(ebp - i));
	}
}
