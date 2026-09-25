#ifndef MESSAGE_HPP
# define MESSAGE_HPP

#include <string>
#include <vector>

// Owner: track B (parsing).
// Plain data holder produced by Parser::parse(). No behaviour on purpose:
// keeping this a dumb struct-like class means command classes (track C)
// and the parser (track B) never fight over the same file.
class Message
{
	public:
		Message();
		Message(const Message& other);
		Message& operator=(const Message& other);
		~Message();

		std::string					prefix;		// optional, rarely sent by clients
		std::string					command;	// e.g. "JOIN", "PRIVMSG"
		std::vector<std::string>	params;		// all params, trailing included as last
};

#endif
