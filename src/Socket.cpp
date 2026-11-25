#include "Socket.hpp"

/**
 * 
 */
static const Socket::prefix prefixArr[] = {
    
	// CONNECTION REGISTRATION
    {Socket::RPL_WELCOME,            "Welcome to the Internet Relay Network "},
    {Socket::RPL_YOURHOST,           "Your host is "},
    {Socket::RPL_CREATED,            "This server was created "},
    {Socket::RPL_MYINFO,             ""},

    // SERVER INFORMATION
    {Socket::RPL_LUSERCLIENT,        "There are "},
    {Socket::RPL_LUSEROP,            ""},
    {Socket::RPL_LUSERUNKNOWN,       ""},
    {Socket::RPL_LUSERCHANNELS,      ""},
    {Socket::RPL_LUSERME,            "I have "},

    // CHANNEL INFORMATION
    {Socket::RPL_NOTOPIC,            "No topic is set"},
    {Socket::RPL_TOPIC,              ""},
    {Socket::RPL_NAMREPLY,           ""},
    {Socket::RPL_ENDOFNAMES,         "End of /NAMES list"},

    // GENERAL ERRORS
    {Socket::ERR_NOSUCHNICK,         "No such nick/channel"},
    {Socket::ERR_NOSUCHSERVER,       "No such server"},
    {Socket::ERR_NOSUCHCHANNEL,      "No such channel"},
    {Socket::ERR_CANNOTSENDTOCHAN,   "Cannot send to channel"},
    {Socket::ERR_TOOMANYCHANNELS,    "You have joined too many channels"},
    {Socket::ERR_NOORIGIN,           "No origin specified"},
	{Socket::ERR_INVALIDCAP, 		 "Invalid CAP command"},
    {Socket::ERR_NORECIPIENT,        "No recipient given"},
    {Socket::ERR_NOTEXTTOSEND,       "No text to send"},
	{Socket::ERR_INPUTTOOLONG,		 "Input too long"},
    {Socket::ERR_UNKNOWNCOMMAND,     "Unknown command"},

    // NICKNAME ERRORS
    {Socket::ERR_NONICKNAMEGIVEN,    "No nickname given"},
    {Socket::ERR_ERRONEUSNICKNAME,   "Erroneous nickname"},
    {Socket::ERR_NICKNAMEINUSE,      "Nickname is already in use"},

    // CHANNEL ERRORS
    {Socket::ERR_USERNOTINCHANNEL,   "They aren't on that channel"},
    {Socket::ERR_NOTONCHANNEL,       "You're not on that channel"},
    {Socket::ERR_USERONCHANNEL,      "is already on channel"},
    {Socket::ERR_CHANNELISFULL,      "Cannot join channel (+l)"},
    {Socket::ERR_INVITEONLYCHAN,     "Cannot join channel (+i)"},
    {Socket::ERR_BANNEDFROMCHAN,     "Cannot join channel (+b)"},
    {Socket::ERR_BADCHANNELKEY,      "Cannot join channel (+k)"},
    {Socket::ERR_CHANOPRIVSNEEDED,   "You're not channel operator"},

    // REGISTRATION ERRORS
    {Socket::ERR_NOTREGISTERED,      "You have not registered"},
    {Socket::ERR_NEEDMOREPARAMS,     "Not enough parameters"},
    {Socket::ERR_ALREADYREGISTERED,  "You may not reregister"},
    {Socket::ERR_PASSMISMATCH,     "Password incorrect"},

    {Socket::END, " invalid reply code"}
};

/* ------------------------------------------------------------------------ */

/* ---------------------------   CONSTRUCTORS   --------------------------- */

/* ------------------------------------------------------------------------ */

/**
 * 
 */
		Socket::Socket(int fd, const std::string &ip)	
			:	_ip(ip),
				_hasPass(false),
				_registered(false),
				_closing(false) {
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

	if (n == 0)
		return (false);

	if (n < 0) {

		if (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR)
			return (true);

		return (false);
	}

	_buffIn.append(buf, n);

	return (true);
}

/**
 * 
 */
bool	Socket::nextLine(std::string &line) {
	std::string::size_type	pos = _buffIn.find('\n');

	if (pos == std::string::npos) {

		if (_buffIn.size() > 512) {

			_buffIn.clear();
			queueOutput(newReply(ERR_INPUTTOOLONG, "input line was too long"));
		
		}
		
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
		
		if (n == 0)
			return (false);
		
		if (n < 0) {
			
			if (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR)
				return (true);
			
			return (false);
		}

		_buffOut.erase(0, n);
	}

	if (_buffOut.empty())
		_pollFD.events &= ~POLLOUT;
	
	return (true);
}

/**
 *
 */
std::string Socket::newReply(const replyco code, const std::string &text) {
    std::ostringstream  reply;
    std::string         def;

    for (int i = 0; prefixArr[i].code != END; ++i) {

		if (prefixArr[i].code == code) {
			def = prefixArr[i].str; 
			break; 
		}
	}
		
    std::string			&t = def;
	std::string			target = "*";

	if (!text.empty())
		t = text;

	if (!_nickname.empty())
		target = _nickname;

    reply << ":" << SERVER_NAME << " " << std::setw(3) << std::setfill('0') << static_cast<int>(code) << " " << target;
    
	if (!t.empty())
		reply << " :" << t;
    reply << "\r\n";
    return (reply.str());
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
const std::string		&Socket::getIP() const throw() {
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
