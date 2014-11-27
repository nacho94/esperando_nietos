#include <stdio.h>
#include <unistd.h>




int main(void) {

	printf("pid= %d, ppid= %d",getpid(),getppid());
	return 0;
}