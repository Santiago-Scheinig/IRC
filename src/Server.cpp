#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "CommandRegistry.hpp"
#include "Config.hpp"

#include <cstring>
#include <stdexcept>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>

Server::Server(const Config& config)
	: _listenFd(-1), _password(config.getPassword()), _running(false),
	  _pollFds(), _clients(), _channels(), _commandRegistry(new CommandRegistry())
{
	setupListenSocket(config.getPort());
}

Server::~Server()
{
	std::map<int, Client*>::iterator cit = _clients.begin();
	for (; cit != _clients.end(); ++cit)
		delete cit->second;

	std::map<std::string, Channel*>::iterator chit = _channels.begin();
	for (; chit != _channels.end(); ++chit)
		delete chit->second;

	delete _commandRegistry;
	if (_listenFd != -1)
		close(_listenFd);
}

void	Server::setupListenSocket(int port)
{
	_listenFd = socket(AF_INET, SOCK_STREAM, 0);
	if (_listenFd < 0)
		throw std::runtime_error("socket() failed");

	int opt = 1;
	setsockopt(_listenFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	fcntl(_listenFd, F_SETFL, O_NONBLOCK);

	struct sockaddr_in addr;
	std::memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(static_cast<unsigned short>(port));

	if (bind(_listenFd, reinterpret_cast<struct sockaddr*>(&addr), sizeof(addr)) < 0)
		throw std::runtime_error("bind() failed");
	if (listen(_listenFd, SOMAXCONN) < 0)
		throw std::runtime_error("listen() failed");

	struct pollfd listenPfd;
	listenPfd.fd = _listenFd;
	listenPfd.events = POLLIN;
	listenPfd.revents = 0;
	_pollFds.push_back(listenPfd);
}

void	Server::run()
{
	_running = true;
	pollLoop();
}

void	Server::pollLoop()
{
	while (_running)
	{
		int ret = poll(&_pollFds[0], _pollFds.size(), -1);
		if (ret < 0)
			break;

		for (size_t i = 0; i < _pollFds.size(); ++i)
		{
			if (_pollFds[i].revents & POLLIN)
			{
				if (_pollFds[i].fd == _listenFd)
					acceptNewClient();
				else
					handleClientReadable(_pollFds[i].fd);
			}
			// TODO(track A): handle POLLOUT to flush Client::outputBuffer(),
			// and POLLHUP/POLLERR to call disconnectClient().
		}
	}
}

void	Server::acceptNewClient()
{
	int fd = accept(_listenFd, NULL, NULL);
	if (fd < 0)
		return;
	fcntl(fd, F_SETFL, O_NONBLOCK);

	struct pollfd clientPfd;
	clientPfd.fd = fd;
	clientPfd.events = POLLIN;
	clientPfd.revents = 0;
	_pollFds.push_back(clientPfd);

	_clients[fd] = new Client(fd);
}

// TODO(track A): recv() into Client::inputBuffer(), split on "\r\n",
// feed each complete line to Parser::parse() then _commandRegistry->dispatch().
void	Server::handleClientReadable(int fd)
{
	(void)fd;
}

void	Server::handleClientWritable(int fd)
{
	(void)fd;
}

void	Server::disconnectClient(int fd)
{
	std::map<int, Client*>::iterator it = _clients.find(fd);
	if (it != _clients.end())
	{
		delete it->second;
		_clients.erase(it);
	}
	close(fd);
}

Client*	Server::getClient(int fd)
{
	std::map<int, Client*>::iterator it = _clients.find(fd);
	return (it == _clients.end() ? NULL : it->second);
}

Channel*	Server::getOrCreateChannel(const std::string& name)
{
	std::map<std::string, Channel*>::iterator it = _channels.find(name);
	if (it != _channels.end())
		return (it->second);

	Channel* channel = new Channel(name);
	_channels[name] = channel;
	return (channel);
}

Channel*	Server::findChannel(const std::string& name)
{
	std::map<std::string, Channel*>::iterator it = _channels.find(name);
	return (it == _channels.end() ? NULL : it->second);
}

void	Server::removeChannelIfEmpty(const std::string& name)
{
	(void)name;
	// TODO(track C): erase from _channels once Channel exposes an
	// isEmpty()/memberCount() check.
}

const std::string&	Server::getPassword() const
{
	return (_password);
}
