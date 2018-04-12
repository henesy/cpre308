/*
 *  bytedump.c
 *
 *  This program reads a set number of bytes from a file and prints
 *  out the hex/ASCII values one byte at a time.
 *
 *  Compile:  gcc bytedump.c -o bytedump
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

#define SIZE 32  /* size of the read buffer */

/* helper functions for printing out the buffer */
char printable(char c);
void bytedump(unsigned char *buf, int offset);


int main(int argc, char *argv[])
{

    int floppy;  /* file descriptor */
    int offset;  /* offset into the file */

    unsigned char buf[SIZE];  /* read buffer */

    /* check args */
    if (argc < 3) {
        printf("%s name offset\n", argv[0]);
        exit(1);
    }

    offset = atoi(argv[2]);

    /* open the file and read from the requested offset */
    floppy = open(argv[1], O_RDONLY);
    lseek(floppy, offset, SEEK_SET);
    read(floppy, buf, SIZE);

    /* print out the buffer */
    bytedump(buf, offset);
    printf("\n");
    return 0;

}


/* prints out the buffer one byte at a time */
void bytedump(unsigned char *buf, int offset)
{
    int i;
    printf("addr   value  ascii\n");
    for(i=0; i<SIZE; i++)
    {
        printf("0x%04x 0x%02x   %c\n", i+offset, buf[i], printable(buf[i]));
    }

}

/* returns the character if it's printable, otherwise a space */
char printable(char c)
{
    if (isgraph(c))
        return c;
    else
        return ' ';
}


