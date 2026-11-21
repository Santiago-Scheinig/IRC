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
    std::time_t currentTime = std::time(NULL);
    std::tm* localTime = std::localtime(&currentTime);

    char buffer[20];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localTime);

    _message = "[" + std::string(buffer) + "]" + message;
}

			IRCException::~IRCException() throw() {}

const char	*IRCException::what() const throw() {
	return (_message.c_str());
}

