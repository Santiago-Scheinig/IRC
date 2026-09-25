#ifndef CONFIG_HPP
# define CONFIG_HPP

#include <string>

// Owner: track A (networking bootstrap).
// Validates argv (./ircserv <port> <password>) once, at startup, so Server
// never has to check argc/argv itself.
class Config
{
	public:
		Config(int argc, char** argv);
		~Config();
		Config(const Config& other);
		Config& operator=(const Config& other);

		int					getPort() const;
		const std::string&	getPassword() const;

	private:
		Config();

		int			_port;
		std::string	_password;
};

#endif
