#include "Client.hpp"

#include <string>

Client::Client(int fd, std::string ip) : _fd(fd), _ip(ip) {
	_nickname = "";
	_username = "";
	_inputBuffer = "";
	_outputBuffer = "";
	_isAuthenticated = false;
}

Client::Client(const Client &copy) : _fd(copy._fd), _ip(copy._ip) {
	this->_nickname = copy._nickname;
	this->_username = copy._username;
	this->_inputBuffer = copy._inputBuffer;
	this->_outputBuffer = copy._outputBuffer;
	this->_isAuthenticated = copy._isAuthenticated;
}

Client& Client::operator=(const Client &copy) {
	if (this != &copy) {
		this->_fd = copy._fd;
		this->_ip = copy._ip;
		this->_nickname = copy._nickname;
		this->_username = copy._username;
		this->_inputBuffer = copy._inputBuffer;
		this->_outputBuffer = copy._outputBuffer;
		this->_isAuthenticated = copy._isAuthenticated;
	}
	return *this;
}

Client::~Client() {}

int Client::getFd() const {
	return this->_fd;
}

void Client::appendInput(const std::string &data) {
	this->_inputBuffer += data;
}

std::string Client::extractMessage(const std::string &data, Message &message) {
	size_t pos = _inputBuffer.find("\r\n", 0);
	if (pos == std::string::npos)
		return ""; //PARSE_INCOMPLETE;
	std::string command = _inputBuffer.substr(0, pos);
	_inputBuffer.erase(0, pos + 2);
	return command;
}

void Client::queueOutput(const std::string &msg) {
	this->_outputBuffer += msg;
}