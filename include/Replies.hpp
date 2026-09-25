#ifndef REPLIES_HPP
# define REPLIES_HPP

#include <string>

// Owner: track B, extended by whoever needs a new numeric.
// One place to format RFC-numeric replies so command classes never
// hand-build "001 nick :Welcome..." strings themselves. Add a function
// here per numeric as you need it -- each addition is a one-line,
// low-conflict change.
namespace Replies
{
	std::string	welcome(const std::string& nickname);
	std::string	errNeedMoreParams(const std::string& nickname, const std::string& command);
	std::string	errNoSuchNick(const std::string& nickname, const std::string& target);
	std::string	errNicknameInUse(const std::string& nickname, const std::string& attempted);
	std::string	errNotRegistered(const std::string& nickname);
}

#endif
