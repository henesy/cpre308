CC=gcc
CFLAGS=

all: bsdump

bsdump: bsdump.c
	$(CC) -o bsdump $(CFLAGS) bsdump.c

bytedump: bytedump.c
	$(CC) -o bytedump $(CFLAGS) bytedump.c

clean: 
	rm a.out
	rm bsdump
	rm bytedump