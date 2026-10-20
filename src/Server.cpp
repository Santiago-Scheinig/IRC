#include "Server.hpp"

Server::Server(int _port, std::string _pass) {
	_port = 0;
	_pass = "";
	_serverFd = -1;
}

Server::Server(const Server &copy) {
	_port = copy._port;
	_pass = copy._pass;
	_serverFd = -1;
}

Server& Server::operator=(const Server &copy) {
	if (this != &copy) {
		for (int i = 0; i < _pollFds.size(); ++i) {
			
		}
	}
	return *this;
}

Server::~Server() {

}

