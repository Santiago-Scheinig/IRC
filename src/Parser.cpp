#include "../include/Parser.hpp"

#include <cctype>

Parser::Parser() : _inputBuffer()
{
}

Parser::~Parser()
{
}

Parser::Parse_e Parser::processInput(const std::string &input, Message &message)
{
	std::size_t	end;
	std::string	line;

	_inputBuffer += input;				// haya o no haya input rabajamos en inputbuffer
	end = _inputBuffer.find("\r\n");
	// Sin CRLF aun no hay linea completa: conservamos el buffer.
	if (end == std::string::npos)
		return PARSE_INCOMPLETE;
	line = _inputBuffer.substr(0, end);
	_inputBuffer.erase(0, end + 2);
	// La linea existe, pero no contiene ningun comando que parsear.
	if (line.empty())
		return PARSE_INVALID;
	return _parseLine(line, message);		// devolvemos true y message struct.
}

void Parser::_skipSpaces(const std::string &line, std::size_t &pos) const
{
	while (pos < line.size() && line[pos] == ' ')
		pos++;
}

Parser::Parse_e Parser::_readPrefix(const std::string &line, std::size_t &pos, std::string &prefix) const
{
	std::size_t	space;

	if (pos >= line.size() || line[pos] != ':')
		return PARSE_MESSAGE;
	space = line.find(' ', pos);
	if (space == std::string::npos || space == pos + 1) //prefix debe ser :prefix
		return PARSE_INVALID; //std::error prefijo invalido
	prefix = line.substr(pos + 1, space - pos - 1);
	pos = space + 1;									// actualizamos pos al inicio de sigiente palabra
	return PARSE_MESSAGE;
}

Parser::Parse_e Parser::_readCommand(const std::string &line, std::size_t &pos, std::string &cmd) const
{
	std::size_t	space;

	_skipSpaces(line, pos);
	if (pos >= line.size() || line[pos] == ':')
		return PARSE_INVALID; // despues del prefijo debe existir un comando.
	space = line.find(' ', pos);
	if (space == std::string::npos)
		cmd = line.substr(pos);
	else
	{
		cmd = line.substr(pos, space - pos);
		pos = space + 1;
	}
	if (cmd.empty())
		return PARSE_INVALID; // no podemos construir un Message sin comando.
	cmd = _toUpper(cmd);
	return PARSE_MESSAGE;
}

Parser::Parse_e Parser::_readParams(const std::string &line, std::size_t &pos, std::vector<std::string> &params, std::string &trailing, bool &hasTrailing) const
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
			return PARSE_MESSAGE; // ':' consume todo el resto como trailing.
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
	return PARSE_MESSAGE; // cero parametros tambien puede ser sintacticamente valido.
}

Parser::Parse_e Parser::_parseLine(const std::string &line, Message &message) const
{
	std::size_t	pos = 0;		//en cada funcion actualizamos pos para mantener el index en line actualizado correctamente
	std::string	prefix;
	std::string	command;
	std::vector<std::string> params;
	std::string	trailing;
	bool		hasTrailing = false;

	if (line.empty())
		return PARSE_INVALID; // CRLF sin contenido: linea invalida.
	if (_readPrefix(line, pos, prefix) == PARSE_INVALID)
		return PARSE_INVALID;
	if (_readCommand(line, pos, command) == PARSE_INVALID)
		return PARSE_INVALID;
	if (_readParams(line, pos, params, trailing, hasTrailing) == PARSE_INVALID)
		return PARSE_INVALID;
	message = Message(prefix, command, params, trailing, hasTrailing);
	return PARSE_MESSAGE;
}

std::string Parser::_toUpper(const std::string &cmd) const
{
	std::string tmp = cmd;

	for (std::size_t i = 0; i < tmp.size(); ++i)
		tmp[i] = static_cast<char>(std::toupper(static_cast<unsigned char>(tmp[i])));
	return tmp;
}
