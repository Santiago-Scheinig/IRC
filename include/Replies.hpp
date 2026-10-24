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
		static std::string pong(const std::string &serverName, const std::string &token);
		static std::string welcome(const std::string &serverName, const std::string &nickname);
		static std::string numeric(const std::string &serverName, const std::string &code, const std::string &target, const std::string &text);
};

#endif