MOD_NAME = sched.mod
SRC = $(wildcard *.c)
CFLAGS = -Wall -g -I../../Scheduler/include
LFLAGS = 

OBJS := $(patsubst %c,%o,$(SRC))

all: $(MOD_NAME)

$(MOD_NAME): $(OBJS)
	gcc -shared -Wl,-soname,$@ $(LFLAGS) -o $@ $^

%.o: %.c
	gcc $(CFLAGS) -o $@ $(CFLAGS) -fPIC -c $<

clean:
	rm -f $(OBJS)
	rm -f $(MOD_NAME)

