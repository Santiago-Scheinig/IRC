#include "ACommand.hpp"

std::string     ACommand::prefix(const Socket &client) {
    return (":" + client.getNickname() + "!" + client.getUsername() + "@" + client.getIP());
}
