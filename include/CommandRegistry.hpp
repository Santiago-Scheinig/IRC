#ifndef COMMANDREGISTRY_HPP
# define COMMANDREGISTRY_HPP

# include "ACommand.hpp"
# include "Socket.hpp"
# include "Message.hpp"

# include <sstream>
# include <vector>
# include <cstdlib>
# include <climits>

class CommandRegistry {
    
    public:
        
        static void exe(const Message &msg, Server &server, Socket &client);
        static  std::vector<std::string> split(const std::string &str, char sep);
};

#endif
 