#ifndef SERVER_HPP
# define SERVER_HPP

# ifndef SERVER_NAME
#  define SERVER_NAME "irc.42.com"
# endif

# include "Config.hpp"
# include "Socket.hpp"
# include "Channel.hpp"
# include "ServerException.hpp"
# include "ClientException.hpp"
# include "CommandRegistry.hpp"


# include <map> 
# include <vector>
# include <cerrno>
# include <cstring>
# include <fstream>

# include <poll.h>
# include <fcntl.h>
# include <unistd.h>
# include <sys/socket.h>
# include <sys/types.h>
# include <arpa/inet.h>
# include <netinet/in.h>

/**
 * 
 */
class Server {

	private:

		struct pollfd						_pollFD;
		int									_spareFD;

		const int 							_port;
		const std::string					_password;

		std::map<int , Socket*>				_clientArr;
		std::map<std::string, Channel *>	_channelArr;

											Server(const Server &copy);
											Server& operator=(const Server &copy);

		int 								initSocket() const;
		int 								initAddr(const int serverFD) const;
		void								initFDS(const int serverFD);

		void 								addNewClient();
		int									acceptClient(struct sockaddr_in &address);
		void 								manageClient(std::map<int, Socket*>::iterator client_it, short revents);
		void 								deleteClient(std::map<int, Socket*>::iterator client_it);
		
		void 								reapClosing();
	
	public:
	
											Server(Config init);
											~Server();

		void 								run();

		bool 								matchPassword(const std::string &pass) const throw();

		Channel 							*getChannel(const std::string &name) const;
		Channel 							*getOrCreate(const std::string &name);

		Socket 								*findClientByNick(const std::string &nick) const;

		void 								removeChannel(const std::string &name);
		void 								removeClientFromChannels(Socket *client);
};

#endif
