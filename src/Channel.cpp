#include "Channel.hpp"

Channel::Channel(const std::string &name) 
    : _name(name),
    _topic(""), 
    _key(""), 
    _limit(0), 
    _inviteOnly(false), 
    _topicRestricted(false) {
}

Channel::~Channel() {}

const std::string &Channel::getName() const {
    return _name;
}

void Channel::addMemeber(Client *client, bool isOperator) {
    if (client == NULL)
        return;
    std::map<Client *, bool>::iterator it = _members.find(client);
    if (it == _members.end())
        _members.insert(std::make_pair(client, isOperator));
    _invited.erase(client);
}

void Channel::addOperator(Client *client) {
    std::map<Client *, bool>::iterator it = _members.find(client);
    if (it == _members.end())
        return;
    it->second = true;
}

void Channel::removeMember(Client *client) {
    _members.erase(client);
    _invited.erase(client);
}

bool Channel::hasMember(const Client *client) const {
    return _members.find(const_cast<Client*>(client)) != _members.end();
}

bool Channel::isOperator(const Client *client) const {
    std::map<Client *, bool>::const_iterator it = _members.find(const_cast<Client*>(client));
    if (it == _members.end())
        return false;
    return it->second;
}

bool Channel::isEmpty() const {
    return _members.empty();
}

size_t Channel::memberCount() const {
    return _members.size();
}