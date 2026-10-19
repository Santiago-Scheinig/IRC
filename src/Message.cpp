#include "../include/Message.hpp"

Message::Message() : _prefix(), _cmd(), _params(), _trailing(),
	_hasTrailing(false)
	
{}

/**
 * This constructor is using a lot of parameters, we could instead use a typedef to define a group of all the
 * needed information. Clearer to see, and easier to modify if we later need more information on the Message
 */
Message::Message(msgInfo info)
Message::Message(const std::string &prefix, const std::string &cmd, const std::vector<std::string> &params, const std::string &trailing, bool hasTrailing) : _prefix(prefix), _cmd(cmd), _params(params),
	_trailing(trailing), _hasTrailing(hasTrailing)
{}

/**
 * We are missing the other OCF functions, its normative.
 */

Message::~Message()
{}

const std::string &Message::getPrefix() const
{
	return _prefix;
}

const std::string &Message::getCommand() const
{
	return _cmd;
}

const std::vector<std::string> &Message::getParams() const
{
	return _params;
}

const std::string &Message::getTrailing() const
{
	return _trailing;
}

bool Message::hasTrailing() const
{
	return _hasTrailing;
}
