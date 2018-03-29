/*
 * main.cpp
 *
 *  Created on: Mar 9, 2016
 *      Author: vens
 */

#include <iostream>
#include <fstream>
#include <string.h>
#include <sstream>

#include <argp.h>

#include "Simulator.h"
#include "libmodule/Module.h"
#include "libjson/json.h"

#include "Debug.h"



using namespace std;


const char *argp_program_version = "Scheduling Simulator 1.0.0";
const char *argp_program_bug_address = "https://github.com/CprE308/lab-08/issues";
static char doc[] = "Scheduling Simulator for CprE 308";
static char args_doc[] = "";

static struct argp_option options[] = {
		{"verbose", 'v', 0, 0, "Produce verbose output"},
		{"algorithm", 'a', "alg.mod", 0, "The algorithm to simulate"},
		{"tasks", 't', "list.json", 0, "The task list workload to simulate"},
		{"wave", 'w', "wave.json", 0, "The name of the output waveform file"},
		{"log", 'l', "out.log", 0, "The name of the output log file"},
		{"config", 'c', "FILE", 0, "Config file to use"},
		{ 0 }
};

static struct arguments
{
	bool verbose;
	std::string algorithm;
	std::string tasks;
	std::string wave;
	std::string log;
	std::string cfg;
}args;

static void args_default(struct arguments *args)
{
	args->verbose = false;
	args->algorithm = std::string("alg.mod");
	args->tasks = std::string("tasks.json");
	args->wave = std::string("wave.json");
	args->log = std::string("out.log");
}

static void config_parse(std::string& file, struct arguments* args)
{
	std::ifstream cfg(file);
	if(cfg.is_open())
	{
		std::string line;
		while(std::getline(cfg, line))
		{
			std::istringstream is_line(line);
			std::string key;
			if(std::getline(is_line, key, '='))
			{
				std::string value;
				if(std::getline(is_line, value))
				{
					//std::cout << "key=" << key << ", value=" << value << std::endl;
					if(key == "algorithm")
						args->algorithm = value;
					else if(key == "tasks")
						args->tasks = value;
					else if(key == "wave")
						args->wave = value;
					else if(key == "log")
						args->log = value;
				}
			}
		}
	}
	else
	{
		std::cerr << "Failed to open config file " << file << std::endl;
	}
}

static error_t parse_opt (int key, char *arg, struct argp_state * state)
{
	struct arguments * arguments = (struct arguments*)state->input;
	switch (key)
	{
	case 'v':
		//std::cout << "setting verbose flag" << std::endl;
		arguments->verbose = true;
		break;
	case 'a':
		arguments->algorithm = std::string(arg);
		break;
	case 't':
		arguments->tasks = std::string(arg);
		break;
	case 'w':
		arguments->wave = std::string(arg);
		break;
	case 'l':
		arguments->log = std::string(arg);
		break;
	case 'c':
		arguments->cfg = std::string(arg);
		break;
	case ARGP_KEY_ARG:
		// too many arguments
		break;
	case ARGP_KEY_END:
		break;
	default:
		return ARGP_ERR_UNKNOWN;
	}
	return 0;
}

static struct argp argp = {options, parse_opt, args_doc, doc };

int main(int argc, char * argv[])
{
	Simulator simulator;

	args_default(&args);
	argp_parse(&argp, argc, argv, 0, 0, &args);
	if(args.cfg.size() > 0)
	{
		config_parse(args.cfg, &args);
	}

	Debug::SetVerbose(args.verbose);

	//std::string alg = std::string("Algorithm = ") + args.algorithm;
	Debug::PrintMsg(std::string("Algorithm = ") + args.algorithm);
	Debug::PrintMsg(std::string("Tasks = ") + args.tasks);
	Debug::PrintMsg(std::string("Wave = ") + args.wave);
	Debug::PrintMsg(std::string("Log = ") + args.log);

	ofstream log_stream(args.log);
	Debug::SetLogFile(&log_stream);

	// install the scheduler module
	module::Module* mod = new module::Module(args.algorithm);

	// open the task_list json file
	ifstream task_list_file(args.tasks);
	task_list_file.seekg(0, task_list_file.end);
	int length = task_list_file.tellg();
	task_list_file.seekg(0, task_list_file.beg);
	char * buf = new char [length];
	task_list_file.read(buf, length);
	task_list_file.close();

	// parse the task_list_file
	json_value * task_list = json_parse(buf, length);

	delete[] buf;

	// add the task list to the simulator
	simulator.MakeTaskList(task_list->u.object.values[0].value);

	json_value_free(task_list);

	// run the simulator
	while(!simulator.IsDone())
	{
		simulator.RunTick();
	}
	simulator.RunTick(); // run one last tick to close all the task ends.

	simulator.PrintLogData();

	// create the output file
	ofstream out(args.wave);
	simulator.ExportWaveform(out);
	out.close();

	mod->Remove();
	delete mod;

	return 0;
}

