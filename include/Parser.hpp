#ifndef PARSER_HPP
# define PARSER_HPP

#include "Message.hpp"
#include <string>
#include <vector>

class Parser
{
	public:
		enum Parse_e			//e_num creado para identifiar cada salida de parser
		{
			PARSE_INCOMPLETE,
			PARSE_MESSAGE,
			PARSE_INVALID
		};

		Parser();
		~Parser();

		Parse_e processInput(const std::string &data, Message &message);

	private:
		Parser(const Parser &other);
		Parser &operator=(const Parser &other);

		std::string _inputBuffer;

		Parse_e _parseLine(const std::string &line, Message &message) const;
		Parse_e _readPrefix(const std::string &line, std::size_t &pos, std::string &prefix) const;
		Parse_e _readCommand(const std::string &line, std::size_t &pos, std::string &cmd) const;
		Parse_e _readParams(const std::string &line, std::size_t &pos, std::vector<std::string> &params, std::string &trailing, bool &hasTrailing) const;
		void _skipSpaces(const std::string &line, std::size_t &pos) const;
		std::string _toUpper(const std::string &text) const;

};

#endif