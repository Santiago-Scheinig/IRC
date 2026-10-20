#include "Client.hpp"

#include <string>

Client::Client(int fd, std::string ip) : _fd(fd), _ip(ip) {
	_nickname = "";
	_username = "";
	_outputBuffer = "";
	_isAuthenticated = false;
}

Client::Client(const Client &copy) : _fd(copy._fd), _ip(copy._ip) {
	this->_nickname = copy._nickname;
	this->_username = copy._username;
	this->_outputBuffer = copy._outputBuffer;
	this->_isAuthenticated = copy._isAuthenticated;
}

Client& Client::operator=(const Client &copy) {
	if (this != &copy) {
		this->_fd = copy._fd;
		this->_ip = copy._ip;
		this->_nickname = copy._nickname;
		this->_username = copy._username;
		this->_outputBuffer = copy._outputBuffer;
		this->_isAuthenticated = copy._isAuthenticated;
	}
	return *this;
}

Client::~Client() {}

int Client::getFd() const {
	return this->_fd;
}

Parser::Parse_e Client::parseData(const std::string &data, Message &msg) {
	return _parser.processInput(data, msg);
}

void Client::queueOutput(const std::string &msg) {
	this->_outputBuffer += msg;
}