CC=gcc
CFLAGS=-I. -ggdb
H = shell.h

%.o: %.c $(H)
	$(CC) -c -o $@ $< $(CFLAGS)

shell: main.o
	gcc -o shell main.o -I.

clean: 
	rm *.o
	rm shell

install: shell
	cp ./shell $(HOME)/bin/shell

