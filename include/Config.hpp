/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/16 18:23:47 by sscheini          #+#    #+#             */
/*   Updated: 2026/11/11 14:31:22 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
# define CONFIG_HPP

# include <string>
# include <cctype>
# include <csignal>
# include <iostream>
# include <sstream>
# include "IRCException.hpp"

class Config {
    private:
        int                 _port;
        std::string         _pass;

                            Config();
                            Config(const Config &other);
        Config              &operator=(const Config &other);
                            ~Config() throw();

        void                parsePort(const std::string &s);
        void                parsePassword(const std::string &s);

        static void         initSignalHandler();
    
    public:
                            Config(int argc, char **argv);

        const std::string   &getPassword() throw() const;
        int                 getPort() throw() const;

};

#endif
