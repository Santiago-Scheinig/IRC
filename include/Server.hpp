#ifndef SERVER_HPP
# define SERVER_HPP

#include <vector>
#include <map>
#include <poll.h>
#include <string>
#include "Client.hpp"

class Server {
	private:
		int _port;
		std::string _pass;
		int _serverFd;
		std::vector<struct pollfd>_pollFds;
		std::map <int , Client*> _clients;
	public:
		Server(int port, std::string pass);
		Server(const Server &copy);
		Server& operator=(const Server &copy);
		~Server();

		void initServer();
		void run();
		void acceptNewClient();
		void receiveData(int fd);
		void sendData(int fd);
};

#endif
