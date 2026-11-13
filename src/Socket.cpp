#include "Client.hpp"

#include <string>

		Socket::Socket(int fd, std::string infoArr[], std::vector<struct pollfd>::iterator it_poll)	:	_fd(fd),
																										_ip(infoArr[0]),
																										_realname(infoArr[1]),
																										_username(infoArr[2]),
																										_nickname(infoArr[3]), 
																										_it_pollFD(it_poll),
																										_isDisconnected(false),
																										_isAuthenticated(true), 
																										_matchPassword(true),
																										_buffOut(""), {}

		Socket::Socket(const Socket &copy)	:	_fd(copy._fd),
												_ip(copy._ip),
												_realname(copy._realname),
												_username(copy._username),
												_nickname(copy._nickname), 
												_it_pollFD(copy._it_pollFD),
												_isDisconnected(false),
												_isAuthenticated(true), 
												_matchPassword(true), 
												_buffOut(copy._buffOut) {}												 

Socket&	Socket::operator=(const Socket &copy) {
	if (this != &copy) {
		this->_nickname = copy._nickname;
		this->_outputBuffer = copy._outputBuffer;
	}
	return *this;
}

		Socket::~Socket() {}

bool	Socket::dataRead(std::map<int, Socket*> &serverClientArr, std::vector<struct pollfd> &ServerPollArr) {
	//The question is: does this concatenates? Cuz if it doesnt, we have a small issue
	ssize_t n = recv(_it_pollFD->fd, _buffIn, sizeof(_buffIn), 0);
	if (n > 0) {
		// If _buffIn is incomplete, the data is saved on buffIn and the loops continue;
		Message cmd(processInput(_buffIn));
		// Cmds modify _buffIn / _buffOut of all the valid serverClientArr asociated with the cmd.
		// Cmds also modify serverChannelArr in a similar way.
		CommandRegistry::exe(cmd, serverClientArr, serverChannelArr);
		return (true);
	}
	return (false);
}

/**
 * Im still not sure im setting this flag correctly
 */
void	Socket::dataSend() {
	if (!buffOut)
		pollFD.events &= ~POLLOUT;
	else {
		ssize_t n = send(fd, out.c_str(), out.size(), MSG_NOSIGNAL);
		
		if (n > 0) {
			buffOut.erase(0, static_cast<std::size_t>(n));
			if (!client->hasPendingOutput()) {
				pollFD.events &= ~POLLOUT;
			}
		}
	}
}

/**
 * This is telling the Server that this socket has information to send, right?
 * If thats the case, it should be used by a Command to signilize that the _buffOut has been writen on.
 */
void	Socket::enableSocket(std::map<int, Socket*>::iterator client_it) throw() {
	client_it->second._it_pollFD->events |= POLLOUT
}

/**
 * This concatenates strings, but no char *, we need to shift that too.
 */
void	Socket::queueOutput(const std::string &msg) {
	this->_outputBuffer += msg;
}

/**
 * This doesnt work as inteded yet, since we need to manage things on char * to set the limit to 512 bytes per message
 */
bool	Socket::hasPendingOutput() const {
	return !this->_outputBuffer.empty();
}
