#ifndef ACOMMAND_HPP
# define ACOMMAND_HPP

# include "ICommand.hpp"
# include "Server.hpp"

# include <sstream>
# include <iomanip>

class ACommand : public ICommand {

    protected:

        static void         welcome(Socket &client);
        static bool         registered(Socket &client);
        static void         numeric(Socket &client, int code, const std::string &middle, const std::string &message);
        static std::string  prefix(const Socket &client);

    public:

        virtual ~ACommand() {}
};

#endif
