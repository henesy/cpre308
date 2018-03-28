CC=gcc
CFLAGS=-Wall -fPIC -ggdb -c
LD=gcc
LDFLAGS=-shared -Wl,-soname,libprintserver.so -o libprintserver.so

all: print_server_client.c
	$(CC) $(CFLAGS) print_server_client.c
	$(LD) $(LDFLAGS) print_server_client.o

clean: 
	rm *.o
	rm *.so
