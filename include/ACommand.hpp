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

#endif
