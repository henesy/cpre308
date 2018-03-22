/**
 * @file      main.c
 * @author    Jeramie Vens
 * @date      2015-02-11: Created
 * @date      2015-02-15: Last updated
 * @date      2016-02-16: Complete re-write
 * @date      2016-02-20: convert to single threaded
 * @brief     Emulate a print server system
 * @copyright MIT License (c) 2015, 2016
 */
 
/*
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <semaphore.h>


#include "print_job.h"
#include "printer_driver.h"
#include "debug.h"

// -- GLOBAL VARIABLES -- //
int verbose_flag = 0;
int exit_flag = 0;

// -- STATIC VARIABLES -- //
static struct printer_group * printer_group_head;

// -- FUNCTION PROTOTYPES -- //
static void parse_command_line(int argc, char * argv[]);
static void parse_rc_file(FILE* fp);


/**
 * A list of print jobs that must be kept thread safe
 */
struct print_job_list
{
	// the head of the list
	struct print_job * head;
	// the number of jobs in the list
	sem_t num_jobs;
	// a lock for the list
	pthread_mutex_t lock;
};

/**
 * A printer object with associated thread
 */
struct printer
{
	// the next printer in the group
	struct printer * next;
	// the driver for this printer
	struct printer_driver driver;
	// the list of jobs this printer can pull from
	struct print_job_list * job_queue;
	// the thread id for this printer thread
	pthread_t tid;
};

/**
 * A printer group.  A group represents a collection of printers that can pull
 * from the same job queue.
 */
struct printer_group
{
	// the next group in the system
	struct printer_group * next_group;
	// the name of this group
	char * name;
	// the list of printers in this group
	struct printer * printer_queue;
	// the list of jobs for this group
	struct print_job_list job_queue;
};

int main(int argc, char* argv[])
{
	int produce = 1;
	int n_jobs = 0;
	struct printer_group * g;
	struct printer * p;
	struct print_job * job;
	struct print_job * prev = NULL;
	char * line = NULL;
	size_t n = 0;
	long long job_number = 0;

	// parse the command line arguments
	parse_command_line(argc, argv);

	// open the runtime config file
	FILE* config = fopen("config.rc", "r");
	// parse the config file
	parse_rc_file(config);
	// close the config file
	fclose(config);

	// order of opperation:
	// 1. while the exit flag has not been set
	// 2. read from standard in
	// 3. do the necessary checks from standard in
	// 4. if PRINT is received, go through each printer group, and check
	//    to see if the group name matches what is specified by the job
	// 5. Once a match is made, it puts the job in the job queue of the
	//    correct printer group
	// 6. then loop through the list of printer groups to get the print
	//    job and send it to the printers
	for(g = printer_group_head; g; g = g->next_group)
	{
		sem_init(&g->job_queue.num_jobs, 0, 0);
	}
	
	while(!exit_flag)
	{
		if(produce){
			getline(&line, &n, stdin);
			if(strncmp(line, "NEW", 3) == 0)
			{
				job = calloc(1, sizeof(struct print_job));
				job->job_number = job_number++;
			}
			else if(job && strncmp(line, "FILE", 4) == 0)
			{
				strtok(line, ": ");
				job->file_name = malloc(n);
				strncpy(job->file_name, strtok(NULL, "\n"), n);
			}
			else if(job && strncmp(line, "NAME", 4) == 0)
			{
				strtok(line, ": ");
				job->job_name = malloc(n);
				strncpy(job->job_name, strtok(NULL, "\n"), n);
			}
			else if(job && strncmp(line, "DESCRIPTION", 11) == 0)
			{
				strtok(line, ": ");
				job->description = malloc(n);
				strncpy(job->description, strtok(NULL, "\n"), n);
			}
			else if(job && strncmp(line, "PRINTER", 7) == 0)
			{
				strtok(line, ": ");
				job->group_name = malloc(n);	
				strncpy(job->group_name, strtok(NULL, "\n"), n);
			}
			else if(job && strncmp(line, "PRINT", 5) == 0)
			{
				if(!job->group_name)
				{
					eprintf("Trying to print without setting printer\n");
					continue;
				}
				if(!job->file_name)
				{
					eprintf("Trying to print without providing input file\n");
					continue;
				}
				for(g = printer_group_head; g; g=g->next_group)
				{
					if(strcmp(job->group_name, g->name) == 0)
					{
 						job->next_job = g->job_queue.head;
 						g->job_queue.head = job;
 						sem_post(&g->job_queue.num_jobs);
						//
						
						job = NULL;
						produce = 0;
						break;
					}
				}
				if(job)
				{
					eprintf("Invalid printer group name given: %s\n", job->group_name);
					continue;
				}
			}
			else if(strncmp(line, "EXIT", 4) == 0)
			{
				exit_flag = 1;
			}
		}else{
			for(g = printer_group_head; g; g = g->next_group){
				for(p = g->printer_queue; p; p = p->next){
					if(sem_getvalue(&p->job_queue->num_jobs, &n_jobs)){
						perror("sem_getvalue");
						abort();
					}

					if(n_jobs){
						// wait for an item to be in the list
	 					sem_wait(&p->job_queue->num_jobs);
	 					// walk the list to the end
	 					for(job = p->job_queue->head; job->next_job; prev = job, job = job->next_job);
	 					if(prev)		
	 						// fix the tail of the list
 							prev->next_job = NULL;
 						else
							// There is only one item in the list
							p->job_queue->head = NULL;
 			
	 					printf("consumed job %s\n", job->job_name);
 		
 						// send the job to the printer
						printer_print(&p->driver, job);
						produce = 1;
					}
				}
			}
		}
	}

	return 0;
}

/**
 * Parse the command line arguments and set the appropriate flags and variables
 * 
 * Recognized arguments:
 *   - `-v`: Turn on Verbose mode
 *   - `-?`: Print help information
 */
static void parse_command_line(int argc, char * argv[])
{
	int c;
	while((c = getopt(argc, argv, "v?")) != -1)
	{
		switch(c)
		{
			case 'v': // turn on verbose mode
				verbose_flag = 1;
				break;
			case '?': // print help information
				fprintf(stdout, "Usage: %s [options]\n", argv[0]);
				exit(0);
				break;
		}
	}
}

static void parse_rc_file(FILE* fp)
{
	char * line = NULL;
	char * ptr;
	size_t n = 0;
	struct printer_group * group = NULL;
	struct printer_group * g;
	struct printer * printer = NULL;
	struct printer * p;

	// get each line of text from the config file
	while(getline(&line, &n, fp) > 0)
	{
		// if the line is a comment
		if(line[0] == '#')
				continue;

		// If the line is defining a new printer group
		if(strncmp(line, "PRINTER_GROUP", 13) == 0)
		{
			strtok(line, " ");
			ptr = strtok(NULL, "\n");
			group = calloc(1, sizeof(struct printer_group));
			group->name = malloc(strlen(ptr)+1);
			strcpy(group->name, ptr);

			if(printer_group_head)
			{
				for(g = printer_group_head; g->next_group; g=g->next_group);
				g->next_group = group;
			}
			else
			{
				printer_group_head = group;
			}
		}
		// If the line is defining a new printer
		else if(strncmp(line, "PRINTER", 7) == 0)
		{
			strtok(line, " ");
			ptr = strtok(NULL, "\n");
			printer = calloc(1, sizeof(struct printer));
			printer_install(&printer->driver, ptr);
			printer->job_queue =  &(group->job_queue);
			if(group->printer_queue)
			{
				for(p = group->printer_queue; p->next; p = p->next);
				p->next = printer;
			}
			else
			{
					group->printer_queue = printer;

			}
		}
	}

	// print out the printer groups
	dprintf("\n--- Printers ---\n"); 
	for(g = printer_group_head; g; g = g->next_group)
	{
		dprintf("Printer Group %s\n", g->name);
		for(p = g->printer_queue; p; p = p->next)
		{
			dprintf("\tPrinter %s\n", p->driver.name);
		}
	}
	dprintf("----------------\n\n");

}

