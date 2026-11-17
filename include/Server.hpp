#ifndef SERVER_HPP
# define SERVER_HPP

# include "Config.hpp"
# include "IRCException.hpp"
# include "Socket.hpp"

#	include <vector>
#	include <map>
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

		const int 				_port;
		const std::string		_password;

		std::map<int , Socket*>	_clientArr;
		struct pollfd			_pollFD;
		int						_spareFD;

								Server(const Server &copy);
								Server& operator=(const Server &copy);

		int 					initSocket() const;
		int 					initAddr(const int serverFD) const;
		void					initFDS(const int serverFD);

		void 					addNewClient();
		int						acceptClient(struct sockaddr_in &address);
		void 					manageClient(std::map<int, Socket*>::iterator client_it, short revents);
		void 					deleteClient(std::map<int, Socket*>::iterator client_it);
		
		void 					reapClosing();
	
	public:
	
								Server(Config init);
								~Server();

		void 					run();
		bool 					matchPassword(const std::string &pass) const throw();
};

#endif
