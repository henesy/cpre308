cmd_drivers/char/built-in.o :=  ld  -m elf_x86_64   -r -o drivers/char/built-in.o drivers/char/mem.o drivers/char/random.o drivers/char/misc.o drivers/char/agp/built-in.o 
