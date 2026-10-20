#ifndef CLIENT_HPP
# define CLIENT_HPP

#include <string>
#include "Parser.hpp"

class Client {
	private:
		int _fd;
		std::string _ip;
		std::string _nickname;
		std::string _username;
		bool _isAuthenticated;
		std::string _inputBuffer;
		std::string _outputBuffer;
	public:
		Client(int fd, std::string ip);
		Client(const Client &copy);
		Client& operator=(const Client &copy);
		~Client() {};

		int getFd() const;
		void Client::appendInput(const std::string &data);
		std::string extractMessage(const std::string &data, Message &message);
		void queueOutput(const std::string &msg);
};

#endif
