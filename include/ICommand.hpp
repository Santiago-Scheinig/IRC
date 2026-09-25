#ifndef ICOMMAND_HPP
# define ICOMMAND_HPP

class Server;
class Client;
class Message;

// Owner: shared contract, do not edit after M0 without a joint decision.
// Every IRC verb (JOIN, KICK, PRIVMSG...) is one concrete class implementing
// this, living in its own src/commands/*.cpp file. That is what keeps
// tickets atomic: two people adding two commands never touch the same file.
class ICommand
{
	public:
		virtual ~ICommand();
		virtual void execute(Server& server, Client& client, const Message& message) = 0;
};

#endif
