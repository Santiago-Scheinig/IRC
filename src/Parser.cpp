#include "../include/Parser.hpp"
#include "../include/IRCException.hpp"

#include <cctype>

Message Parser::processInput(const std::string &line)
{
	if (line.empty())
		throw IRCException(PARSE_INVALID);
	return _parseLine(line);
}

void Parser::_skipSpaces(const std::string &line, std::size_t &pos)
{
	while (pos < line.size() && line[pos] == ' ')
		pos++;
}

void Parser::_readPrefix(const std::string &line, std::size_t &pos, std::string &prefix)
{
	std::size_t	space;

	if (pos >= line.size() || line[pos] != ':')
		return;
	space = line.find(' ', pos);
	if (space == std::string::npos || space == pos + 1) //prefix debe ser :prefix
		throw IRCException(PARSE_INVALID);
	prefix = line.substr(pos + 1, space - pos - 1);
	pos = space + 1;									// actualizamos pos al inicio de sigiente palabra
}

void Parser::_readCommand(const std::string &line, std::size_t &pos, std::string &cmd)
{
	std::size_t	space;

	_skipSpaces(line, pos);
	if (pos >= line.size() || line[pos] == ':')
		throw IRCException(PARSE_INVALID);
	space = line.find(' ', pos);
	if (space == std::string::npos)
		cmd = line.substr(pos);
	else
	{
		cmd = line.substr(pos, space - pos);
		pos = space + 1;
	}
	if (cmd.empty())
		throw IRCException(PARSE_INVALID);
	cmd = _toUpper(cmd);
}

void Parser::_readParams(const std::string &line, std::size_t &pos, std::vector<std::string> &params, std::string &trailing, bool &hasTrailing)
{
	std::size_t	space;

	while (pos < line.size())
	{
		_skipSpaces(line, pos);
		if (pos >= line.size())
			break;
		if (line[pos] == ':')
		{
			trailing = line.substr(pos + 1);
			hasTrailing = true;
			return;
		}
		space = line.find(' ', pos);
		if (space == std::string::npos)
		{
			params.push_back(line.substr(pos));
			break;
		}
		params.push_back(line.substr(pos, space - pos));
		pos = space + 1;
	}
}

Message Parser::_parseLine(const std::string &line)
{
	std::size_t	pos = 0;		//en cada funcion actualizamos pos para mantener el index en line actualizado correctamente
	std::string	prefix;
	std::string	command;
	std::vector<std::string> params;
	std::string	trailing;
	bool		hasTrailing = false;

	if (line.empty())
		throw IRCException(PARSE_INVALID);
	_readPrefix(line, pos, prefix);
	_readCommand(line, pos, command);
	_readParams(line, pos, params, trailing, hasTrailing);
	return Message(prefix, command, params, trailing, hasTrailing);
}

std::string Parser::_toUpper(const std::string &cmd)
{
	std::string tmp = cmd;

	for (std::size_t i = 0; i < tmp.size(); ++i)
		tmp[i] = static_cast<char>(std::toupper(static_cast<unsigned char>(tmp[i])));
	return tmp;
}
