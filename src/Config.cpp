/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/16 18:23:30 by sscheini          #+#    #+#             */
/*   Updated: 2026/11/13 18:10:14 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

                    Config::Config() {
    throw IRCException::IRCException("INV_CONSTRUCTOR", "Cant use default constructor for Config class");
}

                    Config::Config(int argc, char **argv) {
    if (argc != 3)
        throw IRCException::IRCException("INV_CONFIG", "usage: ./ircserv <port> <password>");
    parsePort(argv[1]);
    parsePassword(argv[2]);
    initSignalHandler();
}

                    Config::Config(const Config &other){
    *this = other;
}

Config              &Config::operator=(const Config &other){
    if (this != other) {
        _port = other._port;
        _pass = other._pass;
    }
    return (*this);
}

                    Config::~Config() throw() {}

void                Config::parsePort(const std::string &s) {
    int size = s.size();

    if (s.empty())
        throw IRCException::IRCException("inv_port", "port is empty");
    if (size > 5)
        throw IRCException::IRCException("inv_port", "port values: [1 - 65535]");
    for (std::size_t i = 0; i < size; ++i)
        if (!std::isdigit(static_cast<unsigned char>(s[i])))
            throw IRCException::IRCException("inv_port", "non numeric character found");
    
    std::stringstream   stream(s);

    if (!(stream >> _port) || !stream.eof() || _port < 1 || _port > 65535)
        throw IRCException::IRCException("inv_port", "port values: [1 - 65535]");
}

void                Config::parsePassword(const std::string &s) {
    int size = s.size();
    
    if (s.empty())
        throw IRCException::IRCException("inv_password", "password cannot be empty");
    for (std::size_t i = 0; i < size; i++)
        if (!std::isprint(static_cast<unsigned char>(s[i])))
            throw IRCException::IRCException("inv_password", "password cannot have non printable characters");
	_pass = s;
}

void			signalHandler(int signum) {
	if (signum == SIGINT)
		g_shutdown = 1;
}

void                Config::initSignalHandler() {
	struct sigaction	sa;
	struct sigaction	sa_pipe;

	std::memset(&sa_pipe, 0, sizeof(sa_pipe));
	sa_pipe.handler = SIG_IGN;
	sigemptyset(&sa_pipe.sa_mask);
	if (sigaction(SIGPIPE, &sa_pipe, NULL) == 1) {
		throw std::exception(); //Failed to ignore SIGPIPE
	}
	std::memset(&sa, 0, sizeof(sa));
	sa.sa_handler = signalHandler;
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIGINT, &sa, NULL) == 1) {
		throw std::exception(); //Failed to register SIGINT handler
	}
}

const std::string   Config::getPassword() throw() const {
    return (_pass);
}

int                 Config::getPort() throw() const {
    return (_port);
}
