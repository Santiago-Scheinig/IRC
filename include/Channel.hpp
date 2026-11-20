#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include <string>
#include <map>
#include <set>
#include <cstddef>
#include "Socket.hpp"
class Channel {

	private:

		std::string 			    _name;
		std::map<Socket *, bool>    _members;
		std::string 				_topic;
		std::string 				_key;
		size_t 						_limit;
		bool 						_inviteOnly;
		bool 						_topicRestricted;
		std::set<Socket *> 			_invited;

	public:
									Channel(const std::string &name);
									~Channel();

		const std::string 			&getName() const;
		const std::string 			&getTopic() const;
		const std::string 			&getKey() const;
		size_t 						getLimit() const;
		bool 						getInviteOnly() const;

		void 						setTopic(const std::string &topic);
		void 						addMember(Socket *client, bool isOperator);
		void 						addOperator(Socket *client);
		void 						removeMember(Socket *client);
		bool 						hasMember(const Socket *client) const;
		bool 						isOperator(const Socket *client) const;
		bool 						isEmpty() const;
		size_t 						memberCount() const;
		void 						broadcast(const std::string &msg, Socket *exclude);
};

#endif
