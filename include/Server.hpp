#ifndef SERVER_HPP
# define SERVER_HPP

#include <csignal>
# include <vector>
# include <map>
# include <string>
# include <poll.h>
# include <stdexcept>
# include <iostream>
# include <cstring>
# include <unistd.h>
# include <fcntl.h>
# include <arpa/inet.h>
# include <sys/socket.h>
# include <netinet/in.h>

#include "Message.hpp"
#include "Parser.hpp"


extern volatile sig_atomic_t	g_running;

class Client;
class Channel;

class	Server
{
	private:
		Server();
		Server(const Server &other);
		Server &operator=(const Server &other);

		int					_port;
		std::string			_password;
		int					_serverFd;
		std::vector<struct pollfd> _pollfds;

		std::map<int, Client *> _clients;					//Server crea Client  -> Server destruye Client
		std::map<std::string, Channel *> _channels;			//Server crea Channel -> Server destruye Channel

		void _initSocket();

		void _acceptClient();
		void _handleClient(int fd);
		void _flushClientOutput(int fd);
		void _removeClient(int fd);

	public:
		Server(int port, const std::string &password);
		~Server();

		void	run();

		// * Getters * //
		const std::string&	getPassword() const {return _password;}
};

#endif