#ifndef COMMANDREGISTRY_HPP
# define COMMANDREGISTRY_HPP

# include "CommandRegistry.hpp"
# include "ACommand.hpp"
# include "Server.hpp"
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

class PassCommand : public ACommand {

    public:

        void execute(const Message &msg, Server &server, Socket &client);

};

class NickCommand : public ACommand {
    
    public:

        void execute(const Message &msg, Server &server, Socket &client);

};


class CapCommand : public ACommand {
    public:
        void execute(const Message &msg, Server &, Socket &client);
};

class UserCommand : public ACommand {
    public:
       void execute(const Message &msg, Server &, Socket &client);
};

class JoinCommand : public ACommand {
    public:
        void execute(const Message &msg, Server &server, Socket &client);
};

class PartCommand : public ACommand {
    public:
        void execute(const Message &msg, Server &server, Socket &client);
};

class PrivmsgCommand : public ACommand {
    public:
        void execute(const Message &msg, Server &server, Socket &client);
};

class QuitCommand : public ACommand {
    public:
        void execute(const Message &msg, Server &server, Socket &client);
};

class TopicCommand : public ACommand {
    public:
        void execute(const Message &msg, Server &server, Socket &client);
};

class ModeCommand : public ACommand {
    public:
       void execute(const Message &msg, Server &server, Socket &client);
};

class KickCommand : public ACommand {
    public:
        void execute(const Message &msg, Server &server, Socket &client);
};

class InviteCommand : public ACommand {
    public:
        void execute(const Message &msg, Server &server, Socket &client);
};


#endif
 