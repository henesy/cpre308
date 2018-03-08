EXE=virt-printer
SRC=virt-printer.c
LFLAGS=
CFLAGS=-Wall -g

OBJ := $(patsubst %.c,%.o,$(SRC))

$(EXE): $(OBJ)
	$(CC) -o $@ $^ $(LFLAGS)

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

clean:
	rm -f $(OBJ)
	rm -f $(EXE)
	rm -f ./drivers/*
