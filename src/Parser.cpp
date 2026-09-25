#include "Parser.hpp"

Parser::Parser()
{
}

Parser::~Parser()
{
}

Parser::Parser(const Parser& other)
{
	(void)other;
}

Parser& Parser::operator=(const Parser& other)
{
	(void)other;
	return (*this);
}

// TODO(track B): implement IRC line grammar.
// [':' prefix SPACE] command [params] [':' trailing] CRLF
Message	Parser::parse(const std::string& rawLine)
{
	Message	message;

	(void)rawLine;
	return (message);
}
