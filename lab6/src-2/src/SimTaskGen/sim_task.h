


/**
 * I was thinking that the generator should create 26 tasks named A through Z.  This
 * should make the wavedrom managable and readable.  The tasks do not need to be sorted
 * in any order when they are converted to json, the simulator takes care of that.
 */

struct sim_task
{
	char name[128]; // name of the task
	unsigned char priority; // priority of the task.  0=lowest, 4=highest
	unsigned long arrive_time; // the time the task will first arrive
	unsigned long times[20]; // The times that this task will run/block.  This array must contain 1 or more values each > 0, and must contain an odd number of values.  The even indexes (0, 2, 4, ...) contain the run times (the amount of time the task will run before it blocks), and the odd indexes (1, 3, ...) is the block times (the about of time the task will block for IO each time it needs to block).  The task must end by running, it can not end on a block.  After the very last run time the array should be terminated by a `0`.
	unsigned long deadline; // the system time that the task should be finished by.  This number must be greater than arrive_time + each value in times[] array, and should take into account the priority of the task.
};

void init_json_tasklist(void);
void add_sim_task_to_json(struct sim_task * task);
void export_json_tasklist(FILE* fp);