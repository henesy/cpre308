
#include "signal.hpp"

#include <string>
#include <vector>

#include "wavedrom.hpp"


namespace wavedrom
{


Signal::Signal(const char * name)
{
	this->name = name;
	this->waveform = "";
}

Signal::~Signal()
{

}

void Signal::AddNode(enum NODE::node_type type)
{
	this->waveform += (char)type;
}

void Signal::AddNode(enum NODE::node_data_type type, const char * name)
{
	this->waveform += (char) type;
	this->data.emplace_back(name);
}

void Signal::ContinueNode(int n)
{
	for(int i=0; i<n; i++)
	{
		this->waveform += '.';
	}
}

json_value * Signal::ToJson()
{
	json_value * sig_obj = json_object_new(0);
	json_value * data = json_array_new(0);
	json_object_push(sig_obj, "name", json_string_new(this->name.c_str()));
	json_object_push(sig_obj, "wave", json_string_new(this->waveform.c_str()));
	for (std::vector<std::string>::iterator it = this->data.begin() ; it != this->data.end(); ++it)
    {
    	json_array_push(data, json_string_new(it->c_str()));
    }
    json_object_push(sig_obj, "data", data);
    return sig_obj;
}

}

