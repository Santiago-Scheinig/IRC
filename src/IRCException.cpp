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

			IRCException::IRCException(const std::string& message) {
    std::ostringstream	timestamp;

	auto 				now = std::chrono::system_clock::now();
	std::time_t 		currentTime = std::chrono::system_clock::to_time_t(now);

    timestamp << std::put_time(std::localtime(&currentTime), "%Y-%m-%d %H:%M:%S");
    _message = "[" + timestamp.str() + "] " + message;
}

			IRCException::~IRCException() {}

const char	*IRCException::what() const throw() {
	return (_msg.c_str());
}
