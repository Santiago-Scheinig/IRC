#ifndef COMMANDREGISTRY_HPP
# define COMMANDREGISTRY_HPP

#include <map>
#include <string>

class ICommand;
class Server;
class Client;
class Message;

// Owner: track B (parsing/dispatch).
// Holds one ICommand* per verb. registerCommand() calls happen once at
// startup (likely in a single setup function); dispatch() is the only
// thing called per incoming line. Adding a new command means adding one
// registerCommand() line here plus one new file elsewhere -- not editing
// existing command logic.
class CommandRegistry
{
	public:
		CommandRegistry();
		~CommandRegistry();

		void	registerCommand(const std::string& name, ICommand* command);
		void	dispatch(Server& server, Client& client, const Message& message);

	private:
		CommandRegistry(const CommandRegistry& other);
		CommandRegistry& operator=(const CommandRegistry& other);

		std::map<std::string, ICommand*>	_commands;
};

#endif
