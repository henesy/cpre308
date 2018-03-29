
#include "group.hpp"

#include <string>
#include <vector>

#include "signal.hpp"
#include "wavedrom.hpp"


namespace wavedrom
{


Group::Group(const char * name)
{
	this->name = name;
}

Group::~Group()
{
	for(auto it=children.begin(); it != children.end(); ++it)
	{
		delete *it;
	}
}

Group* Group::AddGroup(const char * name)
{
	Group *g = new Group(name);
	children.push_back(g);
	return g;
}

Signal* Group::AddSignal(const char * name)
{
	Signal* s = new Signal(name);
	children.push_back(s);
	return s;
}

json_value* Group::ToJson()
{
	json_value * array = json_array_new(0);
	json_array_push(array, json_string_new(this->name.c_str()));
	for(std::vector<Json*>::iterator it = this->children.begin(); it != this->children.end(); ++it)
	{
		json_array_push(array, (*it)->ToJson());
	}
	return array;
}

}



