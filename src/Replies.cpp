#include "../include/Replies.hpp"

Replies::Replies()
{
}
// REGISTER REPLIES
std::string Replies::capLs(const std::string &target, const std::string &capabilities)
{
	return "CAP " + target + " LS :" + capabilities + "\r\n";
}

std::string Replies::capList(const std::string &target, const std::string &capabilities)
{
	return "CAP " + target + " LIST :" + capabilities + "\r\n";
}

std::string Replies::capAck(const std::string &target, const std::string &capabilities)
{
	return "CAP " + target + " ACK :" + capabilities + "\r\n";
}

std::string Replies::capNak(const std::string &target, const std::string &capabilities)
{
	return "CAP " + target + " NAK :" + capabilities + "\r\n";
}

std::string Replies::nick(const std::string &prefix, const std::string &nickname)
{
	return ":" + prefix + " NICK " + nickname + "\r\n";
}

std::string Replies::welcome(const std::string &serverName, const std::string &nickname)
{
	return numeric(serverName, "001", nickname, "Welcome to IRC server");
}

// MESSAGE REPLIES
std::string Replies::pong(const std::string &serverName, const std::string &token)
{
	return ":" + serverName + " PONG " + serverName + " :" + token + "\r\n";
}

std::string Replies::privmsg(const std::string &prefix, const std::string &target, const std::string &text)
{
	return ":" + prefix + " PRIVMSG " + target + " :"
		+ text + "\r\n";
}

std::string Replies::quit(const std::string &prefix, const std::string &reason)
{
	return ":" + prefix + " QUIT :" + reason + "\r\n";
}

// CHANELS REPLIES
std::string Replies::join(const std::string &prefix, const std::string &channel)
{
	return ":" + prefix + " JOIN :" + channel + "\r\n";
}

std::string Replies::part(const std::string &prefix, const std::string &channel, const std::string &reason)
{
	std::string reply = ":" + prefix + " PART " + channel;

	if (!reason.empty())
		reply += " :" + reason;
	return reply + "\r\n";
}

std::string Replies::topic(const std::string &prefix, const std::string &channel, const std::string &text)
{
	return ":" + prefix + " TOPIC " + channel + " :" + text + "\r\n";
}

std::string Replies::invite(const std::string &prefix, const std::string &nickname, const std::string &channel)
{
	return ":" + prefix + " INVITE " + nickname + " " + channel + "\r\n";
}

std::string Replies::kick(const std::string &prefix, const std::string &channel, const std::string &nickname, const std::string &reason)
{
	std::string reply = ":" + prefix + " KICK " + channel + " " + nickname;

	if (!reason.empty())
		reply += " :" + reason;
	return reply + "\r\n";
}

std::string Replies::mode(const std::string &prefix, const std::string &target, const std::string &modes, const std::string &parameters)
{
	std::string reply = ":" + prefix + " MODE " + target + " " + modes;

	if (!parameters.empty())
		reply += " " + parameters;
	return reply + "\r\n";
}

// NUMERIC ERROR REPLIES
std::string Replies::numeric(const std::string &serverName, const std::string &code, const std::string &target, const std::string &text)
{
	return ":" + serverName + " " + code + " " + target + " :" + text + "\r\n";
}

/*
1. CAP
2. PASS
3. NICK
4. USER
5. 001-004 de registro
6. JOIN
7. PART
8. PRIVMSG
9. QUIT
10. TOPIC
11. INVITE
12. KICK
13. MODE
*/