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

void Server::deleteFd(size_t i) {
	int fd = _pollFds[i].fd;
	std::map<int, Client*>::iterator it = _clients.find(fd);
	if (it != _clients.end()) {
		delete it->second;
		_clients.erase(it);
	}
	close(fd);
	_pollFds[i] = _pollFds.back();
	_pollFds.pop_back();
}

void Server::run() {
	while (true) {
		int pollret = poll(&_pollFds[0], _pollFds.size(), -1);
		if (pollret < 0) {
			if (errno == EINTR)
				continue;
			throw std::runtime_error("poll() Failed");
		}
		for (size_t i = 0; i != _pollFds.size(); ++i) {
			short ev = _pollFds[i].revents;
			if (ev & POLLNVAL) {
				deleteFd(i);
				--i;
				continue;
			}
			if (ev & POLLERR) {
				if (_pollFds[i].fd == _serverFd)
					throw std::runtime_error("_serverFd Failed, Fatal error!!");
				deleteFd(i);
				--i;
				continue;
			}
			if (ev & (POLLIN | POLLHUP)) {
				if (_pollFds[i].fd == _serverFd) {
					if (ev & POLLHUP)
						throw std::runtime_error("_serverFd Failed, Fatal error!!");
					acceptNewClient();
				}
				else if (!receiveData(_pollFds[i].fd)) {
					deleteFd(i);
					--i;
					continue;
				}
			}
			if (ev & POLLOUT) {
				if (_clients.find(_pollFds[i].fd) != _clients.end())
					sendData(_pollFds[i].fd);
			}
		}
	}
}

bool Server::receiveData(int fd) {
	std::map<int, Client*>::iterator it = _clients.find(fd);
	if (it == _clients.end())
		return false;
	Client *client = it->second;
	char buf[512];
	while (true) {
		ssize_t n = recv(fd, buf, sizeof(buf), 0);
		if (n > 0) {
			std::string pending(buf, static_cast<std::size_t>(n));
			while (true) {
				Message msg;
				Parser::Parse_e result = client->parseData(pending, msg);
				pending.clear();
				if (result == Parser::PARSE_INCOMPLETE)
					break;
				if (result == Parser::PARSE_MESSAGE) {
					//ejecutar el comando de msg aqui.
				}
			}
			continue;
		}
		if (n == 0)
			return false;
		if (errno == EAGAIN || errno == EWOULDBLOCK)
			return true;
		return false;
	}
}
