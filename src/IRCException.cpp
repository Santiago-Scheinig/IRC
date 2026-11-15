/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCException.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/16 18:23:21 by sscheini          #+#    #+#             */
/*   Updated: 2026/10/16 19:23:52 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRCException.hpp"

			IRCException::IRCException(ircErrco errco)
				:	_errco(errco),
					_msg(IRCFormatter::format(errco)) {}

			IRCException::IRCException(std::string category, std::string msg)
				:	_errco(CUSTOM),
					_msg(IRCFormatter::format(category, msg)) {}

			IRCException::~IRCException() throw() {}

const char	*IRCException::what() const throw() {
	return (_msg.c_str());
}

std::string	IRCFormatter::format(ircErrco errco) {
	std::stringstream	ss;

	switch (errco) {
		case SUCCESS:
			ss << INFO << "Success!" << END;
		case ERR_MEM:
			ss << ERROR << "[ERR_MEM]" << END << INFO << "Memory allocation failed" << END;
		case ERR_SCKT:
			ss << ERROR << "[ERR_SCKT]" << END << INFO << "Socket creation failed" << END;
		case R_INVCMD:
			ss << ERROR << "[R_INVCMD]" << END << INFO << "Unknown command" << END;
		case R_INVNICKNAME:
			ss << ERROR << "[R_INVNICKNAME]" << END << INFO << "Nickname already in use" << END;
		case R_INVCMDARGS:
			ss << ERROR << "[R_INVCMDARGS]" << END << INFO << "Missing/Invalid command parameters" << END;
		default:
			ss << ERROR << "[UNKNOWN]" << END << INFO << "Unrecognized error code" << END;
	}
	return (ss.str());
}

std::string	IRCFormatter::format(std::string category, std::string msg) {
	std::stringstream	ss;

	ss << ERROR << "[" << std::toUpper(category) << "]" << END << INFO << msg << END;
	return (ss.str());
}