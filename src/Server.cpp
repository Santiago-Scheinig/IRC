#include "Server.hpp"

Server::Server(int port, std::string pass) : _port(port), _pass(pass), _serverFd(-1) {
}

Server::Server(const Server &copy) {}

Server& Server::operator=(const Server &copy) {}

Server::~Server() {
	for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
		close(it->first);
		delete it->second;
	}
	_clients.clear();
	if (_serverFd != -1)
		close(_serverFd);
}

void Server::initServer() {
	_serverFd = socket(AF_INET, SOCK_STREAM, 0);
	if (_serverFd < 0)
		throw std::runtime_error("socket() failed");
	int opt = 1;
	if (setsockopt(_serverFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
		close(_serverFd);
		throw std::runtime_error("setsockopt() failed");
	}
	if (fcntl(_serverFd, F_SETFL, O_NONBLOCK) < 0) {
		close(_serverFd);
		throw std::runtime_error("fcntl() failed");
	}

	struct sockaddr_in addr;
	std::memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(_port);
	if (bind(_serverFd, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
		close(_serverFd);
		throw std::runtime_error("bind() failed");
	}
	if (listen(_serverFd, SOMAXCONN) < 0) {
		close(_serverFd);
		throw std::runtime_error("listen() Failed");
	}
	struct pollfd pdf;
	pdf.fd = _serverFd;
	pdf.events = POLLIN;
	pdf.revents = 0;
	_pollFds.push_back(pdf);
}

void Server::run() {
	while (true) {
		int pollret = poll(&_pollFds[0], _pollFds.size(), -1);
		if (pollret < 0)
			throw std::runtime_error("poll() Failed");
		else {
			for (size_t i = 0; i != _pollFds.size(); ++i) {
				if (_pollFds[i].revents & (POLLERR | POLLHUP | POLLNVAL)) {
					close(_pollFds[i].fd);
					std::map<int, Client*>::iterator it = _clients.find(_pollFds[i].fd);
					if (it != _clients.end()) {
						delete it->second;
						_clients.erase(it);
					}
					_pollFds.erase(_pollFds.begin() + i);
					--i;
					continue;
				}
				if (_pollFds[i].revents & POLLIN) {
					if (_pollFds[i].fd == _serverFd)
						acceptNewClient();
					else
						receiveData(_pollFds[i].fd);
				}
				if (_pollFds[i].revents & POLLOUT) {
					if (_clients.find(_pollFds[i].fd) != _clients.end())
						sendData(_pollFds[i].fd);
				}
			}
		}
	}
}
