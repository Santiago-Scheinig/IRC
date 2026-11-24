#ifndef PARSER_HPP
# define PARSER_HPP

# include "ClientException.hpp"
# include "Message.hpp"

# include <cctype>
# include <string>
# include <vector>

class Parser 
{
	public:

		static Message 		processInput(const std::string &line);

	private:

		static Message 		_parseLine(const std::string &line);
		static void 		_readPrefix(const std::string &line, std::size_t &pos, std::string &prefix);
		static void 		_readCommand(const std::string &line, std::size_t &pos, std::string &cmd);
		static void 		_readParams(const std::string &line, std::size_t &pos, std::vector<std::string> &params, std::string &trailing, bool &hasTrailing);
		static void 		_skipSpaces(const std::string &line, std::size_t &pos);
		static std::string	_toUpper(const std::string &text);

};

#endif