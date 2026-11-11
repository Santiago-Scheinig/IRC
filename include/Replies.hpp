#ifndef REPLIES_HPP
# define REPLIES_HPP

/**
 * Should use IRCErrcoFormatter to generate Replies with the respective
 * generic format.
 */

 #include <string>

class Replies
{
	private:
		Replies();
		Replies(const Replies &other);
		Replies &operator=(const Replies &other);

public:
		// CAPABILITIES: Capability negotiation (CAP LS, LIST, ACK, NAK)
		static std::string	capLs(const std::string &target, const std::string &capabilities);
		static std::string	capList(const std::string &target, const std::string &capabilities);
		static std::string	capAck(const std::string &target, const std::string &capabilities);
		static std::string	capNak(const std::string &target, const std::string &capabilities);
		// CONNECTION: Connection and client registration
		static std::string	pong(const std::string &serverName, const std::string &token);
		static std::string	welcome(const std::string &serverName, const std::string &nickname);
		// NICKNAME
		static std::string	nick(const std::string &prefix, const std::string &nickname);
		// CHANNEL MEMBERSHIP: Joining, leaving and listing channel members.
		static std::string	join(const std::string &prefix, const std::string &channel);
		static std::string	namesReply(const std::string &serverName, const std::string &target, const std::string &channel, const std::string &names);
		static std::string	endOfNames(const std::string &serverName, const std::string &target, const std::string &channel);
		static std::string	part(const std::string &prefix, const std::string &channel, const std::string &reason);
		static std::string	quit(const std::string &prefix, const std::string &reason);
		// MESSAGING: Private messages between clients or channels.
		static std::string	privmsg(const std::string &prefix, const std::string &target, const std::string &text);
		// TOPIC: Channel topic changes and queries.
		static std::string	topic(const std::string &prefix, const std::string &channel, const std::string &text);
		static std::string	topicReply(const std::string &serverName, const std::string &target, const std::string &channel, const std::string &topic);
		static std::string	noTopic(const std::string &serverName, const std::string &target, const std::string &channel);
		// CHANNEL MANAGEMENT: Invitations, kicks and mode changes.
		static std::string	invite(const std::string &prefix, const std::string &nickname, const std::string &channel);
		static std::string	kick(const std::string &prefix, const std::string &channel, const std::string &nickname, const std::string &reason);
		static std::string	mode(const std::string &prefix, const std::string &target, const std::string &modes, const std::string &parameters);
		// NUMERIC REPLIES: Standard IRC numeric responses and error messages.
		static std::string	numeric(const std::string &serverName, const std::string &code, const std::string &target, const std::string &text);
};

#endif
