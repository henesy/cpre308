#include <unistd.h>
#include <stdio.h>

main() {
	execl("/bin/ls", "ls", NULL);
	printf("What happened?\n");
}

