/* fat12ls.c 
* 
*  Displays the files in the root sector of an MSDOS floppy disk
*/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>

#define SIZE 32      /* size of the read buffer */
#define ROOTSIZE 256 /* max size of the root directory */
//define PRINT_HEX   // un-comment this to print the values in hex for debugging

struct BootSector
{
    unsigned char  sName[9];            // The name of the volume
    unsigned short iBytesSector;        // Bytes per Sector
    
    unsigned char  iSectorsCluster;     // Sectors per Cluster
    unsigned short iReservedSectors;    // Reserved sectors
    unsigned char  iNumberFATs;         // Number of FATs
    
    unsigned short iRootEntries;        // Number of Root Directory entries
    unsigned short iLogicalSectors;     // Number of logical sectors
    unsigned char  xMediumDescriptor;   // Medium descriptor
    
    unsigned short iSectorsFAT;         // Sectors per FAT
    unsigned short iSectorsTrack;       // Sectors per Track
    unsigned short iHeads;              // Number of heads
    
    unsigned short iHiddenSectors;      // Number of hidden sectors
};

void parseDirectory(int iDirOff, int iEntries, unsigned char buffer[]);
//  Pre: Calculated directory offset and number of directory entries
// Post: Prints the filename, time, date, attributes and size of each entry

unsigned short endianSwap(unsigned char one, unsigned char two);
//  Pre: Two initialized characters
// Post: The characters are swapped (two, one) and returned as a short

void decodeBootSector(struct BootSector * pBootS, unsigned char buffer[]);
//  Pre: An initialized BootSector struct and a pointer to an array
//       of characters read from a BootSector
// Post: The BootSector struct is filled out from the buffer data

char * toDOSName(char string[], unsigned char buffer[], int offset);
//  Pre: String is initialized with at least 12 characters, buffer contains
//       the directory array, offset points to the location of the filename
// Post: fills and returns a string containing the filename in 8.3 format

char * parseAttributes(char string[], unsigned char key);
//  Pre: String is initialized with at least five characters, key contains
//       the byte containing the attribue from the directory buffer
// Post: fills the string with the attributes

char * parseTime(char string[], unsigned short usTime);
//  Pre: string is initialzied for at least 9 characters, usTime contains
//       the 16 bits used to store time
// Post: string contains the formatted time

char * parseDate(char string[], unsigned short usDate);
//  Pre: string is initialized for at least 13 characters, usDate contains
//       the 16 bits used to store the date
// Post: string contains the formatted date

int roundup512(int number);
// Pre: initialized integer
// Post: number rounded up to next increment of 512


// reads the boot sector and root directory
int main(int argc, char * argv[])
{
    int pBootSector = 0;
    unsigned char buffer[SIZE];
    unsigned char rootBuffer[ROOTSIZE * 32];
    struct BootSector sector;
    int iRDOffset = 0;
    
    // Check for argument
    if (argc < 2) {
    	printf("Specify boot sector\n");
    	exit(1);
    }
    
    // Open the file and read the boot sector
    pBootSector = open(argv[1], O_RDONLY);
    read(pBootSector, buffer, SIZE);
    
    // Decode the boot Sector
    decodeBootSector(&sector, buffer);
    
    // Calculate the location of the root directory
    iRDOffset = (1 + (sector.iSectorsFAT * sector.iNumberFATs) )
                 * sector.iBytesSector;
    
    // Read the root directory into buffer
    lseek(pBootSector, iRDOffset, SEEK_SET);
    read(pBootSector, rootBuffer, ROOTSIZE);
    close(pBootSector);
    
    // Parse the root directory
    parseDirectory(iRDOffset, sector.iRootEntries, rootBuffer);
    
} // end main


// Converts two characters to an unsigned short with two, one
unsigned short endianSwap(unsigned char one, unsigned char two)
{
    // This is stub code!
    return 0x0000;
}


// Fills out the BootSector Struct from the buffer
void decodeBootSector(struct BootSector * pBootS, unsigned char buffer[])
{
    int i = 3;  // Skip the first 3 bytes
    
    // Pull the name and put it in the struct (remember to null-terminate)
    
    // Read bytes/sector and convert to big endian
    
    // Read sectors/cluster, Reserved sectors and Number of Fats
    
    // Read root entries, logicical sectors and medium descriptor
    
    // Read and covert sectors/fat, sectors/track, and number of heads
    
    // Read hidden sectors
    
    return;
}


// iterates through the directory to display filename, time, date,
// attributes and size of each directory entry to the console
void parseDirectory(int iDirOff, int iEntries, unsigned char buffer[])
{
    int i = 0;
    char string[13];
    
    // Display table header with labels
    printf("Filename\tAttrib\tTime\t\tDate\t\tSize\n");
    
    // loop through directory entries to print information for each
    for(i = 0; i < (iEntries); i = i + /* entry width */)   {
    	if (  /* valid file */ ) {
    		// Display filename
    		printf("%s\t", toDOSName(string, buffer, /*name offset*/)  );
    		// Display Attributes
    		printf("%s\t", parseAttributes(string, /* attr offset */)  );
    		// Display Time
    		printf("%s\t", parseTime(string, /*time offsets */ )  );
    		// Display Date
    		printf("%s\t", parseDate(string, /*date offsets */ )  );
    		// Display Size
    		printf("%d\n", /* size offsets */ );
    	}
    }
    
    // Display key
    printf("(R)ead Only (H)idden (S)ystem (A)rchive\n");
} // end parseDirectory()


// Parses the attributes bits of a file
char * parseAttributes(char string[], unsigned char key)
{
    // This is stub code!
    return string;
} // end parseAttributes()


// Decodes the bits assigned to the time of each file
char * parseTime(char string[], unsigned short usTime)
{
    unsigned char hour = 0x00, min = 0x00, sec = 0x00;
    
    // DEBUG: printf("time: %x", usTime);
    
    // This is stub code!
    
    sprintf(string, "%02i:%02i:%02i", hour, min, sec);
    
    return string;
    
    
} // end parseTime()


// Decodes the bits assigned to the date of each file
char * parseDate(char string[], unsigned short usDate)
{
    unsigned char month = 0x00, day = 0x00;
    unsigned short year = 0x0000;
    
    //printf("date: %x", usDate);
    
    // This is stub code!
    
    sprintf(string, "%d/%d/%d", year, month, day);
    
    return string;
    
} // end parseDate()


// Formats a filename string as DOS (adds the dot to 8-dot-3)
char * toDOSName(char string[], unsigned char buffer[], int offset)
{
    // This is stub code!
    return string;
} // end toDosNameRead-Only Bit

