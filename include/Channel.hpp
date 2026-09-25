#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include <string>
#include <vector>

class Client;

// Owner: track C (channels/messaging).
// Owns its own membership and broadcasting -- commands ask the Channel to
// do things rather than reaching into its internals, so channel-mode
// logic (i/t/k/o/l) stays in one place instead of leaking into every
// command that touches a channel.
class Channel
{
	public:
		explicit Channel(const std::string& name);
		~Channel();

		const std::string&	getName() const;

		void	addMember(Client* client);
		void	removeMember(Client* client);
		bool	isMember(Client* client) const;

		void	addOperator(Client* client);
		void	removeOperator(Client* client);
		bool	isOperator(Client* client) const;

		const std::string&	getTopic() const;
		void	setTopic(const std::string& topic);

		bool	isInviteOnly() const;
		void	setInviteOnly(bool value);

		bool	isTopicRestricted() const;
		void	setTopicRestricted(bool value);

		void	setKey(const std::string& key);
		bool	checkKey(const std::string& key) const;

		void	setUserLimit(size_t limit);
		bool	isFull() const;

		// Sends `message` to every member except `exclude` (typically the sender).
		void	broadcast(const std::string& message, Client* exclude);

	private:
		Channel();
		Channel(const Channel& other);
		Channel& operator=(const Channel& other);

		std::string				_name;
		std::string				_topic;
		std::string				_key;
		std::vector<Client*>	_members;
		std::vector<Client*>	_operators;
		bool					_inviteOnly;
		bool					_topicRestricted;
		size_t					_userLimit;
};

#endif
