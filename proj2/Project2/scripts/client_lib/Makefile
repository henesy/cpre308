default: library

library: print_server_client.o
	gcc -shared -Wl,-soname,libprintserver.so -o libprintserver.so print_server_client.o

print_server_client.o: print_server_client.c print_server_client.h
	gcc -Wall -Werror -fPIC -c print_server_client.c

clean:
	rm -f *.o *.so *~
