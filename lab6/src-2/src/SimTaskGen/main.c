
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "sim_task.h"

int interactive = 0;
unsigned long minimum = 1;
unsigned long maximum = 5;
int num_tasks = 26;
int deadline = 120;
int seed = 5;

/**
 * Recognized arguments:
 * - `-m`: the minimum amount of time that a task will run
 * - `-M`: the maximum amount of time that a task will run
 * - `-n`: number of tasks to generate
 * - `-s`: sets the seed
 * - `--interactive': interactive workload
 * - `--batch`: batch workload
 * - `--strict`: strict deadline for the tasks(+ 10%)[default is 20%]
 * - `--loose`: loose deadline for the tasks(+ 30%)[default is 20%]
 * - `--output`: set the name of the output file [efault is `out.json`]
 */
struct option long_options[] = {
	// these options set a flag
	{"interactive", no_argument, &interactive, 1},
	{"batch", no_argument, &interactive, 0},
	{"strict", no_argument, &deadline, 110},
	{"loose", no_argument, &deadline, 130},
	// these options do not set a flag
	{"min", required_argument, 0, 'm'},
	{"max", required_argument, 0, 'M'},
	{"num-tasks", required_argument, 0, 'n'},
	{"seed", required_argument, 0, 's'},
	{"output", required_argument, 0, 'o'},
	{0, 0, 0, 0}
};

int main(int argc, char * argv[])
{
	int c;
	int opt_idx;
	int i;
	int j;
	int last_arrive_time;
	int times_num;
	struct sim_task *task = NULL;
	int run_time;
	char * out_name = "out.json";

	c = getopt_long(argc, argv, "m:M:n:s:?", long_options, &opt_idx);
	while(c != -1){
		switch(c){
			case 0: if(long_options[opt_idx].flag != 0){
					printf("Option %s was called, setting value to %d\n", long_options[opt_idx].name, long_options[opt_idx].val);
					break;
				}
				if(optarg){
					
				}
				break;
			case 'm': // set the minumum time
				minimum = atol(optarg);
				printf("Minimum run time set to: %lu\n", minimum);
				break;
			case 'M': // set the maximum time
				maximum = atol(optarg);
				printf("Maximum run time set to: %lu\n", maximum);
				break;
			case 'n': // set the number of tasks to create
				num_tasks = atoi(optarg);
				printf("Number of tasks to generate: %d\n", num_tasks);
				break;
			case 's': // set the seed for the random number generator
				seed = atoi(optarg);
				printf("New seed set to: %d\n", seed);
				break;
			case 'o': // set the name of the output file
				out_name = optarg;
				printf("Output file set to %s\n", out_name);
				break;
		}
		c = getopt_long(argc, argv, "m:M:n:s:?", long_options, &opt_idx);
	}

	// set the seed for the random number generator
	srand(seed);

	// initialize the json task list
	init_json_tasklist();

	// populate the task list
	for(i = 0; i < num_tasks; i++){
		// first allocate memory for the task
		task = calloc(1, sizeof(struct sim_task));
		printf("generated a task\n");

		// name the task, times that the task will be running/blocking, and set the deadline
		if(i < 26){
			task->name[0] = 'A' + i;
		}
		printf("\tName: %s\n", task->name);

		// give it priority
		task->priority = (unsigned char)(rand() % 4);
		printf("\tPriority: %d\n", task->priority);

		// set arrival time
		if(i == 0){
			task->arrive_time = (unsigned long)(rand() % 5);
		}else{
			task->arrive_time = last_arrive_time + (rand() % 15);
		}
		last_arrive_time = task->arrive_time;
		printf("\tArrival time: %lu\n", task->arrive_time);

		// set up times for the task to run/block
		times_num = (rand() + 1) % 19;
		if(0 == times_num % 2){
			times_num--;
		}
		printf("\ttimes_num: %d\n", times_num);
		run_time = 0;
		for(j = 0; j < times_num; j++){
			if(interactive){
				if(j % 2){
					task->times[j] = (rand() + minimum) % ((4 - task->priority) * maximum);
					while(task->times[j] == 0){
						task->times[j] = (rand() + minimum) % ((4 - task->priority) * maximum);
					}
				}else{
					task->times[j] = (rand() + minimum) % (2 * maximum);
					while(task->times[j] == 0){
						task->times[j] = (rand() + minimum) % (2 * maximum);
					}
				}
			}else{
				if(j % 2){
					task->times[j] = (rand() + (2 * minimum)) % ((6 - task->priority) * maximum);
					while(task->times[j] == 0){
						task->times[j] = (rand() + (2 * minimum)) % ((6 - task->priority) * maximum);
					}
				}else{
					task->times[j] = (rand() + minimum) % maximum;
					while(task->times[j] == 0){
						task->times[j] = (rand() + minimum) % maximum;
					}
				}
			}
			run_time += task->times[j];
			printf("\t\ttimes[%d]: %lu\n", j, task->times[j]); 
		}
		task->times[j] = 0;

		// set the deadline
		task->deadline = (unsigned long)((run_time * deadline)/100) + task->arrive_time;
		printf("\tDeadline: %lu\n", task->deadline);
		
		// add this task to the json
		add_sim_task_to_json(task);
		// "let go" of the task
		task = NULL;
	}

	FILE* fp = fopen(out_name, "w");
	export_json_tasklist(fp);
	fclose(fp);
	return 0;
}
