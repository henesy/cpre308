CC=gcc
CFLAGS=-Wall -fPIC -c
LD=gcc
LDFLAGS=-shared -Wl,-soname,libprintserver.so -o libprintserver.so

hello: lib_print.c
	$(CC) $(CFLAGS) lib_print.c
	$(LD) $(LDFLAGS) lib_print.o

clean: 
	rm *.o
	rm *.so
