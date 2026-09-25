#include "Config.hpp"
#include <cstdlib>
#include <stdexcept>

Config::Config(int argc, char** argv) : _port(0), _password("")
{
	if (argc != 3)
		throw std::invalid_argument("usage: ./ircserv <port> <password>");
	_port = std::atoi(argv[1]);
	if (_port <= 0 || _port > 65535)
		throw std::invalid_argument("port must be between 1 and 65535");
	_password = argv[2];
}

Config::~Config()
{
}

Config::Config(const Config& other) : _port(other._port), _password(other._password)
{
}

Config& Config::operator=(const Config& other)
{
	if (this != &other)
	{
		_port = other._port;
		_password = other._password;
	}
	return (*this);
}

int	Config::getPort() const
{
	return (_port);
}

const std::string&	Config::getPassword() const
{
	return (_password);
}
