#ifndef REPLIES_HPP
# define REPLIES_HPP

 #include <string>

class Replies
{
	private:
		Replies();
		Replies(const Replies &other);
		Replies &operator=(const Replies &other);

public:
		static std::string capLs(const std::string &target, const std::string &capabilities);
		static std::string capList(const std::string &target, const std::string &capabilities);
		static std::string capAck(const std::string &target, const std::string &capabilities);
		static std::string capNak(const std::string &target, const std::string &capabilities);
		static std::string nick(const std::string &prefix, const std::string &nickname);
		static std::string welcome(const std::string &serverName, const std::string &nickname);
		
		static std::string pong(const std::string &serverName, const std::string &token);
		static std::string quit(const std::string &prefix, const std::string &reason);
		static std::string privmsg(const std::string &prefix, const std::string &target, const std::string &text);

		static std::string join(const std::string &prefix, const std::string &channel);
		static std::string part(const std::string &prefix, const std::string &channel, const std::string &reason);
		static std::string topic(const std::string &prefix, const std::string &channel, const std::string &text);
		static std::string invite(const std::string &prefix, const std::string &nickname, const std::string &channel);
		static std::string kick(const std::string &prefix, const std::string &channel, const std::string &nickname, const std::string &reason);
		static std::string mode(const std::string &prefix, const std::string &target, const std::string &modes, const std::string &parameters);

		static std::string numeric(const std::string &serverName, const std::string &code, const std::string &target, const std::string &text);
};

#endif