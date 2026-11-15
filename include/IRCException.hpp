/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCException.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/16 18:24:12 by sscheini          #+#    #+#             */
/*   Updated: 2026/10/16 19:16:27 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCEXCEPTION_HPP
# define IRCEXCEPTION_HPP

# define ERROR "\033[31m[ERROR]: "
# define INFO "\033[33m\t[INFO]: "
# define END ".\033[0m\n"

# include <sstream>
# include <stdexcept>
# include <string>

/**
 * SUCCESS,
 * ERR_MEM, Memory allocation error
 * ERR_SCKT, Socket creation error
 * R_INVCMD, Invalid IRC command (send Reply)
 * R_INVNICKNAME, Nickname already in use (send Reply)
 * R_INVCMDARGS, Missing command parameters (send Reply)
 */
enum ircErrco {
	CUSTOM = 0,
	SUCCESS,
	ERR_MEM,
	ERR_SCKT,
	R_INVCMD = 421,
	R_INVNICKNAME = 433,
	R_INVCMDARGS = 461,
};

/**
 * If errco != 421 && errco != 433 && errco != 461
 * Then its a system error, otherwise its a Reply 
 */
class IRCException : public std::exception {
	
	private:
		std::string			_msg;
		ircErrco			_errco;

	public:
							IRCException(ircErrco errco);
							IRCException(std::string category, std::string msg);
		virtual				~IRCException() throw();

		virtual const char	*what() const throw();
};

class IRCFormatter {

	private:
							IRCErrcoFormatter();
							~IRCErrcoFormatter();
	
	public:
		static std::string	format(ircErrco errco);
		static std::string	format(std::string category, std::string msg);
};

#endif