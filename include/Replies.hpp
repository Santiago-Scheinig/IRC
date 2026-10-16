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
		static std::string pong(const std::string &serverName, const std::string &token);
		static std::string numeric(const std::string &serverName, const std::string &code, const std::string &target, const std::string &text);
};

#endif