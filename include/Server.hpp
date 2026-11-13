#ifndef SERVER_HPP
# define SERVER_HPP

# include "Config.hpp"
# include "IRCException.hpp"
# include "Socket.hpp"

#	include <vector>
#	include <map>
#	include <string>
#	include <cerrno>
#	include <cstring>
#	include <fstream>
#	include <poll.h>
#	include <fcntl.h>
#	include <unistd.h>
#	include <sys/socket.h>
#	include <sys/types.h>
#	include <arpa/inet.h>
#	include <netinet/in.h>

class Server {
	private:

		/* ----------------------------   SERVER INFO   ---------------------------- */
		
		/**
		 * htons expects uint16_t
		 * is this relevant for port?
		 */
		const int 				_port;
		const std::string		_password;
		
		/* ---------------------------- SERVER CONTENTS ---------------------------- */
		
		std::map<int , Socket*>	_clientArr;
		struct pollfd			_pollFD;
		int						_spareFD;

		/* ----------------------------       OCF       ---------------------------- */
		
						Server(const Server &copy);
						Server& operator=(const Server &copy);

		/* ----------------------------   INITIALIZERS  ---------------------------- */
		
		void 			initAddr(const int fd) const;
		void 			initSocket(const int fd) const;

		struct pollfd	newPoll(const int fd) const throw();
		void 			newSocket();
		
		/* ----------------------------     MANAGMENT   ---------------------------- */
		
		void 			manageSocket(std::map<int, Socket*>::iterator client_it, short revents);
		void 			deleteSocket(std::map<int, Socket*>::iterator client_it);
	
	public:
	
		/* ----------------------------       OCF       ---------------------------- */

						Server(Config type);
						~Server();

		/* ----------------------------     MANAGMENT   ---------------------------- */

		void 			run();

		bool 			matchPassword(const std::string &pass) const throw();
};

#endif
