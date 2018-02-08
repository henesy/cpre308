# run 'make' without the quotes to compile.
CC=gcc
CFLAGS=-I. -ggdb
H = shell.h

%.o: %.c $(H)
	$(CC) -c -o $@ $< $(CFLAGS)

shell: main.o list.o
	gcc -o shell main.o list.o -I.

clean: 
	rm *.o
	rm shell

install: shell
	cp ./shell $(HOME)/bin/shell

