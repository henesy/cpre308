#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void my_quit_handler(int i)
{
	printf("\nI got the signal!\n");
	fflush(stdout);
}

int main(int argc, char** argv)
{
	printf("my pid: %d\n", getpid());
	signal(42, my_quit_handler);
	while(1)
	{
		printf(".");
		fflush(stdout);
		sleep(1);
	}
	return 0;
}

