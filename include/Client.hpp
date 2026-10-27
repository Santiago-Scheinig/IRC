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
		Parser _parser;
		std::string _outputBuffer;
	public:
		Client(int fd, std::string ip);
		Client(const Client &copy);
		Client& operator=(const Client &copy);
		~Client();

		int getFd() const;
		void queueOutput(const std::string &msg);
		Parser::Parse_e parseData(const std::string &data, Message &msg);
		bool hasPendingOutput() const;
		std::string &outputBuffer();
};

#endif
