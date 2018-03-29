
#ifndef SIGNAL_HPP
#define SIGNAL_HPP

#include "../libwavedrom/json.hpp"
#include "../libwavedrom/wavedrom.hpp"

namespace wavedrom
{

namespace NODE
{
	enum node_type
	{
		LOW = '0',
		HIGH = '1',
		Z = 'z',
		UP = 'u',
		DOWN = 'd',
		CONTINUE = '.',
		X = 'x',
		PCLK = 'p',
		PCLK_ARROW = 'P',
		NCLK = 'n',
		NCLK_ARROW = 'N',
		SPACER = '|'

	};

	enum node_data_type
	{
		WHITE = '2',
		YELLOW = '3',
		RED = '4',
		BLUE = '5'
	};
}

/**
 * @brief     A Wavedrom Signal lane object.
 */
class Signal : protected Json
{
	/// @brief     Allow Group to construct Signals
	friend class Group;

public:
	/**
	 * @brief      Add a new node to this signal.
	 * @param      type
	 *                  The type of node to add.
	 */
	void AddNode(enum NODE::node_type type);
	/**
	 * @brief      Add a new named node to this signal
	 * @param      type
	 *                  The type of node to add.
	 * @param      name
	 *                  The name of the node to add.
	 */
	void AddNode(enum NODE::node_data_type type, const char * name = "");
	/**
	 * @brief      Continue the previously created node n more times
	 * @param      n
	 *                  The number of steps to repeat the previous node
	 */
	void ContinueNode(int n = 1);

protected:
	/**
	 * @brief      Convert this Signal into its JSON representation
	 * @return     The JSON value.
	 */
	virtual json_value* ToJson();

private:
	/**
	 * @brief      Create a new Signal object
	 * @param      name
	 *                  The name of the new Signal
	 */
	Signal(const char * name);
	/**
	 * @brief      Destroy the signal
	 */
	~Signal();

	/// @brief     The name of this signal
	std::string name;
	/// @brief     The raw waveform string for the nodes of this signal
	std::string waveform;
	/// @brief     List of names for the named nodes of this signal
	std::vector<std::string> data;
};

}

#endif
