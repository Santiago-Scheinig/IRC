#include "../include/Server.hpp"

void Server::_initSocket()
{
	_serverFd = socket(AF_INET, SOCK_STREAM, 0);
	if (_serverFd == -1)
		throw std::runtime_error("socket() failed");

	std::cout << "Fd : "<< _serverFd << std::endl;
	int reuse = 1;
	if (setsockopt(_serverFd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) == -1)
	{
		close(_serverFd);
		_serverFd = -1;
		throw std::runtime_error("setsockopt() failed");
	}

	if (fcntl(_serverFd, F_SETFL, O_NONBLOCK) == -1)
	{
		close(_serverFd);
		_serverFd = -1;
		throw std::runtime_error("fcntl() failed");
	}

	sockaddr_in address;
	std::memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl(INADDR_ANY);
	address.sin_port = htons(_port);

	if (bind(_serverFd, reinterpret_cast<sockaddr *>(&address), sizeof(address)) == -1)
	{
		close(_serverFd);
		_serverFd = -1;
		throw std::runtime_error("bind() failed");
	}

	if (listen(_serverFd, SOMAXCONN) == -1)
	{
		close(_serverFd);
		_serverFd = -1;
		throw std::runtime_error("listen() failed");
	}

	struct pollfd pfd;
	pfd.fd = _serverFd;
	pfd.events = POLLIN;
	pfd.revents = 0;
	_pollfds.push_back(pfd);
	std::cout << "Server initialized on port " << _port << std::endl;
}

Server::Server(int port, const std::string& password) : _port(port), _password(password), _serverFd(-1)
{
	_initSocket();
}

Server::~Server()
{
	if (_serverFd != -1)
		close(_serverFd);
}

void Server::run(){

	std::cout << "Server running. Waiting for connections..." << std::endl;
	while (g_running)
	{
		if (poll(_pollfds.data(), _pollfds.size(), -1) < 0){
			if (!g_running)
				break ;
			throw std::runtime_error("poll() failed");
		}
		/*for (size_t i = 0; i < _pollfds.size(); i++){
			int fd = _pollfds[i].fd;
			short revents = _pollfds[i].revents;
			if (fd == _serverFd){
				if (revents & POLLIN)
					_acceptClient();
				continue ;
			}
			if (revents & POLLIN) { _handleClient(fd); }
			if (_clients.find(fd) == _clients.end()) { continue ; }
			if (revents & POLLOUT) { _flushClientOutput(fd); }
		}*/
	}
	std::cout << "Server shutting down." << std::endl;
}

void Server::_handleClient(int fd){
	(void)fd;

	Message message;

	//data = recibirBytesDelCliente();
	std::string data(":nick!user@host PRIVMSG #general :Hola\r\n");

	Parser clientParser;
	Parser::Parse_e result = clientParser.processInput(data, message);

	if (result == Parser::PARSE_INCOMPLETE)
	{
		// Esperar más bytes.
	}
	else if (result == Parser::PARSE_INVALID)
	{
		// Rechazar la línea o cerrar el cliente.
	}
	else if (result == Parser::PARSE_MESSAGE)
	{
		// Enviar message al CommandRegistry.
	}
}