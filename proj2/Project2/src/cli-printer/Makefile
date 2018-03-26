EXE=cli-printer
SRC=cli-printer.c
CFLAGS=-D_GNU_SOURCE -L../libprintserver -I../libprintserver -lprint
LFLAGS=-pthread
DEBUG=-g -Wall


OBJ := $(patsubst %.c,%.o,$(SRC))

all: $(EXE)

$(EXE): $(OBJ)
	gcc -o $@ $^ $(LFLAGS)

%.o: %.c *.h
	gcc -c $< $(CFLAGS) $(DEBUG)

doc: 
	doxygen

clean:
	rm -rf *.o
	rm -rf $(EXE)
	
.PHONY: doc

