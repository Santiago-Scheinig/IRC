/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/16 18:23:30 by sscheini          #+#    #+#             */
/*   Updated: 2026/11/18 21:11:53 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

                    Config::Config() {
    throw SrvExc(SrvExc::CONFIG_LOAD_FAILED);
}

                    Config::Config(int argc, char **argv) {
    if (argc != 3)
        throw SrvExc(SrvExc::CONFIG_INVALID, "./irc <port> <password>");
    parsePort(argv[1]);
    parsePassword(argv[2]);
    initSignalHandler();
}

                    Config::Config(const Config &other){
    *this = other;
}

Config              &Config::operator=(const Config &other){
    if (this != &other) {
        _port = other._port;
        _pass = other._pass;
    }
    return (*this);
}

                    Config::~Config() throw() {}

void                Config::parsePort(const std::string &s) {
    std::size_t    size = s.size();

    if (s.empty())
        throw SrvExc(SrvExc::CONFIG_INVALID, "port is empty");
    if (size > 5)
        throw SrvExc(SrvExc::CONFIG_INVALID, "port values: [1 - 65535]");
    for (std::size_t i = 0; i < size; ++i)
        if (!std::isdigit(static_cast<unsigned char>(s[i])))
            throw SrvExc(SrvExc::CONFIG_INVALID, "non numeric character found");
    
    std::stringstream   stream(s);

    if (!(stream >> _port) || !stream.eof() || _port < 1 || _port > 65535)
        throw SrvExc(SrvExc::CONFIG_INVALID, "port values: [1 - 65535]");
}

void                Config::parsePassword(const std::string &s) {
    std::size_t    size = s.size();
    
    if (s.empty())
        throw SrvExc(SrvExc::CONFIG_INVALID, "password cannot be empty");
    for (std::size_t i = 0; i < size; ++i) {
        if (!std::isprint(static_cast<unsigned char>(s[i])))
            throw SrvExc(SrvExc::CONFIG_INVALID, "password cannot have non printable characters");
    }
	_pass = s;
}

void			    Config::signalHandler(int signum) {
	if (signum == SIGINT)
		g_shutdown = 1;
}

void                Config::initSignalHandler() {
	struct sigaction	sa;
	struct sigaction	sa_pipe;

	std::memset(&sa_pipe, 0, sizeof(sa_pipe));
	sa_pipe.sa_handler = SIG_IGN;
	sigemptyset(&sa_pipe.sa_mask);
	if (sigaction(SIGPIPE, &sa_pipe, NULL) == 1) {
		throw SrvExc(SrvExc::CONFIG_LOAD_FAILED, strerror(errno));
	}
	std::memset(&sa, 0, sizeof(sa));
	sa.sa_handler = signalHandler;
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIGINT, &sa, NULL) == 1) {
		throw SrvExc(SrvExc::CONFIG_LOAD_FAILED, strerror(errno));
	}
}

const std::string   &Config::getPassword() const throw() {
    return (_pass);
}

int                 Config::getPort() const throw() {
    return (_port);
}
