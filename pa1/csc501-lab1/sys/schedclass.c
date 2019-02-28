#include<kernel.h>
#include<proc.h>

int schedclass = 0;

void setschedclass(int sched_class) {
	schedclass = sched_class;
}

int getschedclass(){
	return schedclass;
}
