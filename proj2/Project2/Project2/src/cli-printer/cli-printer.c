/**
 * @file      cli-printer.c
 * @author    Brody Concannon
 * @brief     Library to connect client to print server
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>

#include <sys/types.h>
#include <netdb.h>
#include <sys/un.h>

#include "../libprintserver/print_server_client.h"

static void parse_command_line(int argc, char * argv[]);
static void check_job_info();
static void free_pointers();
static void test_print_list();
char *remove_ext (char* mystr, char dot, char sep);

printer_driver_t** list;
int n;
char * driver;
char * job_name;
char * description;
char * data;

int main(int argc, char* argv[])
{
	char * line = NULL;
	size_t n = 0;

	//Get set parameters from command line (parse_command_line), then request any additional info (check_information)
	//Must be called in that order
	parse_command_line(argc, argv);
	check_job_info();

	printer_print(NULL, driver, job_name, description, data);

	free_pointers();
	
	//test_print_list();
}

/// \brief Make sure all required fields are set for the printer_print function
static void check_job_info() {
	
	char * line = NULL;
	size_t n = 0;

	//Check that a driver is assigned
	if(driver == NULL || (strcmp(driver, "") == 0)) {
		printf("Please enter a driver\n");
		while(driver == NULL || (strcmp(driver, "") == 0) || (strcmp(driver, "\n") == 0)) {
			if(getline(&line, &n, stdin) > 0) {
				driver = malloc(n);
				strcpy(driver, line);
				strtok(driver, "\n");
				printf("Driver -- %s\n", driver);
			}
		}
	}

	//Check that a job_name is assigned
	if(job_name == NULL || (strcmp(job_name, "") == 0)) {
		
		char* tmp = remove_ext(data, '.', '/');
		job_name = malloc(sizeof(tmp));
		strcpy(job_name, tmp);
		if(tmp)
			free(tmp);
	}

	//Check that a description is assigned
	if(description == NULL || (strcmp(description, "") == 0)) {
		description = malloc(sizeof(""));
		strcpy(description, "");
		printf("Description -- %s\n", description);
	}
}

/**
 * \brief Free all of the global pointers used in cli-printer
 */
static void free_pointers() {
	if(driver)
		free(driver);
	if(job_name)
		free(job_name);
	if(description)
		free(description);
	if(data)
		free(data);
}

/**
 * Parse the command line arguments and set the appropriate flags and variables
 * 
 */
static void parse_command_line(int argc, char * argv[])
{
	int c;
	int i;
	int file_index;
	int option_index = 0;

	static struct option long_options[] = 
	{
		{"driver", required_argument, NULL, 'd'},
		{"output", required_argument, NULL, 'o'},
		{"description", required_argument, NULL, 's'},
		{"list", no_argument, NULL, 'l'},
		{"version", no_argument, NULL, 'v'},
		{"usage", no_argument, NULL, 'u'},
		{"help", no_argument, NULL, '?'}
	};

	if(argc < 2) {
		fprintf(stdout, "Usage: %s [options]\n", argv[0]);
		exit(0);
	}

	if( access( argv[argc-1], F_OK ) == -1 ) {
		//File doesn't exist for reading
		printf("File doesn't exist or isn't readable\n");
		fprintf(stdout, "Usage: %s [options]\n", argv[0]);
		free_pointers();
		exit(0);
	}
	data = malloc(sizeof(argv[argc-1]));
	strcpy(data, argv[argc-1]);
	printf("Data: %s\n", data);

	while((c = getopt_long(argc, argv, "d:o:s:lvu?", long_options, &option_index)) != -1)
	{
		
		switch(c)
		{
			case 'd': 
				// If driver is already set, then set the description
				if(!driver) {
					driver = malloc(sizeof(char)*(strlen(optarg)+1));
					printf("Driver: %s\n", optarg);
					strcpy(driver, optarg);
				} else {
					description = malloc(sizeof(char)*(strlen(optarg)+1));
					printf("Description: %s\n", optarg);
					strcpy(description, optarg);
				}
				break;
			case 'o': // output file
				job_name = malloc(sizeof(char)*(strlen(optarg)+1));
				printf("Output: %s\n", optarg);
				strcpy(job_name, optarg);
				break;
			case 's': // set description
				description = malloc(sizeof(char)*(strlen(optarg)+1));
				printf("Description: %s\n", optarg);
				strcpy(description, optarg);
				break;
			case 'v': // print version
				printf("Version 1.0\n");
				free_pointers();
				exit(0);
				break;
			case 'u': // print usage
				fprintf(stdout, "Usage: %s [options]\n", argv[0]);
				break;
			case '?': // print help information
				printf("%s is a program to send files to a printer\n", argv[0]);
				free_pointers();
				exit(0);
				break;
			case 'l': //Prints out list information and quits
				n = 0;
				list = printer_list_drivers(&n);
				
				for(i = 0; i < n; i++) {
					printf("printer_name=%s\n", list[i]->printer_name);
				}

				free_pointers();
				exit(0);
				break;
		}
	}
}

char *remove_ext (char* mystr, char dot, char sep) {
	char *retstr, *lastdot, *lastsep;

	// Error checks and allocate string.

	if (mystr == NULL)
		return NULL;
	if ((retstr = malloc (strlen (mystr) + 1)) == NULL)
		return NULL;

	// Make a copy and find the relevant characters.

	strcpy (retstr, mystr);
	lastdot = strrchr (retstr, dot);
	lastsep = (sep == 0) ? NULL : strrchr (retstr, sep);

	// If it has an extension separator.
	if (lastdot != NULL) {

		// and it's before the extenstion separator.
		if (lastsep != NULL) {
			if (lastsep < lastdot) {
			// then remove it.
				*lastdot = '\0';
			}
		} else {
		// Has extension separator with no path separator.
			*lastdot = '\0';
		}
	}

	// Return the modified string.
	return retstr;
}

