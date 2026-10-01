#ifndef SERVER_HPP
# define SERVER_HPP

#include <vector>
#include <map>
#include <poll.h>
#include <string>

class Server {
	private:
		int _serverFd;
		std::vector<struct pollfd>_pollFds;
		std::map<int , Client*>;
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
