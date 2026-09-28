#ifndef CONFIG_HPP
# define CONFIG_HPP

# include <string>
# include <cctype>
# include <csignal>
# include <iostream>
# include <sstream>
# incldue <stdexcept>

class Config {
    private:
        int                 _port;
        std::string         _password;

                            Config();
                            Config(const Config &other) throw ;
        Config              &operator=(const Config &other) throw ;
                            ~Config() throw;

        void                parsePort(const std::string &s);
        void                parsePassword(const std::string &s);

        static void         initSignalHandler();
    
    public:
                            Config(int argc, char **argv);

        const std::string   &getPassword() throw const;
        int                 getPort() throw const;

}
#endif
