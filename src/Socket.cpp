#include "Socket.hpp"

#include <string>


/* ------------------------------------------------------------------------ */

/* ---------------------------   CONSTRUCTORS   --------------------------- */

/* ------------------------------------------------------------------------ */

/**
 * 
 */
		Socket::Socket(int fd, const &std::string ip)	
			:	_ip(ip), _hasPass(false), _registered(false), _closing(false) {
	_pollFD.fd = fd;
	_pollFD.events = POLLIN;
	_pollFD.revents = 0;
}

/**
 * 
 */
		Socket::~Socket() {}


/* ------------------------------------------------------------------------ */

/* --------------------------- MEMBER FUNCTIONS --------------------------- */

/* ------------------------------------------------------------------------ */

/**
 * 
 */
bool	Socket::dataRead() {
	char	buf[512];
	ssize_t n = recv(_pollFD.fd, buf, sizeof(buf), 0);

	if (n <= 0) {
		return (false);
	}
	_buffIn.append(buf, n);
	return (true);
}

/**
 * 
 */
bool	Socket::nextLine(std::string &line) {
	std::string::size_type pos = _buffIn.find('\n');

	if (pos == std::string::npos) {
		if (_buffIn.size() > 512)	//No terminator and already too long
			_buffIn.clear();		// or queue ERR_INPUTTOOLONG (417)
		return (false);
	}
	line = _buffIn.substr(0, pos);
	_buffIn.erase(0, pos + 1);
	if (!line.empty() && line[line.size() - 1] == '\r')
		line.erase(line.size() - 1);
	return (true);
}

/**
 * 
 */
void	Socket::queueOutput(const std::string &msg) {
	_buffOut += msg;
	_pollFD.events |= POLLOUT;
}

/**
 * 
 */
bool	Socket::dataSend() {
	if (!_buffOut.empty()) {
		ssize_t n = send(_pollFD.fd, _buffOut.data(), _buffOut.size(), MSG_NOSIGNAL);
		if (n <= 0) {
			return (false);
		}
		_buffOut.erase(0, n);
	}
	if (_buffOut.empty())
		_pollFD.events &= ~POLLOUT;
	return (true);
}

/* ------------------------------------------------------------------------ */

/* ---------------------------      SETTERS     --------------------------- */

/* ------------------------------------------------------------------------ */

/**
 * 
 */
void	Socket::setNickname(const std::string &str) {
	_nickname = str;
}

/**
 * 
 */
void	Socket::setUsername(const std::string &str) {
	_username = str;
}

/**
 * 
 */
void	Socket::setRealname(const std::string &str) {
	_realname = str;
}

/**
 * 
 */
void	Socket::setRegistered(bool ans) {
	_registered = ans;
}

/**
 * 
 */
void	Socket::setPass(bool ans) {
	_hasPass = ans;
}

/**
 * 
 */
void	Socket::setClosing() {
	_closing = true;
	_pollFD.events &= ~POLLIN;
}

/* ------------------------------------------------------------------------ */

/* ---------------------------      GETTERS     --------------------------- */

/* ------------------------------------------------------------------------ */

/**
 * 
 */
const std::string		&Socket::getNickname() const throw() {
	return (_nickname);
}

/**
 * 
 */
const std::string		&Socket::getUsername() const throw() {
	return (_username);
}

/**
 * 
 */
const std::string		&Socket::getRealname() const throw() {
	return (_realname);
}

/**
 * 
 */
const struct pollfd		&Socket::getPollFD() const throw() {
	return (_pollFD);
}

/**
 * 
 */
const int				&Socket::getFD() const throw() {
	return (_pollFD.fd);
}

/**
 * 
 */
const int				&Socket::getIP() const throw() {
	return (_ip);
}

/**
 * 
 */
bool					Socket::hasPendingOutput() const throw() {
	return !(_buffOut.empty());
}

/**
 * 
 */
bool					Socket::hasPass() const throw() {
	return (_hasPass);
}

/**
 * 
 */
bool					Socket::isRegistered() const throw() {
	return (_registered);
}

/**
 * 
 */
bool					Socket::isClosing() const throw() {
	return (_closing);
}

/* ------------------------------------------------------------------------ */

/* ---------------------------        END       --------------------------- */

/* ------------------------------------------------------------------------ */
