#include "Server.hpp"

/* ------------------------------------------------------------------------ */

/* ---------------------------   CONSTRUCTORS   --------------------------- */

/* ------------------------------------------------------------------------ */

/**
 * 
 */
		Server::Server(Config init)
					:	_port(init.getPort()), _password(init.getPassword()), _spareFD(-1) {
	initFDS(initAddr(initSocket()));
}

/**
 * 
 */
int		Server::initSocket() const {
	int serverFD = socket(AF_INET, SOCK_STREAM, 0);
	if (serverFD < 0)
		throw std::runtime_error(IRCFormatter::format(ERR_SCKT));
	
	int	opt = 1;
	if (setsockopt(serverFD, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
		close(serverFD);
		throw std::runtime_error(IRCFormatter::format(ERR_SCKTOPT));
	}
	
	if (fcntl(serverFD, F_SETFL, O_NONBLOCK) < 0) {
		close(serverFD);
		throw std::runtime_error(IRCFormatter::format(ERR_FCNTL));
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
		throw std::runtime_error(IRCFormatter::format(ERR_BIND));
	}
	
	if (listen(serverFD, SOMAXCONN) < 0) {
		close(serverFD);
		throw std::runtime_error(IRCFormatter::format(ERR_LISTEN));
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
		throw std::runtime_error(IRCFormatter::format(ERR_OPEN));
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
			throw std::runtime_error(IRCFormatter::format(ERR_POLL));
		}
		
		if (pollArr[0].revents & (POLLERR | POLLNVAL))
			throw std::runtime_error(IRCFormatter::format(ERR_LISTEN));
		
		try {
			if (pollArr[0].revents & POLLIN)
				addNewClient();
		}
		catch (IRCException::IRCException &e) {
			//Handle failed client acceptance.
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
			catch (const IRCException::IRCException &e) {
				//Handle client specific errors.
				client_it = _clientArr.find(pollArr[i].fd);
				if (client_it != _clientArr.end())
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
	
	for (Socket_it pos = _clientArr.begin(); pos != _clientArr.end(); ;) {
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

	int fd = acceptClient(address);

	Socket *client = NULL;
    try {
        client = new Socket(fd, std::string(inet_ntoa(address.sin_addr)));
        if (!_clientArr.insert(std::make_pair(fd, client)).second)
            throw std::runtime_error(IRCFormatter::format(ERR_NEWCL));
        client = NULL;
    }
    catch (...) {
        delete client;
        close(fd);
        throw; //Throw a formatted message using the errno.
    }
}

/**
 * 
 */
int		Server::acceptClient(struct sockaddr_in &address) {
    socklen_t addrlen = sizeof(address);

    int fd = accept(_pollFD.fd, (struct sockaddr *)&address, &addrlen);
    if (fd < 0) {
        if (errno == EMFILE || errno == ENFILE) {
            close(_spareFD);
            _spareFD = -1;
            int tmp = accept(_pollFD.fd, NULL, NULL);
            if (tmp >= 0) {
                const char msg[] = "ERROR: server full\r\n"; //Needs reply formatting.
                send(tmp, msg, sizeof(msg) - 1, MSG_NOSIGNAL);
                close(tmp);
            }
            _spareFD = open("/dev/null", O_RDONLY);
        }
		//Where is this constructor coming from?
        throw IRCException::IRCException(ERR_INVCLIENT, "failed to accept client address");
    }
	
	if (fcntl(fd, F_SETFL, O_NONBLOCK) < 0) {
        close(fd);
		//its here also!
        throw IRCException::IRCException(ERR_INVCLIENT, "failed to initialize client [fcntl()]");
    }

	return (fd);
}

/**
 * 
 */
void	Server::manageClient(std::map<int, Socket*>::iterator client_it, short revents) {
	std::string	line;

	Socket	*client = client_it->second;
	bool	alive = true;
	
	//Invalid descriptor: remove immediatly.
	if (revents & POLLNVAL) {
		deleteClient(client_it);
		return;
	}
	
	//Read avalible data. Process every complete line in the receive buffer.
	if (revents & (POLLIN | POLLHUP | POLLERR) && !client->isClosing()) {
		alive = client->dataRead();
		while (alive && !client->isClosing() && client->nextLine(line)) {
			if (line.empty())
				continue;
			try {
				Message cmd(Parser::) //What is this?
				CommandRegistry::exe(Parser::processInput(line), *this, *client);
			}
			catch(const IRCException::IRCexception &e) {
				//Handle invalid IRC message.
					//Build a reply of invalid input. Fill the client _buffOut with it.
				//Continue processing subsequent complete lines.
			}
		}
	}
	
	//Flush pending output.
	if (alive && (revents & POLLOUT))
		alive = client->dataSend();
	
	//Remove dead or disconnected clients.
	if (!alive || (revents & (POLLHUP | POLLERR)))
		deleteClient(client_it);
}

/**
 * 
 */
void	Server::deleteClient(std::map<int, Socket*>::iterator client_it) {
	close(client_it->first);
	delete client_it->second;
	_clientArr.erase(client_it);
}


Channel *Server::getChannel(const std::string &name) const {
	std::map<std::string, Channel *>::const_iterator it = _channels.find(name);
	if (it == _channels.end())
		throw IRCException::IRCException(R_NOCHAN, "channel not found"); //Needs the right reply code.
	return it->second;
}

Channel *Server::getOrCreate(const std::string &name) {}
void 	Server::removeChannel(const std::string &name) {}
Socket 	*Server::findClientByNick(const std::string &nick) const {}

void Server::removeClientFromChannels(Socket *client) {
	for (std::map<std::string, Channel *>::iterator it = _channels.begin(); it != _channels.end(); ++it) {
		if (it->second->hasMember(client)) {
			it->second->removeMember(client);
			if (it->second->isEmpty()) {
				delete it->second;
				_channels.erase(it);
			}
		}
	}
}
/* ------------------------------------------------------------------------ */

/* ---------------------------        END       --------------------------- */

/* ------------------------------------------------------------------------ */