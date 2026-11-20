/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/16 18:23:47 by sscheini          #+#    #+#             */
/*   Updated: 2026/11/13 18:10:21 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
# define CONFIG_HPP

# include <string>
# include <cctype>
# include <cstring>
# include <csignal>
# include <iostream>
# include <sstream>
# include "IRCException.hpp"

extern volatile	sig_atomic_t	g_shutdown;

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
