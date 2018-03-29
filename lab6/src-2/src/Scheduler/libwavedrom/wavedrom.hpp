/**
 * @file      wavedrom.hpp
 * @author    Jeramie Vens
 * @date      March 7, 2016: Initial version
 * @brief     This is the main include for the Wavedrom C++ Library
 *
 */


#ifndef WAVEDROM_HPP
#define WAVEDROM_HPP

#include "../libwavedrom/group.hpp"
#include "../libwavedrom/signal.hpp"

/**
 * @brief     The Wavedrom Library encases all of its code in the wavedrom namespace.
 */
namespace wavedrom
{

/**
 * @brief     A wavedrom object.
 * @details   The wavedrom object is the main entry point of the wavedrom
 *            library.  It encapsulates all the supported features of
 *            Wavedrom.
 */
class Wavedrom : public Group
{
public:
	/**
	 * @brief      Create a new wavedrom object.
	 */
	Wavedrom();

	virtual ~Wavedrom();

	/**
	 * @brief      Export this wavedrom object to a Wavedrom JSON format
	 * @details    This will allocate a new c string containing a valid
	 *             Wavedrom JSON formmated descrption of this waveform.
	 *             The exported JSON description can be converted to an
	 *             image using http://wavedrom.com/editor.html
	 * @return     An allocated cstring that contains the JSON object.
	 */
	char * Export();
};

}

#endif
