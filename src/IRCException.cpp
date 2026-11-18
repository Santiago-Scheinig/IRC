/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCException.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/16 18:23:21 by sscheini          #+#    #+#             */
/*   Updated: 2026/11/18 21:12:00 by eduaserr         ###   ########.fr       */
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
//Should? add a break in every case?
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
	std::string upperCase = category;

	for (std::size_t i = 0; i < upperCase.size(); ++i)
		upperCase[i] = std::toupper(upperCase[i]);
	ss << ERROR << "[" << upperCase << "]" << END << INFO << msg << END;
	return (ss.str());
}