#include <stdlib.h>

void
main()
{
	pid_t child = fork();

	if(child > 0)
		sleep(10);
	else
		exit(0);

}

