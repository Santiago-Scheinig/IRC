#include "../include/Replies.hpp"

Replies::Replies()
{
}

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

std::string Replies::pong(const std::string &serverName, const std::string &token)
{
	return ":" + serverName + " PONG " + serverName + " :" + token + "\r\n";
}

std::string Replies::welcome(const std::string &serverName, const std::string &nickname)
{
	return numeric(serverName, "001", nickname, "Welcome to IRC server");
}


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