#ifndef CLIENT_HPP
# define CLIENT_HPP

#include <string>
#include "Parser.hpp"

class Socket {

	private:

		const std::string							_ip;
		const std::string							_realname;
		const std::string							_username;
		const bool 									_isAuthenticated;
		const bool 									_matchPassword;
	
		std::string									_nickname;
		struct pollfd								_pollFD;
		char										*_buffIn[512];
		char										*_buffOut[512];

	public:
		Socket(int fd, std::string ip);
		Socket(const Socket &copy);
		Socket& operator=(const Socket &copy);
		~Socket();

		void queueOutput(const std::string &msg);
		bool hasPendingOutput() const;
};

#endif
