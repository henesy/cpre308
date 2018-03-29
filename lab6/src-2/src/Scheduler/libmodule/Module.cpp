/*
 * module.cpp
 *
 *  Created on: Mar 8, 2016
 *      Author: vens
 */

#include "Module.h"

#include <string>
#include <iostream>

#include <dlfcn.h>


namespace module {

Module::Module(const std::string& name) {
	char * error;
	int rv;
	int (*init_function)();

	this->module_handle = dlopen(name.c_str(), RTLD_NOW);
	if(!this->module_handle){
		// TODO throw exception;
		std::cerr << std::string(dlerror()) << std::endl;
		return;
	}

	init_function = (int (*)())dlsym(this->module_handle, "init_module");
	if((error = dlerror()) != NULL){
		// TODO throw exception
		std::cerr << std::string(error) << std::endl;
		return;
	}

	this->module_name = name;

	rv = init_function();
	if(rv){
		// TODO throw exception
		std::cerr << "Init function failed, returned: " << rv << std::endl;
		return;
	}

	return;
}

void Module::Remove()
{
	char * error;
	void (*cleanup_function)();
	cleanup_function = (void (*)())dlsym(this->module_handle, "cleanup_module");
	if((error = dlerror()) != NULL){
		std::cerr << std::string(error) << std::endl;
		return;
	}

	cleanup_function();
}

Module::~Module() {
	dlclose(this->module_handle);
}

} /* namespace module */
