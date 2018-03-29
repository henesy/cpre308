


#include "../libwavedrom/wavedrom.hpp"

#include <string>

namespace wavedrom
{


Wavedrom::Wavedrom()
	: Group("")
{

}

Wavedrom::~Wavedrom()
{
	//json_builder_free(root);
}

char * Wavedrom::Export()
{
	json_value* root = json_object_new(0);
	json_value* signals = this->ToJson();
	json_object_push(root, "signal", signals);
	char * buf = new char[json_measure(root)];
	json_serialize(buf, root);
	json_builder_free(root);
	return buf;
}

}
