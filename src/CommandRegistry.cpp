#include "CommandRegistry.hpp"
#include "ICommand.hpp"
#include "Message.hpp"

CommandRegistry::CommandRegistry() : _commands()
{
}

CommandRegistry::~CommandRegistry()
{
	std::map<std::string, ICommand*>::iterator it = _commands.begin();
	for (; it != _commands.end(); ++it)
		delete it->second;
}

void	CommandRegistry::registerCommand(const std::string& name, ICommand* command)
{
	_commands[name] = command;
}

// TODO(track B): look up message.command in _commands, call execute() if
// found, otherwise send ERR_UNKNOWNCOMMAND back to the client.
void	CommandRegistry::dispatch(Server& server, Client& client, const Message& message)
{
	(void)server;
	(void)client;
	(void)message;
}
