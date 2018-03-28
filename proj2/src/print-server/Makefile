EXE=main
SRC=print_server_single.c printer_driver.c
CFLAGS=-D_GNU_SOURCE
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
