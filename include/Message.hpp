#ifndef MESSAGE_HPP
# define MESSAGE_HPP

#include <string>
#include <vector>

class Message
{
	/**
	 * All of this private information could instead be a typedef (ex: msgInfo)
	 * This is practical, since we could group and divide pieces of information
	 * regarding how other objects interact with this.
	 * _prefix, _trailing and _hasTrailing could be a typedef (ex: msgInfo)
	 * _cmd and _params could be a typedef (ex: msgParams)
	 * Not necesarilly the case, yet, sometimes, its great to group data since it makes
	 * it easier to travel and identify.
	 */
	private:
		std::string					_prefix;
		std::string					_cmd;
		std::vector<std::string>	_params;
		std::string					_trailing;
		bool						_hasTrailing;

	public:
		Message();
		Message(const std::string &prefix, const std::string &cmd, const std::vector<std::string> &params, const std::string &trailing, bool hasTrailing);
	/**
	* We are missing the other OCF functions, its normative.
	*/
		~Message();

		const std::string				&getPrefix() const;
		const std::string				&getCommand() const;
		const std::vector<std::string>	&getParams() const;
		const std::string				&getTrailing() const;
		bool							hasTrailing() const;
};

#endif