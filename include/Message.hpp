#ifndef MESSAGE_HPP
# define MESSAGE_HPP

#include <string>
#include <vector>

class Message
{
	private:
		std::string					_prefix;
		std::string					_cmd;
		std::vector<std::string>	_params;
		std::string					_trailing;
		bool						_hasTrailing;

	public:
		Message();
		Message(const std::string &prefix, const std::string &cmd, const std::vector<std::string> &params, const std::string &trailing, bool hasTrailing);
		~Message();

		const std::string				&getPrefix() const;
		const std::string				&getCommand() const;
		const std::vector<std::string>	&getParams() const;
		const std::string				&getTrailing() const;
		bool							hasTrailing() const;
};

#endif