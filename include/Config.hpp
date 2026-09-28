#ifndef CONFIG_HPP
# define CONFIG_HPP

# include <csignal>
# include <string>

class Config {
    private:
        int                 _port;
        std::string         _password;

                            Config();
                            Config(const Config &other);
        Config              &operator=(const Config &other);

        bool                parsePort(const std::string &s);
        bool                parsePassword(const std::string &s);

    public:
                            Config(int argc, char **argv);
                            ~Config();

        const std::string   &getPassword() const;
        int                 getPort() const;

        static void         initSignalHandler();
}
#endif
