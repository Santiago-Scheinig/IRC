#ifndef ICOMMAND_HPP
# define ICOMMAND_HPP

# include "Socket.hpp"
# include "Server.hpp"
# include "Message.hpp"

/**
 * 
 */
class ICommand {

    public:
   
        virtual void    execute(const Message &msg, Server &server, Socket &client) = 0;
        virtual         ~ICommand() {}
};
 
#endif
