CC=gcc
CFLAGS=-Wall -fPIC -c
LD=gcc
LDFLAGS=-shared -Wl,-soname,libhello.so -o libhello.so

all: hello
	$(CC) -L./ lib_test.c -lhello -o lib_test

hello: lib_hello.c
	$(CC) $(CFLAGS) lib_hello.c
	$(LD) $(LDFLAGS) lib_hello.o

clean: 
	rm *.o
	rm *.so
