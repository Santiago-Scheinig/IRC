#ifndef ACOMMAND_HPP
# define ACOMMAND_HPP

# include "ICommand.hpp"
# include "Server.hpp"

# include <sstream>
# include <iomanip>

class ACommand : public ICommand {

    protected:

        static std::string  prefix(const Socket &client);

    public:

        virtual ~ACommand() {}
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
