#ifndef SERVER_HPP
# define SERVER_HPP

#include <vector>
#include <map>
#include <poll.h>
#include <string>
#include <unistd.h>
#include <fstream>
#include "Client.hpp"
#include <sys/socket.h>
#include <sys/types.h>
#include <fcntl.h>
#include <cstring>
#include <stdexcept>
#include <netinet/in.h>
#include <errno>

class Server {
	private:
		int _port;
		std::string _pass;
		int _serverFd;
		std::vector<struct pollfd>_pollFds;
		std::map <int , Client*> _clients;
		Server(const Server &copy);
		Server& operator=(const Server &copy);
	public:
		Server(int port, std::string pass);
		~Server();

		void initServer();
		void deleteFd(size_t i);
		void run();
		void acceptNewClient();
		bool receiveData(int fd);
		void sendData(int fd);
		void enableWrite(int fd);
};

#endif
