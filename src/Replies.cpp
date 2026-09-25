#include "Replies.hpp"

// TODO(track B): format as real RFC 2812 numerics, e.g.
// ":server 001 nick :Welcome to the IRC network, nick" + "\r\n"
namespace Replies
{
	std::string	welcome(const std::string& nickname)
	{
		return (":ircserv 001 " + nickname + " :Welcome\r\n");
	}

	std::string	errNeedMoreParams(const std::string& nickname, const std::string& command)
	{
		return (":ircserv 461 " + nickname + " " + command + " :Not enough parameters\r\n");
	}

	std::string	errNoSuchNick(const std::string& nickname, const std::string& target)
	{
		return (":ircserv 401 " + nickname + " " + target + " :No such nick/channel\r\n");
	}

	std::string	errNicknameInUse(const std::string& nickname, const std::string& attempted)
	{
		return (":ircserv 433 " + nickname + " " + attempted + " :Nickname is already in use\r\n");
	}

	std::string	errNotRegistered(const std::string& nickname)
	{
		return (":ircserv 451 " + nickname + " :You have not registered\r\n");
	}
}
