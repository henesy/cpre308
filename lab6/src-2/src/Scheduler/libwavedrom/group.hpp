/**
 * @file      group.hpp
 * @author    Jeramie Vens
 * @date      March 7, 2016: Created
 * @brief     A group wrapper that can contain nested groups and Signals.
 */
#ifndef GROUP_HPP
#define GROUP_HPP

#include <vector>
#include <string>

#include "json.hpp"

namespace wavedrom
{

class Signal;

/**
 * @brief     A group object which can contain nested Groups and Signals.
 */
class Group : protected Json
{
public:
	/**
	 * @brief      Create a new subgroup of this group.
	 * @param      name
	 *                  The name of the new subgroup.
	 * @return     The newly created group.
	 */
	Group* AddGroup(const char * name);
	/**
	 * @brief      Create a new signal inside this group.
	 * @param      name
	 *                  The name of the new signal.
	 * @return     The newly created signal.
	 */
	Signal* AddSignal(const char * name);


protected:
	/**
	 * @brief      Convert this group into its JSON value
	 * @return     The JSON value object.
	 */
	virtual json_value* ToJson();

	Group(const char * name);

	/**
	 * @brief      Destroy this group and all of its children
	 */
	virtual ~Group();

private:
	/**
	 * @brief      List of children of this group.
	 */
	std::vector<Json*> children;
	/**
	 * @brief      The name of this group.
	 */
	std::string name;
};





}

#endif
