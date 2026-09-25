#ifndef SERVER_HPP
# define SERVER_HPP

#include <string>
#include <vector>
#include <map>
#include <poll.h>

class Client;
class Channel;
class CommandRegistry;
class Config;

// Owner: track A (networking core).
// Single poll() loop over the listening socket + every client fd, per the
// 42 ft_irc constraints (no threads, no fork). This class should change
// rarely once the loop works -- new behaviour belongs in commands/channels,
// not here, which is what keeps this file out of most merge conflicts.
class Server
{
	public:
		explicit Server(const Config& config);
		~Server();

		void	run();

		// Exposed so ICommand implementations can act on shared state
		// (send to a client, look up/create a channel, etc).
		Client*		getClient(int fd);
		Channel*	getOrCreateChannel(const std::string& name);
		Channel*	findChannel(const std::string& name);
		void		removeChannelIfEmpty(const std::string& name);
		const std::string&	getPassword() const;

	private:
		Server();
		Server(const Server& other);
		Server& operator=(const Server& other);

		void	setupListenSocket(int port);
		void	pollLoop();
		void	acceptNewClient();
		void	handleClientReadable(int fd);
		void	handleClientWritable(int fd);
		void	disconnectClient(int fd);

		int								_listenFd;
		std::string						_password;
		bool							_running;
		std::vector<struct pollfd>		_pollFds;
		std::map<int, Client*>			_clients;
		std::map<std::string, Channel*>	_channels;
		CommandRegistry*				_commandRegistry;
};

#endif
