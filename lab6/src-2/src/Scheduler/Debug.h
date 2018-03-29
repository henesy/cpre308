/*
 * Debug.h
 *
 *  Created on: Mar 12, 2016
 *      Author: vens
 */

#ifndef DEBUG_H_
#define DEBUG_H_

#include <fstream>
#include <string>

class Debug {

public:
	static void SetLogFile(std::ofstream* stream);
	static void SetVerbose(bool value = true);
	static void AppendLog(std::string str);
	static void PrintMsg(std::string str);


private:
	static std::ofstream* log_stream;
	static bool verbose;
};


#endif /* DEBUG_H_ */
