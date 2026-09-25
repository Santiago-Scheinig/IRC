#include "Channel.hpp"
#include "Client.hpp"
#include <algorithm>

Channel::Channel(const std::string& name)
	: _name(name), _topic(""), _key(""), _members(), _operators(),
	  _inviteOnly(false), _topicRestricted(false), _userLimit(0)
{
}

Channel::~Channel()
{
}

const std::string&	Channel::getName() const
{
	return (_name);
}

void	Channel::addMember(Client* client)
{
	if (!isMember(client))
		_members.push_back(client);
}

void	Channel::removeMember(Client* client)
{
	_members.erase(std::remove(_members.begin(), _members.end(), client), _members.end());
	_operators.erase(std::remove(_operators.begin(), _operators.end(), client), _operators.end());
}

bool	Channel::isMember(Client* client) const
{
	return (std::find(_members.begin(), _members.end(), client) != _members.end());
}

void	Channel::addOperator(Client* client)
{
	if (!isOperator(client))
		_operators.push_back(client);
}

void	Channel::removeOperator(Client* client)
{
	_operators.erase(std::remove(_operators.begin(), _operators.end(), client), _operators.end());
}

bool	Channel::isOperator(Client* client) const
{
	return (std::find(_operators.begin(), _operators.end(), client) != _operators.end());
}

const std::string&	Channel::getTopic() const
{
	return (_topic);
}

void	Channel::setTopic(const std::string& topic)
{
	_topic = topic;
}

bool	Channel::isInviteOnly() const
{
	return (_inviteOnly);
}

void	Channel::setInviteOnly(bool value)
{
	_inviteOnly = value;
}

bool	Channel::isTopicRestricted() const
{
	return (_topicRestricted);
}

void	Channel::setTopicRestricted(bool value)
{
	_topicRestricted = value;
}

void	Channel::setKey(const std::string& key)
{
	_key = key;
}

bool	Channel::checkKey(const std::string& key) const
{
	return (_key.empty() || _key == key);
}

void	Channel::setUserLimit(size_t limit)
{
	_userLimit = limit;
}

bool	Channel::isFull() const
{
	return (_userLimit != 0 && _members.size() >= _userLimit);
}

// TODO(track C): write `message` to every member's outputBuffer except
// `exclude`, then let Server's poll loop flush it on POLLOUT.
void	Channel::broadcast(const std::string& message, Client* exclude)
{
	(void)message;
	(void)exclude;
}
