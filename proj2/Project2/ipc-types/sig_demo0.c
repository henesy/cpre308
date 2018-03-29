#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

int main(int argc, char** argv)
{
	int pid;
	printf("pid: ");
	scanf("%d", &pid);
	
	kill(pid, 42);

	return 0;
}

