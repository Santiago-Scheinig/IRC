#include "ACommand.hpp"

void            ACommand::numeric(Socket &client, int code, const std::string &middle, const std::string &message) {
    std::ostringstream out;

    out << ":" << SERVER_NAME << " " << std::setw(3) << std::setfill('0') << code << " " << client.getNickname();

    if (!middle.empty())
        out << " " << middle;

    if (!message.empty())
        out << " :" << message;

    out << "\r\n";
    client.queueOutput(out.str());
}

std::string     ACommand::prefix(const Socket &client) {
    return (":" + client.getNickname() + "!" + client.getUsername() + "@" + client.getIP());
}

bool            ACommand::registered(Socket &client) {
    if (client.isRegistered())
        return true;


    numeric(client, 451, "", "You have not registered");
    return (false);
}

void            ACommand::welcome(Socket &client) {
    numeric(client, 1, "", "Welcome to the Internet Relay Network " + client.getNickname());
    numeric(client, 2, "", "Your host is " SERVER_NAME);
    numeric(client, 3, "", "This server is running ft_irc");
    numeric(client, 4, "", std::string(SERVER_NAME) + " ft_irc");
}
