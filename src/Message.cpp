#include "Message.hpp"

Message::Message() : prefix(""), command(""), params()
{
}

Message::Message(const Message& other)
	: prefix(other.prefix), command(other.command), params(other.params)
{
}

Message& Message::operator=(const Message& other)
{
	if (this != &other)
	{
		prefix = other.prefix;
		command = other.command;
		params = other.params;
	}
	return (*this);
}

Message::~Message()
{
}
