/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/16 18:23:47 by sscheini          #+#    #+#             */
/*   Updated: 2026/11/18 21:08:42 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
# define CONFIG_HPP

# include "ServerException.hpp"

# include <string>
# include <sstream>
# include <iostream>

# include <cctype>
# include <cerrno>
# include <cstring>
# include <csignal>
 
extern volatile	sig_atomic_t	g_shutdown;

/**
 * 
 */
class Config {
   
    private:
     
        int                 _port;
        std::string         _pass;

                            Config();
        Config              &operator=(const Config &other);

        void                parsePort(const std::string &s);
        void                parsePassword(const std::string &s);

        static void         signalHandler(int signum);
		static void         initSignalHandler();
    
    public:
                            Config(const Config &other);
                            Config(int argc, char **argv);
                            ~Config() throw();

        const std::string   &getPassword() const throw();
        int                 getPort() const throw();

};

#endif
