#ifndef JSON_HPP
#define JSON_HPP

#include "../libjson/json.h"

namespace wavedrom
{

/**
 * @brief     An object that can be converted into a JSON representation.
 */
class Json
{
public:
	/**
	 * @brief      Default destructor
	 */
	virtual ~Json(){};
	/**
	 * @brief      Convert this object into its JSON representation
	 * @return     The JSON Value object
	 */
	virtual json_value* ToJson() = 0;
};

}

#endif
