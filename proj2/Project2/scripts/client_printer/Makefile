default: cli-printer

cli-printer: cli-printer.o
	gcc -L../libprintserver cli-printer.o -lprintserver -o cli-printer

cli-printer.o: cli-printer.c
	gcc -Wall -Werror -c cli-printer.c

clean:
	rm -f *.o *~ cli-printer
	
