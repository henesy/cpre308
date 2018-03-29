/*
 * Simulator.h
 *
 *  Created on: Mar 10, 2016
 *      Author: vens
 */

#ifndef SIMULATOR_H_
#define SIMULATOR_H_

#include <fstream>
#include <list>
#include "libjson/json.h"
#include "libwavedrom/wavedrom.hpp"

class SimTask;

/**
 * This simulator handles generating and sending simulated tasks to the scheduler.
 */
class Simulator {
public:
	/// Create a new simulator
	Simulator();
	virtual ~Simulator();

	/// Convert a JSON array into a new task list to simulate
	void MakeTaskList(json_value * array);

	/// return true if all the tasks in the task list are finished
	bool IsDone();

	/// run one system tick on the scheduler
	void RunTick();

	void PrintLogData();

	/// export the json waveform file
	void ExportWaveform(std::ofstream& out, bool include_legend = true);

private:
	std::list<SimTask*> sim_task_list;
	std::list<SimTask*> finished_list;
	SimTask* running_task;

	unsigned int num_tasks;
	unsigned int remaining_tasks;

	unsigned long sys_time;

	wavedrom::Wavedrom wave_root;
	wavedrom::Signal * wave_running;
};

#endif /* SIMULATOR_H_ */
