EXE = main

CPPSRC := $(wildcard *.cpp) $(wildcard */*.cpp)
CPPOBJ := $(patsubst %.cpp,%.cpp.o,$(CPPSRC))

CSRC := $(wildcard *.c) $(wildcard */*.c)
COBJ := $(patsubst %.c,%.c.o,$(CSRC))

CFLAGS = -Wall
CXXFLAGS = -Wall -std=c++10
LFLAGS = 

all: $(EXE)
	
$(EXE): $(CPPOBJ) $(COBJ)
	g++ $(LFLAGS) -o $@ $^

%.cpp.o: %.cpp
	g++ $(CXXFLAGS) -o $@ -c $<

%.c.o: %.c
	gcc $(CFLAGS) -o $@ -c $<


clean:
	rm -f $(CPPOBJ)
	rm -f $(COBJ)
	rm -f main

.PHONY: all $(SUBDIRS)





