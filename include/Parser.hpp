#ifndef PARSER_HPP
# define PARSER_HPP

#include <string>
#include "Message.hpp"

// Owner: track B (parsing).
// Stateless by design: parse() is a pure function of its input line.
// No Server/Client dependency here on purpose, so it stays unit-testable
// on its own and never needs to merge against networking changes.
class Parser
{
	public:
		static Message	parse(const std::string& rawLine);

	private:
		Parser();
		~Parser();
		Parser(const Parser& other);
		Parser& operator=(const Parser& other);
};

#endif
