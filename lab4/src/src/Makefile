EXE=main
SRC=main.c printer_driver.c
CFLAGS=-D_GNU_SOURCE
LFLAGS=-lpthread
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
