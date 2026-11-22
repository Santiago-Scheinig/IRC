#include "Server.hpp"

/* ------------------------------------------------------------------------ */

/* ---------------------------   CONSTRUCTORS   --------------------------- */

/* ------------------------------------------------------------------------ */

/**
 * 
 */
		Server::Server(Config init)
					:	_port(init.getPort()),
						_password(init.getPassword()),
						_spareFD(-1) {
	initFDS(initAddr(initSocket()));
}

/**
 * 
 */
int		Server::initSocket() const {
	int serverFD = socket(AF_INET, SOCK_STREAM, 0);
	int	opt = 1;

	if (serverFD < 0)
		throw SrvExc(SrvExc::SOCKET_CREATION_FAILED, strerror(errno));
	
	if (setsockopt(serverFD, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
		close(serverFD);
		throw SrvExc(SrvExc::SOCKET_CREATION_FAILED, strerror(errno));
	}
	
	if (fcntl(serverFD, F_SETFL, O_NONBLOCK) < 0) {
		close(serverFD);
		throw SrvExc(SrvExc::SOCKET_CREATION_FAILED, strerror(errno));
	}

	return (serverFD);
}

/**
 * 
 */
int		Server::initAddr(const int serverFD) const {
	struct sockaddr_in	address;
	
	std::memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(_port);
	
	if (bind(serverFD, (struct sockaddr *) &address, sizeof(address)) < 0) {
		close(serverFD);
		throw SrvExc(SrvExc::SOCKET_BIND_FAILED, strerror(errno));
	}
	
	if (listen(serverFD, SOMAXCONN) < 0) {
		close(serverFD);
		throw SrvExc(SrvExc::SOCKET_BIND_FAILED, strerror(errno));
	}

	return (serverFD);
}

/**
 * 
 */
void	Server::initFDS(const int serverFD) {
	_pollFD.fd = serverFD;
	_pollFD.events = POLLIN;
	_pollFD.revents = 0;

	_spareFD = open("/dev/null", O_RDONLY);
	if (_spareFD < 0) {
		close(_pollFD.fd);
		throw SrvExc(SrvExc::SOCKET_BIND_FAILED, strerror(errno));
	}
}

/**
 * 
 */
		Server::~Server() {
	while (!_clientArr.empty())
		deleteClient(_clientArr.begin());
	close(_pollFD.fd);
	close(_spareFD);
}

/* ------------------------------------------------------------------------ */

/* ----------------------      SERVER MANAGMENT      ---------------------- */

/* ------------------------------------------------------------------------ */

/**
 * 
 */
void	Server::run() {
	while (!g_shutdown) {
		std::vector<struct pollfd> pollArr;

		pollArr.push_back(_pollFD);
		for (std::map<int, Socket *>::iterator client_it = _clientArr.begin(); client_it != _clientArr.end(); ++client_it)
			pollArr.push_back(client_it->second->getPollFD());
		
		if (poll(&pollArr[0], pollArr.size(), -1) < 0) {
			if (errno == EINTR) {
				if (g_shutdown)
					break;
				continue;
			}
			throw SrvExc(SrvExc::POLL_INITIALIZATION_FAILED, strerror(errno));
		}
		
		if (pollArr[0].revents & (POLLERR | POLLNVAL))
			throw SrvExc(SrvExc::INVALID_INTERNAL_STATE, strerror(errno));

		try {
			if (pollArr[0].revents & POLLIN)
				addNewClient();
		}
		catch (const ClientException &e) {
			std::cout << e.what();
		}	
		
		for (size_t i = 1; i < pollArr.size(); ++i) {
			
			if (g_shutdown)
				break;
			
			if (!pollArr[i].revents)
				continue;
			
			std::map <int, Socket *>::iterator client_it = _clientArr.find(pollArr[i].fd);
	
			if (client_it == _clientArr.end())
				continue;
				
			try {
				manageClient(client_it, pollArr[i].revents);
			}
			catch (const ClientException &e) {
				std::cout << e.what();
				deleteClient(client_it);
			}
		}
		reapClosing();
	}	
}

/**
 * 
 */
bool	Server::matchPassword(const std::string &pass) const throw() {
	return (_password == pass);
}

/**
 * 
 */
void	Server::reapClosing() {
	typedef std::map<int, Socket*>::iterator Socket_it;
	
	for (Socket_it pos = _clientArr.begin(); pos != _clientArr.end();) {
		Socket_it current_pos = pos++;
		if (current_pos->second->isClosing() && !current_pos->second->hasPendingOutput()) {
			deleteClient(current_pos);
		}
	}
}

/* ------------------------------------------------------------------------ */

/* ----------------------      CLIENT MANAGMENT      ---------------------- */

/* ------------------------------------------------------------------------ */

/**
 * 
 */
void	Server::addNewClient() {
	struct sockaddr_in	address;

	int 				fd = acceptClient(address);
	Socket				*client = NULL;

    try {
        client = new Socket(fd, std::string(inet_ntoa(address.sin_addr)));
        if (!_clientArr.insert(std::make_pair(fd, client)).second)
            throw SrvExc(SrvExc::SOCKET_CREATION_FAILED, strerror(errno));
        client = NULL;
    }
    catch (const std::exception& e) {
        delete client;
        close(fd);
        throw SrvExc(SrvExc::INTERNAL_OPERATION_FAILED, e.what());
    }
}

/**
 * 
 */
int		Server::acceptClient(struct sockaddr_in &address) {
    socklen_t 	addrlen = sizeof(address);
    int 		fd = accept(_pollFD.fd, (struct sockaddr *)&address, &addrlen);
    
	if (fd < 0) {
        if (errno == EMFILE || errno == ENFILE) {
            close(_spareFD);
            _spareFD = -1;
            int tmp = accept(_pollFD.fd, NULL, NULL);
            if (tmp >= 0) {
				const char msg[] = "ERROR :Server is full (maximum file descriptors reached)\r\n";
                send(tmp, msg, sizeof(msg) - 1, MSG_NOSIGNAL);
                close(tmp);
            }
            _spareFD = open("/dev/null", O_RDONLY);
        }
        throw CliExc(CliExc::CONNECTION_ACCEPT_FAILED, "unavalible file descriptors to open connection");
    }
	
	if (fcntl(fd, F_SETFL, O_NONBLOCK) < 0) {
        close(fd);
        throw CliExc(CliExc::CONNECTION_ACCEPT_FAILED, strerror(errno));
    }

	return (fd);
}

/**
 * 
 */
void	Server::manageClient(std::map<int, Socket*>::iterator client_it, short revents) {
	std::string	line;

	Socket		*client = client_it->second;
	bool		alive = true;
	
	if (revents & POLLNVAL)
		throw CliExc(CliExc::INVALID_CLIENT_STATE);
	
	if (revents & (POLLIN | POLLHUP | POLLERR) && !client->isClosing()) {
		alive = client->dataRead();
		while (alive && !client->isClosing() && client->nextLine(line)) {
			if (line.empty())
				continue;
			CommandRegistry::exe(Parser::processInput(line), *this, *client);
		}
	}
	
	if (alive && (revents & POLLOUT))
		alive = client->dataSend();
	
	if (!alive || (revents & (POLLHUP | POLLERR)))
		throw CliExc(CliExc::CONNECTION_LOST);
}

/**
 * 
 */
void	Server::deleteClient(std::map<int, Socket*>::iterator client_it) {
	close(client_it->first);
	delete client_it->second;
	_clientArr.erase(client_it);
}

/* ------------------------------------------------------------------------ */

/* ---------------------------        END       --------------------------- */

/* ------------------------------------------------------------------------ */