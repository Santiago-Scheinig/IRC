#include "Config.hpp"

                    Config::Config() {
    throw //std::exception("Cant use default constructor for Config class");
}

                    Config::Config(int argc, char **argv) {
    if (argc != 3)
        throw std::invalid_argument("usage: ./ircserv <port> <password>");
    parsePort(argv[1]);
    parsePassword(argv[2]);
    initSignalHandler();
}

//If operator= can throw an exception. Remove throw
                    Config::Config(const Config &other) throw {
    *this = other;
}

//Does copy string / int can throw an exception?
//If so remove throw
Config              &Config::operator=(const Config &other) throw {
    if (&this != other) {
        _port = other._port;
        _password = other._password;
    }
    return *this;
}

                    Config::~Config() throw {}

void                Config::parsePort(const std::string &s) {
    int size = s.size();

    if (s.empty())
        throw std::invalid_argument("ERROR: invalid port - port is empty.");
    if (size > 5)
        throw std::invalid_argument("ERROR: invalid port - port values: [1 - 65535]."); //Colur Error red. I could use Macros or create a shared exception so we only need to specify the message.
    for (std::size_t i = 0; i < size; ++i)
        if (!std::isdigit(static_cast<unsigned char>(s[i])))
            throw std::invalid_argument("ERROR: invalid port - non numeric character found.");
    
    std::stringstream   stream(s);

    if (!(stream >> _port) || !stream.eof() || _port < 1 || _port > 65535)
        throw std::invalid_argument("ERROR: invalid port - port values: [1 - 65535].");
}

void                Config::parsePassword(const std::string &s) {
    int size = s.size();
    
    if (s.empty())
        throw std::invalid_argument("ERROR: invalid password - empty value.");
    for (std::size_t i = 0; i < size; i++)
        if (!std::isprint(static_cast<unsigned char>(s[i])))
            throw std::invalid_argument("ERROR: invalid password - non printable character found.");
}

void                Config::initSignalHandler() {
    //What are we handling? And why?
}

const std::string   Config::getPassword() throw const {
    return _password;
}

int                 Config::getPort() throw const {
    return _port;
}
