/*
 * SimTask.h
 *
 *  Created on: Mar 9, 2016
 *      Author: vens
 */

#ifndef SIMTASK_H_
#define SIMTASK_H_

#include <string>

#include "Task/Task.h"
#include "libjson/json.h"
#include "libwavedrom/wavedrom.hpp"
#include "Task/Context.h"




class SimTask : public Task {
public:

	enum State
	{
		INIT,
		NOT_ARRIVED,
		READY,
		RUNNING,
		BLOCKED,
		FINISHED
	};

	SimTask(json_value * task, wavedrom::Group * wave_grp);
	virtual ~SimTask();

	unsigned long GetArrivalTime();

	bool operator < (const SimTask&);

	bool IsFinished();
	bool IsRunning();
	std::string& GetName();
	int GetPriority() {return this->priority;};

	// From Context
	/// This is called by the scheduler when this task will be given time on the processor
	virtual void SwapIn(unsigned long sys_time);
	/// This is called by the scheduler when this task is being taken out of the processor
	virtual void SwapOut(unsigned long sys_time);

	/// called on this task at the beginning of every system tick
	void OnStartTick(unsigned long sys_time);
	/// called on this task during every system tick
	void OnSysTick(unsigned long sys_time);
	/// called on this task at the end of every system tick
	void OnEndTick(unsigned long sys_time);

	std::string GetLogString();

private:
	int priority;
	unsigned long next_arrival_time;
	unsigned long deadline;
	unsigned long times[20];
	int times_index;
	int num_runs;
	Task * task;
	std::string name;

	enum State state;
	enum State last_state;

	bool remove_flag;

	wavedrom::Signal* wave;
	wavedrom::NODE::node_data_type color;

	unsigned long finish_time;
	int num_context_switches;
	int num_bursts;
};


#endif /* SIMTASK_H_ */
