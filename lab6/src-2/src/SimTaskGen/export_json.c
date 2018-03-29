
#include <stdlib.h>
#include <stdio.h>

#include "libjson/json.h"
#include "sim_task.h"

json_value* list;

const json_serialize_opts json_opts =
{
	.mode = json_serialize_mode_multiline,
	.opts = 0,
	.indent_size = 2
};

void init_json_tasklist(void)
{
	list = json_array_new(0);
}

void add_sim_task_to_json(struct sim_task * task)
{
	json_value* t = json_object_new(0);
	json_value* rt = json_array_new(0);
	json_value* bt = json_array_new(0);
	int i;
	json_object_push(t, "name", json_string_new(task->name));
	json_object_push(t, "priority", json_integer_new(task->priority));
	json_object_push(t, "arrive-time", json_integer_new(task->arrive_time));
	json_object_push(t, "deadline", json_integer_new(task->deadline));
	json_array_push(rt, json_integer_new(task->times[0]));
	for(i=1;i<19;i+=2)
	{
		if(task->times[i] && task->times[i+1])
		{
			json_array_push(bt, json_integer_new(task->times[i]));
			json_array_push(rt, json_integer_new(task->times[i+1]));
		}
		else
		{
			break;
		}
	}
	json_object_push(t, "run-times", rt);
	json_object_push(t, "block-times", bt);
	json_array_push(list, t);
}

void export_json_tasklist(FILE* fp)
{
	json_value * root = json_object_new(0);
	json_object_push(root, "tasks", list);
	size_t len = json_measure_ex(root, json_opts);
	char * buf = malloc(len);
	json_serialize_ex(buf, root, json_opts);
	fprintf(fp, "%s\n", buf);
	//fwrite(buf, len - 1, 1, fp);
}
