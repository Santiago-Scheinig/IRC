#ifndef SOCKET_HPP
# define SOCKET_HPP

#include "Parser.hpp"

class Socket {

	private:

		const std::string	_ip; 
		std::string			_nickname, _username, _realname;
		bool	 			_hasPass, _registered, _closing;
		
		std::string			_buffIn, _buffOut;
		struct pollfd		_pollFD;
		
							Socket(const Socket &other);
		Socket				&operator=(const Socket &other);

	public:

							Socket(int fd, const std::string &ip);
							~Socket();

		bool				dataRead();
		bool				dataSend();
		bool				nextLine(std::string &line);
		void 				queueOutput(const std::string &msg);
		
		void				setNickname(const std::string &str);
		void				setUsername(const std::string &str);
		void				setRealname(const std::string &str);
		void				setRegistered(bool ans);
		void				setPass(bool ans);
		void				setClosing();
		
		const std::string	getNickname() const throw();
		const std::string	getUsername() const throw();
		const std::string	getRealname() const throw();
		const struct pollfd	getPollFD() const throw();
		const int			getFD() const throw();
		const int			getIP() const throw();
		
		bool 				hasPendingOutput() const throw();
		bool				hasPass() const throw();
		bool				isRegistered() const throw();
		bool				isClosing() const throw();
};

#endif
