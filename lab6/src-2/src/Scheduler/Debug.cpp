/*
 * Debug.cpp
 *
 *  Created on: Mar 12, 2016
 *      Author: vens
 */

#include <iostream>

#include "Debug.h"

std::ofstream* Debug::log_stream = 0;
bool Debug::verbose = false;

void Debug::SetLogFile(std::ofstream* stream)
{
	log_stream = stream;
}

void Debug::SetVerbose(bool value)
{
	verbose = value;
}

void Debug::AppendLog(std::string str)
{
	if(log_stream)
		*log_stream << str << std::endl;
	PrintMsg(str);
}

void Debug::PrintMsg(std::string str)
{
	if(verbose)
		std::cout << str << std::endl;
}
