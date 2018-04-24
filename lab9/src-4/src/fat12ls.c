/* fat12ls.c 
* 
*  Displays the files in the root sector of an MSDOS floppy disk
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>

#define SIZE 32      /* size of the read buffer */
#define ROOTSIZE 256 /* max size of the root directory */
//define PRINT_HEX   // un-comment this to print the values in hex for debugging
#define nil ((void*)0)

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

char * parseAttributes(unsigned char buffer[], int);
//  Pre: String is initialized with at least five characters, key contains
//       the byte containing the attribue from the directory buffer
// Post: fills the string with the attributes

char * parseTime(unsigned char buffer[], int);
//  Pre: string is initialzied for at least 9 characters, usTime contains
//       the 16 bits used to store time
// Post: string contains the formatted time

char * parseDate(unsigned char buffer[], int);
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
//unsigned short endianSwap(unsigned char one, unsigned char two)
//{
//   // This is stub code!
//    return 0x0000;
//}

/* !! The lab machines are little endian, so this should work consistenly (tested on lab machine) */
// Converts two characters to int (unsigned short) -- I'd rather use this than endianSwap
int
two2int(unsigned char buffer[], int i)
{
	return ((buffer[i+1] << 8) & 0xFF00) | (buffer[i] & 0xFF);
}

// Fills out the BootSector Struct from the buffer
void decodeBootSector(struct BootSector * pBootS, unsigned char buffer[])
{
    int i = 3;  // Skip the first 3 bytes
    
   	// Pull the name and put it in the struct (remember to null-terminate) -- 3-10
	char* name = calloc(9, sizeof(char));
	for(; i < 11; i++)
		name[i-3] = buffer[i];
	name[7] = '\0';
	strcpy(pBootS->sName, name);
	free(name);
    
	// Read bytes/sector and convert to big endian -- 11-12
	int bytesec = two2int(buffer, 11);
	pBootS->iBytesSector = bytesec;
    
	// Read sectors/cluster, Reserved sectors and Number of Fats -- 13, 14-15, 16
	int secclust = buffer[13] & 0xFF;
	pBootS->iSectorsCluster = secclust;
	
	pBootS->iReservedSectors = two2int(buffer, 14);
	
	pBootS->iNumberFATs = buffer[16] & 0xFF;
    
	// Read root entries, logicical sectors and medium descriptor -- 17-18, 19-20, 21
	pBootS->iRootEntries = two2int(buffer, 17);
	
	pBootS->iLogicalSectors = two2int(buffer, 19);
	
	// Use the raw hex ☺
	pBootS->xMediumDescriptor = buffer[21];
    
	// Read and covert sectors/fat, sectors/track, and number of heads -- 22-23, 24-25, 26-27
	pBootS->iSectorsFAT = two2int(buffer, 22);
	
	pBootS->iSectorsTrack = two2int(buffer, 24);
	
	pBootS->iHeads = two2int(buffer, 26);
    
	// Read hidden sectors -- 28-31 (4 byte value)
	int32_t hidsec = ((buffer[31]) << 24) | ((buffer[30]) << 16) | ((buffer[29]) << 8) | (buffer[28]);
	pBootS->iHiddenSectors = hidsec;
    
    return;
}


// If the filename starts with 0x00, it’s not a valid entry.  If it starts with 0xE5, it’s also an invalid entry, but it has been released/deleted.
int
validfile(unsigned char buffer[], int offset)
{
	if(buffer[offset] == 0xE5 || buffer[offset] == 0x00)
		return 0;
	return 1;
}


// iterates through the directory to display filename, time, date,
// attributes and size of each directory entry to the console
void parseDirectory(int iDirOff, int iEntries, unsigned char buffer[])
{
    int i = 0;
    char string[13];
    int entrywidth = 32;
    int nameoff = 0;
    int attroff = 8 + 3;
    int timeoff = 8 + 3 + 1 + 10;
    int dateoff = 8 + 3 + 1 + 10 + 2;
    int sizeoff = 8 + 3 + 1 + 10 + 2 + 2 +2;
    
    // Display table header with labels
    printf("Filename\tAttrib\tTime\t\tDate\t\tSize (bytes)\n");
    
    // loop through directory entries to print information for each
    for(i = 0; i < (iEntries); i = i + entrywidth)   {
    	if (validfile(buffer, i)) {
    		// Display filename -- filename . extension ☺
    		printf("%s\t", toDOSName(string, buffer, i+nameoff)  );
    		// Display Attributes
    		printf("%s\t", parseAttributes(buffer, i+attroff)  );
    		// Display Time
    		printf("%s\t", parseTime(buffer, i+timeoff)  );
    		// Display Date
    		printf("%s\t", parseDate(buffer, i+dateoff)  );
    		// Display Size -- use 4 byte little endian magic used in boot sector code
    		int32_t fsize = ((buffer[i+sizeoff+3]) << 24) | ((buffer[i+sizeoff+2]) << 16) | ((buffer[i+sizeoff+1]) << 8) | (buffer[i+sizeoff]);
    		printf("%d\n", fsize);
    	}
    }
    
    // Display key
    printf("(R)ead Only (H)idden (S)ystem (A)rchive\n");
} // end parseDirectory()


// convert a char to a binary string
char*
char2bin(char c)
{
	char* str = calloc(9, sizeof(char));
	int i, j = 0;
    for (i = 7; i >= 0; --i, j++){
        str[j] = ((c & (1 << i)) ? '1' : '0');
    }
    str[8] = '\0';
	return str;
}


// Convert a binary string to an int
int
bin2int(char* str)
{
	return (int) strtol(str, nil, 2);
}


// Function to reverse a string
void
reverse(char* str )
{ 
    int i, j, len;
    char* tmp = calloc(strlen(str)+1, sizeof(char));
      
    // calculating length of the string
    len = strlen(str);
      
    for(j = 0, i = len-1; i >= 0; i--, j++)
		tmp[j] = str[i];
	
	tmp[j] = '\0';
	strcpy(str, tmp);
	free(tmp);
}


// Parses the attributes bits of a file
char*
parseAttributes(unsigned char buffer[], int offset)
{
	char* attrstr = calloc(5, sizeof(char));
	char attr[9];
	int i;
	strcpy(attrstr, "----");
	
	
	strcpy(attr, char2bin(buffer[offset]));
	reverse(attr);
	
	//sprintf(attr, "%x", buffer[offset]);
	//printf("\n[ATTRBIN: %s]\n", attr);
	
	for(i = 0; i < 8; i++){
		if(i == 0 && attr[i] == '1')
			attrstr[0] = 'R';
		if(i == 1 && attr[i] == '1')
			attrstr[1] = 'H';
		if(i == 2 && attr[i] == '1')
			attrstr[2] = 'S';
		if(i == 5 && attr[i] == '1')
			attrstr[3] = 'A';
	}
	
	attrstr[4] = '\0';
    return attrstr;
} // end parseAttributes()


// Decodes the bits assigned to the time of each file
char*
parseTime(unsigned char buffer[], int offset)
{
    int hour = 0, min = 0, sec = 0;
    char datebin[17];
    char* string = calloc(9, sizeof(char));
    int i;
    
    // Reversing the order of the string gave invalid time values, so don't reverse I suppose
	//printf("\n[DATE: %x,%x]\n", buffer[offset], buffer[offset+1]);
	strcpy(datebin, char2bin(buffer[offset]));
	strcat(datebin, char2bin(buffer[offset+1]));
	//reverse(datebin);
	//printf("\n[DATEBIN: %s]\n", datebin);
	
	char shour[6];
	char smin[7];
	char ssec[6];
	strncpy(shour, datebin, 5);
	strncpy(smin, datebin+5, 6);
	strncpy(ssec, datebin+11, 5);
	
	hour = bin2int(shour);
	min = bin2int(smin);
	sec = bin2int(ssec);
	// Since every other second is counted
	sec *= 2;
    
    // DEBUG: printf("time: %x", usTime);
    
    // This is stub code!
    
    sprintf(string, "%02i:%02i:%02i", hour, min, sec);
    
    return string;
    
    
} // end parseTime()


// Decodes the bits assigned to the date of each file
char *
parseDate(unsigned char buffer[], int offset)
{
    unsigned char month = 0x00, day = 0x00;
    unsigned short year = 0x0000;
    //char* string = calloc(11, sizeof(char));
    
	char datebin[17];
    char* string = calloc(9, sizeof(char));
    int i;
    
    // Reversing the order of the string gave invalid time values, so don't reverse I suppose
	//printf("\n[DATE: %x,%x]\n", buffer[offset], buffer[offset+1]);
	strcpy(datebin, char2bin(buffer[offset]));
	strcat(datebin, char2bin(buffer[offset+1]));
	//reverse(datebin);
	//printf("\n[DATEBIN: %s]\n", datebin);
	
	char syear[8];
	char smon[5];
	char sday[6];
	strncpy(syear, datebin, 7);
	strncpy(smon, datebin+7, 4);
	strncpy(sday, datebin+11, 5);
	
	year = bin2int(syear);
	month = bin2int(smon);
	day = bin2int(sday);
	// Since year offset is 1980
	year += 1980;
    
    //printf("date: %x", usDate);
    
    // This is stub code!
    
    sprintf(string, "%d/%d/%d", year, month, day);
    
    return string;
    
} // end parseDate()


// Formats a filename string as DOS (adds the dot to 8-dot-3)
char * toDOSName(char string[], unsigned char buffer[], int offset)
{
	// filename is 8 bytes, ext is 3 bytes, insert a '.'
	int i;
	char* name = calloc(9, sizeof(char));
	char* ext = calloc(4, sizeof(char));
	for(i = 0; i < 8; i++){
		// Don't copy spaces for now -- should check if only whitespace pads the right side of name to strip ☺
		if(buffer[offset+i-1] != 0x20)
			name[i] = buffer[offset+i];
		else
			name[i] = '\0';
	}
	//name[7] = '\0';
	for(; i-8 < 3; i++)
		ext[i-8] = buffer[offset+i];
	ext[3] = '\0';
	
	//printf("NAME: %s\n", name);
	//printf("EXT: %s\n", ext);
	
	strcpy(string, name);
	strcat(string, ".");
	strcat(string, ext);
	
	free(name);
	free(ext);

    return string;
} // end toDosNameRead-Only Bit

