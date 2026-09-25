#include "Client.hpp"

Client::Client(int fd)
	: _fd(fd), _nickname(""), _username(""), _realname(""),
	  _state(UNREGISTERED), _providedPassword(false),
	  _inputBuffer(""), _outputBuffer("")
{
}

Client::~Client()
{
}

Client::Client(const Client& other)
	: _fd(other._fd), _nickname(other._nickname), _username(other._username),
	  _realname(other._realname), _state(other._state),
	  _providedPassword(other._providedPassword),
	  _inputBuffer(other._inputBuffer), _outputBuffer(other._outputBuffer)
{
}

Client& Client::operator=(const Client& other)
{
	if (this != &other)
	{
		_fd = other._fd;
		_nickname = other._nickname;
		_username = other._username;
		_realname = other._realname;
		_state = other._state;
		_providedPassword = other._providedPassword;
		_inputBuffer = other._inputBuffer;
		_outputBuffer = other._outputBuffer;
	}
	return (*this);
}

int	Client::getFd() const
{
	return (_fd);
}

const std::string&	Client::getNickname() const
{
	return (_nickname);
}

void	Client::setNickname(const std::string& nickname)
{
	_nickname = nickname;
}

const std::string&	Client::getUsername() const
{
	return (_username);
}

void	Client::setUsername(const std::string& username)
{
	_username = username;
}

const std::string&	Client::getRealname() const
{
	return (_realname);
}

void	Client::setRealname(const std::string& realname)
{
	_realname = realname;
}

Client::RegistrationState	Client::getState() const
{
	return (_state);
}

void	Client::setState(RegistrationState state)
{
	_state = state;
}

bool	Client::hasProvidedPassword() const
{
	return (_providedPassword);
}

void	Client::setProvidedPassword(bool value)
{
	_providedPassword = value;
}

std::string&	Client::inputBuffer()
{
	return (_inputBuffer);
}

std::string&	Client::outputBuffer()
{
	return (_outputBuffer);
}
