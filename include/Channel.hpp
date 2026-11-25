#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "Socket.hpp"

# include <string>
# include <map>
# include <set>

# include <cstddef>

/**
 * 
 */
class Channel {

	private:

		std::string 			    _name; 
		std::map<Socket *, bool>    _members;
		std::set<Socket *> 			_invited;
		
		std::string 				_topic;
		std::string 				_key;
		size_t 						_limit;
		bool 						_inviteOnly;
		bool 						_topicRestricted;

	public:
									Channel(const std::string &name);
									~Channel();

		const std::string 			&getName() const;
		const std::string 			&getTopic() const;
		const std::string 			&getKey() const;
		size_t 						getLimit() const;
		bool 						getInviteOnly() const;

		bool 						isTopicRestricted() const;
        bool 						isInvited(const Socket *client) const;
		bool 						isOperator(const Socket *client) const;
		bool 						isEmpty() const;

		void 						setInviteOnly(bool enabled);
        void 						setTopicRestricted(bool enabled);
        void 						setKey(const std::string &key);
        void 						setLimit(size_t limit);
		void 						setTopic(const std::string &topic);

		void 						addMember(Socket *client, bool isOperator);
		bool 						hasMember(const Socket *client) const;
		size_t 						memberCount() const;
		void 						removeMember(Socket *client);
		void 						invite(Socket *client);
		void 						broadcast(const std::string &msg, Socket *exclude);

		void 						addOperator(Socket *client);
        void 						removeOperator(Socket *client);

};

#endif
