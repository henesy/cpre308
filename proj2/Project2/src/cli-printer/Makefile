EXE=cli-printer
SRC=cli-printer.c
CFLAGS=
LFLAGS=-pthread
DEBUG=-g -Wall

all:
	gcc -fPIC -o cli-printer -ggdb -D_GNU_SOURCE -L../libprintserver cli-printer.c -lprintserver

doc: 
	doxygen

clean:
	rm -rf *.o
	rm -rf $(EXE)
	
.PHONY: doc
