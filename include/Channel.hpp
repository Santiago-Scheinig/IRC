#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include <string>
#include <map>
#include <set>
#include <cstddef>

class Client;

class Channel {
    private:
        std::string _name;
        std::map<Client *, bool> _members;
        std::string _topic;
        std::string _key;
        size_t _limit;
        bool _inviteOnly;
        bool _topicRestricted;
        std::set<Client *> _invited;
    public:
        Channel(const std::string &name);
        ~Channel();
        const std::string &getName() const;
        void addMember(Client *client, bool isOperator);
        void addOperator(Client *client);
        void removeMember(Client *client);
        bool hasMember(const Client *client) const;
        bool isOperator(const Client *client) const;
        bool isEmpty() const;
        size_t memberCount() const;
};

#endif
