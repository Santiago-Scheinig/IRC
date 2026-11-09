#ifndef CLIENT_HPP
# define CLIENT_HPP

#include <string>
#include "Parser.hpp"

class Client {
	private:
		int _fd;
		std::string _ip;
		std::string _realname;
		std::string _nickname;
		std::string _username;
		std::string _outputBuffer;
		bool _isAuthenticated;
		bool _passworkOk;
		Parser _parser;
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
		const std::string &getRealName() const;
		const std::string &getNickName() const;
		const std::string &getUserName() const;
		const std::string &getIp() const;
		bool isAuthenticated() const;
		bool isPassworkOk() const;
		void setRealName(const std::string &realname);
		void setNickName(const std::string &nickname);
		void setUserName(const std::string &username);
		void setAuthenticated(bool isAuthenticated);
		void setPassworkOk(bool passworkOk);
};

#endif
