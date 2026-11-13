#include "Server.hpp"

/* ------------------------------------------------------------------------ */
/* ------------------------------ CONSTRUCTORS ---------------------------- */
/* ------------------------------------------------------------------------ */

				Server::Server(Config type)	:	_port(type._port),
												_password(type._pass),
												_spareFD(-1) {
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd < 0)
		throw std::exception(ERR_SCKT); //FATAL needs formating
	initSocket(fd);
	initAddr(fd);
	_pollFD = newPoll(fd);
	_spareFD = open("/dev/null", O_RDONLY);
	if (_spareFD < 0) {
		close(_pollFD.fd);
		throw std::exception(ERR_OPEN); //FATAL needs formating
	}
}

void			Server::initSocket(const int fd) const {
	int	opt = 1;

	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
		close(fd);
		throw std::exception(ERR_SCKTOPT, "setsockopt() failed"); //FATAL needs formating
	}
	if (fcntl(fd, F_SETFL, O_NONBLOCK) < 0) {
		close(fd);
		throw std::exception(ERR_FCNTL, "fcntl() failed"); //FATAL needs formating
	}
}

void			Server::initAddr(const int fd) const {
	struct sockaddr_in	address;

	std::memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(_port);
	if (bind(fd, (struct sockaddr *) &address, sizeof(address)) < 0) {
		close(fd);
		throw std::exception(ERR_BIND, "bind() failed"); //FATAL needs formating
	}
	if (listen(fd, SOMAXCONN) < 0) {
		close(fd);
		throw std::exception(ERR_LISTEN, "listen() Failed"); //FATAL needs formating
	}
}

				Server::~Server() {
	while (!_clientArr.empty())
		deleteSocket(_clientArr.begin());
	close(_pollFD.fd);
	close(_spareFD);
}

/* ------------------------------------------------------------------------ */
/* --------------------------------- PUBLIC ------------------------------- */
/* ------------------------------------------------------------------------ */

void			Server::run() {
	while (!verifyGSF()) {

		std::vector<struct pollfd> pollArr;

		pollArr.push_back(_pollFD);
		for (std::map<int, Socket *>::iterator client_it = _clientArr.begin(); client_it != _clientArr.end(); ++client_it)
			pollArr.push_back(client_it->second->getPollFD());
		
		if (poll(&pollArr[0], pollArr.size(), -1) < 0) {
			if (errno == EINTR)
				continue;
			throw std::runtime_error("poll() Failed"); //Fatal needs formating
		}
		if (pollArr[0].revents & (POLLERR | POLLNVAL))
			throw std::runtime_error("listening socket failed"); //Fatal needs formating
		try {
			if (pollArr[0].revents & POLLIN)
				newSocket();
		}
		catch (IRCException::IRCException &e) {
			//Handle new Client Issues.
		}	
		for (size_t i = 1; i < pollArr.size(); ++i) {
			if (!pollArr[i].revents)
				continue;
			std::map <int, Socket *>::iterator client_it = _clientArr.find(pollArr[i].fd);
			try {
				if (client_it != _clientArr.end()) 
					manageSocket(client_it, pollArr[i].revents);
			}
			catch (IRCException::IRCException &e) {
				//Handle Client Issues
			}
		}
	}
}

/* ------------------------------------------------------------------------ */
/* --------------------------------- PRIVATE ------------------------------ */
/* ------------------------------------------------------------------------ */

struct pollfd	Server::newPoll(const int fd) const throw() {
	struct pollfd newPoll;

	newPoll.fd = fd;
	newPoll.events = POLLIN;
	newPoll.revents = 0;
	return (newPoll);
}

void			Server::newSocket() {
	struct sockaddr_in	address;
	
	socklen_t	addrlen = sizeof(address);
	int 		fd = accept(_pollFD.fd, (struct sockaddr *) &address, &addrlen);
	
	if (fd < 0) {
		if (errno == EMFILE || errno == ENFILE) {
			close(_spareFD);
			int tmp = accept(_pollFD.fd, NULL, NULL);
			if (tmp >= 0) {
				const char msg[] = "ERROR: server full\r\n"; //Needs formating;
				send(tmp, msg, sizeof(msg) - 1, MSG_NOSIGNAL);
				close(tmp);
			}
			_spareFD = open("/dev/null", O_RDONLY);
		}
		/**
	 	*	This fd can return with errnos: EAGAIN, EWOULDBLOCK, ECONNABORTED.
	 	*	If so, detailed throw must be done per each for log purposes only, those arent fatal.
	 	*/
		throw IRCException::IRCException(ERR_INVCLIENT, "failed to accept client address");
	}
	if (fcntl(fd, F_SETFL, O_NONBLOCK) < 0) {
		close(fd);
		throw IRCException::IRCException(ERR_INVCLIENT, "failed to initialize client address [fcntl()]");
	}
	Socket *client = NULL;
	try {
		client = new Socket(fd, std::string(inet_ntoa(address.sin_addr)), newPoll(fd));
		_clientArr.insert(std::make_pair(fd, client));
	}
	catch (std::exception &e) { //Verify this catch, since Socket can fail with both std as IRCException.
		delete client;
		close(fd);
		throw; //FATAL needs formating?
	}
}

void			Server::manageSocket(std::map<int, Socket*>::iterator client_it, short revents) {
	bool	alive = true;

	if (revents & POLLNVAL) {
		deleteSocket(client_it);
		return;
	}
	if (revents & (POLLIN | POLLHUP | POLLERR))
		alive = client_it->second->dataRead();
	if (alive && (revents & POLLOUT))
		client_it->second->dataSend(_clientArr, channelArr);
	if (!alive || (revents & (POLLHUP | POLLERR)))
		deleteSocket(client_it);
}

void			Server::deleteSocket(std::map<int, Socket*>::iterator client_it) {
	close(client_it->first);
	delete client_it->second;
	_clientArr.erase(client_it);
}

bool			Server::matchPassword(const std::string &pass) const throw() {
	return (this->_password == pass);
}

/* ------------------------------------------------------------------------ */
/* ----------------------------------- END -------------------------------- */
/* ------------------------------------------------------------------------ */