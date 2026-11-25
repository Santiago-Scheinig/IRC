#ifndef ICOMMAND_HPP
# define ICOMMAND_HPP

# include "Socket.hpp"
# include "Message.hpp"

class Server;

/**
 * 
 */
class ICommand {

    public:
   
        virtual void    execute(const Message &msg, Server &server, Socket &client) = 0;
        virtual         ~ICommand() {}
};
 
#endif
