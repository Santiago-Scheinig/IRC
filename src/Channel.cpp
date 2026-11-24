#include "Channel.hpp"

                    Channel::Channel(const std::string &name) 
                        :   _name(name),
                            _topic(""), 
                            _key(""), 
                            _limit(0), 
                            _inviteOnly(false), 
                            _topicRestricted(false) {}

                    Channel::~Channel() {}

const std::string   &Channel::getName() const {
    return (_name);
}

const std::string   &Channel::getTopic() const {
    return (_topic);
}

const std::string   &Channel::getKey() const {
    return (_key);
}

size_t              Channel::getLimit() const {
    return (_limit);
}

bool                Channel::getInviteOnly() const {
    return (_inviteOnly);
}

void                Channel::setTopic(const std::string &topic) {
    _topic = topic;
}

void                Channel::addMember(Socket *client, bool isOperator) {
    
    if (client == NULL)
        return;
    
    std::map<Socket *, bool>::iterator  it = _members.find(client);
    
    if (it == _members.end())
        _members.insert(std::make_pair(client, isOperator));
    _invited.erase(client);
}

void                Channel::addOperator(Socket *client) {
    std::map<Socket *, bool>::iterator  it = _members.find(client);
    
    if (it == _members.end())
        return;
    
    it->second = true;
}

void                Channel::removeMember(Socket *client) {
    _members.erase(client);
    _invited.erase(client);
}

bool                Channel::hasMember(const Socket *client) const {
    return (_members.find(const_cast<Socket*>(client)) != _members.end());
}

bool                Channel::isOperator(const Socket *client) const {
    std::map<Socket *, bool>::const_iterator    it = _members.find(const_cast<Socket *>(client));
    
    if (it == _members.end())
        return (false);
    
    return (it->second);
}

bool                Channel::isEmpty() const {
    return (_members.empty());
}

size_t              Channel::memberCount() const {
    return (_members.size());
}

void                Channel::broadcast(const std::string &msg, Socket *exclude) {
   
    for (std::map<Socket *, bool>::iterator it = _members.begin(); it != _members.end(); ++it) {
        
        if (it->first == exclude)
            continue;
        
        it->first->queueOutput(msg);
    }
}

void                Channel::removeOperator(Socket *client) {
    std::map<Socket *, bool>::iterator  it = _members.find(client);
    
    if (it != _members.end())
        it->second = false;
}

void                Channel::setInviteOnly(bool enabled) {
    _inviteOnly = enabled;
}

void                Channel::setTopicRestricted(bool enabled) {
    _topicRestricted = enabled;
}

void                Channel::setKey(const std::string &key) {
    _key = key;
}

void                Channel::setLimit(size_t limit) {
    _limit = limit;
}

bool                Channel::isTopicRestricted() const {
    return (_topicRestricted);
}

bool                Channel::isInvited(const Socket *client) const {
    return (_invited.find(const_cast<Socket *>(client)) != _invited.end());
}

void                Channel::invite(Socket *client) {
    if (client)
        _invited.insert(client);
}
