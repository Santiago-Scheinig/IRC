#include "../include/Config.hpp"

#include <cctype>
#include <csignal>
#include <iostream>
#include <sstream>
#include <stdexcept>

volatile sig_atomic_t g_running = true;

static void signalHandler(int signal)
{
	(void)signal;
	g_running = false;
}

Config::Config(int argc, char **argv) : _port(0), _passwd()
{
	if (argc != 3)
		throw std::invalid_argument("usage: ./ircserv <port> <password>");
	if (!_parsePort(argv[1], _port) || !_parsePasswd(argv[2]))
		throw std::invalid_argument("invalid port or password");
	_passwd = argv[2];
}

Config::~Config(){}

void Config::installSignalHandlers()
{
	signal(SIGINT, signalHandler);
	signal(SIGQUIT, signalHandler);
	signal(SIGPIPE, signalHandler);
}

bool Config::_parsePort(const std::string &s, int &port)
{
	if (s.empty() || s.size() > 5)
		return false;
	for (std::size_t i = 0; i < s.size(); ++i)
	{
		if (!std::isdigit(static_cast<unsigned char>(s[i])))
			return false;
	}
	std::stringstream stream(s);
	if (!(stream >> port) || !stream.eof() || port < 1 || port > 65535)
		return false;
	return true;
}

bool Config::_parsePasswd(const std::string &passwd)
{
	if (passwd.empty())
		return false;
	for (std::size_t i = 0; i < passwd.size(); ++i)
	{
		if (!std::isprint(static_cast<unsigned char>(passwd[i])))
			return false;
	}
	return true;
}

int Config::getPort() const
{
	return _port;
}

const std::string &Config::getPasswd() const
{
	return _passwd;
}

