#include "Client.hpp"

#include <string>

Client::Client(int fd, std::string ip) : _fd(fd), _ip(ip) {
	_realname = "";
	_nickname = "";
	_username = "";
	_outputBuffer = "";
	_isAuthenticated = false;
	_passworkOk = false;
}

Client::Client(const Client &copy) : _fd(copy._fd), _ip(copy._ip) {
	this->_realname = copy._realname;
	this->_nickname = copy._nickname;
	this->_username = copy._username;
	this->_outputBuffer = copy._outputBuffer;
	this->_isAuthenticated = copy._isAuthenticated;
	this->_passworkOk = copy._passworkOk;
}

Client& Client::operator=(const Client &copy) {
	if (this != &copy) {
		this->_fd = copy._fd;
		this->_ip = copy._ip;
		this->_realname = copy._realname;
		this->_nickname = copy._nickname;
		this->_username = copy._username;
		this->_outputBuffer = copy._outputBuffer;
		this->_isAuthenticated = copy._isAuthenticated;
		this->_passworkOk = copy._passworkOk;
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

bool Client::hasPendingOutput() const {
	return !this->_outputBuffer.empty();
}

std::string &Client::outputBuffer() {
	return this->_outputBuffer;
}

const std::string &Client::getRealName() const {
	return this->_realname;
}

const std::string &Client::getNickName() const {
	return this->_nickname;
}

const std::string &Client::getUserName() const {
	return this->_username;
}

const std::string &Client::getIp() const {
	return this->_ip;
}

bool Client::isAuthenticated() const {
	return _isAuthenticated;
}

bool Client::isPassworkOk() const {
	return this->_passworkOk;
}

void Client::setRealName(const std::string &realname) {
	this->_realname = realname;
}

void Client::setNickName(const std::string &nickname) {
	this->_nickname = nickname;
}

void Client::setUserName(const std::string &username) {
	this->_username = username;
}

void Client::setAuthenticated(bool isAuthenticated) {
	this->_isAuthenticated = isAuthenticated;
}

void Client::setPassworkOk(bool passworkOk) {
	this->_passworkOk = passworkOk;
}
