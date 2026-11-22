#ifndef SOCKET_HPP
# define SOCKET_HPP

# ifndef SERVER_NAME
#  define SERVER_NAME "irc.42.com"
# endif

# include "Parser.hpp"

# include <sstream>
# include <iomanip>
# include <poll.h>

class Socket {

	public:

		struct 				prefix {
			const int 		code;
			const char 		*str;
		};

		enum 	 			replyco {

		    /* CONNECTION REGISTRATION */

   			RPL_WELCOME             = 001,
   			RPL_YOURHOST            = 002,
   			RPL_CREATED             = 003,
   			RPL_MYINFO              = 004,

    		/* SERVER INFORMATION */

    		RPL_LUSERCLIENT         = 251,
    		RPL_LUSEROP             = 252,
    		RPL_LUSERUNKNOWN        = 253,
    		RPL_LUSERCHANNELS       = 254,
    		RPL_LUSERME             = 255,

    		/* CHANNEL INFORMATION */

    		RPL_NOTOPIC             = 331,
    		RPL_TOPIC               = 332,
    		RPL_NAMREPLY            = 353,
    		RPL_ENDOFNAMES          = 366,

    		/* GENERAL ERRORS */

    		ERR_NOSUCHNICK          = 401,
    		ERR_NOSUCHSERVER        = 402,
    		ERR_NOSUCHCHANNEL       = 403,
    		ERR_CANNOTSENDTOCHAN    = 404,
    		ERR_TOOMANYCHANNELS     = 405,

    		ERR_NOORIGIN            = 409,
    		ERR_NORECIPIENT         = 411,
    		ERR_NOTEXTTOSEND        = 412,
    		ERR_UNKNOWNCOMMAND      = 421,

    		/* NICKNAME ERRORS */

    		ERR_NONICKNAMEGIVEN     = 431,
    		ERR_ERRONEUSNICKNAME    = 432,
    		ERR_NICKNAMEINUSE       = 433,

    		/* CHANNEL ERRORS */

    		ERR_USERNOTINCHANNEL    = 441,
    		ERR_NOTONCHANNEL        = 442,
    		ERR_USERONCHANNEL       = 443,

    		ERR_CHANNELISFULL       = 471,
    		ERR_INVITEONLYCHAN      = 473,
    		ERR_BANNEDFROMCHAN      = 474,
    		ERR_BADCHANNELKEY       = 475,
    		ERR_CHANOPRIVSNEEDED    = 482,

    		/* REGISTRATION ERRORS */

    		ERR_NOTREGISTERED       = 451,
    		ERR_NEEDMOREPARAMS      = 461,
    		ERR_ALREADYREGISTERED   = 462,
    		ERR_PASSWDMISMATCH      = 464,

    		END = 000
		};

	private:

		const std::string	_ip; 
		std::string			_nickname, _username, _realname;
		bool	 			_hasPass, _registered, _closing;
		
		std::string			_buffIn, _buffOut;
		struct pollfd		_pollFD;
		
							Socket(const Socket &other);
		Socket				&operator=(const Socket &other);

	public:

							Socket(int fd, const std::string &ip);
							~Socket();

		bool				dataRead();
		bool				dataSend();
		bool				nextLine(std::string &line);
		void 				queueOutput(const std::string &msg);
		
		void				setNickname(const std::string &str);
		void				setUsername(const std::string &str);
		void				setRealname(const std::string &str);
		void				setRegistered(bool ans);
		void				setPass(bool ans);
		void				setClosing();
		
		const int			&getFD() const throw();
		const struct pollfd	&getPollFD() const throw();
		const std::string	&getIP() const throw();
		const std::string	&getNickname() const throw();
		const std::string	&getUsername() const throw();
		const std::string	&getRealname() const throw();
		
		bool 				hasPendingOutput() const throw();
		bool				hasPass() const throw();
		bool				isRegistered() const throw();
		bool				isClosing() const throw();

		static std::string 	newReply(replyco code, const std::string& target, const std::vector<std::string>& args);
};

#endif
