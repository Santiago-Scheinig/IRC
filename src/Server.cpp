#include "Server.hpp"
#include "Replies.hpp"

Server::Server(int port, std::string pass) : _port(port), _pass(pass), _serverFd(-1) {
}

Server::~Server() {
	std::map<std::string, Channel*>::iterator itCh = _channels.begin();
	while (itCh != _channels.end()) {
		delete itCh->second;
		_channels.erase(itCh++);
	}
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
		_serverFd = -1;
		throw std::runtime_error("bind() failed");
	}
	if (listen(_serverFd, SOMAXCONN) < 0) {
		close(_serverFd);
		_serverFd = -1;
		throw std::runtime_error("listen() Failed");
	}
	struct pollfd pfd;
	pfd.fd = _serverFd;
	pfd.events = POLLIN;
	pfd.revents = 0;
	_pollFds.push_back(pfd);
}

void Server::deleteFd(size_t i) {
	int fd = _pollFds[i].fd;
	std::map<int, Client*>::iterator it = _clients.find(fd);
	if (it != _clients.end()) {
		removeClientFromChannels(it->second);
		delete it->second;
		_clients.erase(it);
	}
	close(fd);
	_pollFds[i] = _pollFds.back();
	_pollFds.pop_back();
}

void Server::removeClientFromChannels(Client *client) {
	std::map<std::string, Channel*>::iterator it = _channels.begin();
	while (it != _channels.end()) {
		Channel *channel = it->second;
		channel->removeMember(client);
		if (channel->isEmpty()) {
			delete channel;
			std::map<std::string, Channel*>::iterator current = it;
			++it;
			_channels.erase(current);
		}
		else
			++it;
	}
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
			if (ev & (POLLNVAL | POLLERR)) {
				if ((ev & POLLERR) && _pollFds[i].fd == _serverFd)
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
				if (_clients.find(_pollFds[i].fd) != _clients.end() && !sendData(_pollFds[i].fd)) {
					deleteFd(i);
					--i;
					continue;
				}
			}
		}
	}
}

void Server::acceptNewClient() {
	struct sockaddr_in clientAddr;
	socklen_t clientAddrLen = sizeof(clientAddr);
	int clientFd = accept(_serverFd, (struct sockaddr *) &clientAddr, &clientAddrLen);
	if (clientFd < 0)
		return;
	if (fcntl(clientFd, F_SETFL, O_NONBLOCK) < 0) {
		close(clientFd);
		return;
	}
	struct pollfd pfd;
	pfd.fd = clientFd;
	pfd.events = POLLIN;
	pfd.revents = 0;
	_pollFds.push_back(pfd);
	_clients[clientFd] = new Client(clientFd, std::string(inet_ntoa(clientAddr.sin_addr)));
}

bool Server::receiveData(int fd) {
	std::map<int, Client*>::iterator it = _clients.find(fd);
	if (it == _clients.end())
		return false;
	Client *client = it->second;
	char buf[512];
	ssize_t n = recv(fd, buf, sizeof(buf), 0);
	if (n == 0)
		return false;
	if (n < 0)
		return true;
	std::string pending(buf, static_cast<std::size_t>(n));
	while (true) {
		Message msg;
		Parser::Parse_e result = client->parseData(pending, msg);
		pending.clear();
		if (result == Parser::PARSE_INCOMPLETE)
			break;
		if (result == Parser::PARSE_INVALID) {
			Replies::numeric("ircserver", "421", client->getNickName(), "Unknown command: " + msg.getCommand());
			enableWrite(fd);
			continue;
		}
		if (result == Parser::PARSE_MESSAGE) {
			//ejecutar el command.registry de msg aqui.
		}
		//registry.dispatch(); 
	}
	return true;
}

bool Server::sendData(int fd) {
	std::map<int, Client*>::iterator it = _clients.find(fd);
	if (it == _clients.end())
		return false;
	Client *client = it->second;
	std::string &out = client->outputBuffer();
	if (out.empty()) {
		for (size_t i = 0; i != _pollFds.size(); ++i) {
			if (_pollFds[i].fd == fd) {
				_pollFds[i].events &= ~POLLOUT;
				break;
			}
		}
		return true;
	}
	ssize_t n = send(fd, out.c_str(), out.size(), MSG_NOSIGNAL);
	if (n <= 0)
		return true;
	out.erase(0, static_cast<std::size_t>(n));
	if (!client->hasPendingOutput()) {
		for (size_t i = 0; i != _pollFds.size(); ++i) {
			if (_pollFds[i].fd == fd)
				_pollFds[i].events &= ~POLLOUT;
				break;
		}
	}
	return true;
}

void Server::enableWrite(int fd) {
	for (size_t i = 0; i != _pollFds.size(); ++i) {
		if (_pollFds[i].fd == fd) {
			_pollFds[i].events |= POLLOUT;
			return;
		}
	}
}

// checks if the password matches the one in the config file
// this functions wshould be called for PASS command
bool Server::passwordMatches(const std::string &pass) const {
	return this->_pass == pass;
}
