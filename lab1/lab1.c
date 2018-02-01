#include "message.h"
	int main(int argc, char** argv) {
	char buf[1024];
	print_message();
	printf("You ran me as:%s\n", argv[0]);
	getcwd(buf, 1024);
	printf("Current working directory:%s\n", buf);
	return 0;
}

