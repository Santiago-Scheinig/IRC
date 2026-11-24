#include "CommandRegistry.hpp"

/* ------------------------------------------------------------------------ */

/* ----------------------            CAP             ---------------------- */

/* ------------------------------------------------------------------------ */

void CapCommand::execute(const Message &msg, Server &, Socket &client) {
        const std::vector<std::string>  &p = msg.getParams();
        std::string                     target = "*";

        if (p.empty()) {
            client.queueOutput(client.newReply(Socket::ERR_NEEDMOREPARAMS, "(usage) CAP <parameter>"));
            return;
        }

        const std::string               subcommand = p[0];

        if (!client.getNickname().empty())
            target = client.getNickname();

        if (subcommand == "LS") {
            std::string reply = ":" SERVER_NAME " CAP " + target + " LS :\r\n";

            client.queueOutput(reply);
            return;
        }

        if (subcommand == "LIST") {
            std::string reply = ":" SERVER_NAME " CAP " + target + " LIST :\r\n";

            client.queueOutput(reply);
            return;
        }

        if (subcommand == "REQ") {
            std::string requested;

            if (msg.hasTrailing())
                requested = msg.getTrailing();
            else if (p.size() > 1)
                requested = p[1];

            if (requested.empty()) {
                std::string reply = ":" SERVER_NAME " CAP " + target + " NAK :\r\n";

                client.queueOutput(reply);
                return;
            }

            std::string reply = ":" SERVER_NAME " CAP " + target + " NAK :" + requested + "\r\n";

            client.queueOutput(reply);
            return;
        }

        if (subcommand == "END") {
            return;
        }

        client.queueOutput(client.newReply(Socket::ERR_INVALIDCAP, ""));
}


/* ------------------------------------------------------------------------ */

/* ----------------------            PASS            ---------------------- */

/* ------------------------------------------------------------------------ */

void PassCommand::execute(const Message &msg, Server &server, Socket &client) {
    const std::vector<std::string> &p = msg.getParams();

    if (client.hasPass())
        client.queueOutput(client.newReply(Socket::ERR_ALREADYREGISTERED, ""));
    else if (p.empty() || p[0].empty())
        client.queueOutput(client.newReply(Socket::ERR_NEEDMOREPARAMS, "(usage) PASS <password>"));
    else if (!server.matchPassword(p[0])) {
        client.queueOutput(client.newReply(Socket::ERR_PASSMISMATCH, ""));
        client.setClosing();
    }
    client.setPass(true);
    return;
}

/* ------------------------------------------------------------------------ */

/* ----------------------            NICK            ---------------------- */

/* ------------------------------------------------------------------------ */

void NickCommand::execute(const Message &msg, Server &server, Socket &client) {
    const std::vector<std::string>  &p = msg.getParams();

    if (p.empty()) {
        client.queueOutput(client.newReply(Socket::ERR_NONICKNAMEGIVEN, "(usage) NICK <nickname>"));
        return;
    }

    const std::string               nick = p[0];

    if (nick.empty() || nick.size() > 9 || nick[0] == '#' || nick[0] == ':' || nick[0] == '&') {
        client.queueOutput(client.newReply(Socket::ERR_ERRONEUSNICKNAME, nick));
        return;
    }

    for (std::string::size_type i = 0; i < nick.size(); ++i) {
        char c = nick[i];

        if (!(isalnum(static_cast<unsigned char>(c)) || c == '-' || c == '_' || c == '[' || c == ']' || c == '\\' || c == '`' || c == '^' || c == '{' || c == '}')) {
            client.queueOutput(client.newReply(Socket::ERR_ERRONEUSNICKNAME, nick));
            return;
        }
    }

    Socket                          *existing = server.findClientByNick(nick);

    if (existing && existing != &client) {
        client.queueOutput(client.newReply(Socket::ERR_NICKNAMEINUSE, nick));
        return;
    }

    std::string                     oldPrefix = prefix(client);

    client.setNickname(nick);

    if (client.isRegistered()) {
        std::string                 reply = oldPrefix + " NICK :" + nick + "\r\n";
        
        client.queueOutput(reply);
    }

    if (!client.isRegistered() && !client.getUsername().empty()) {
        client.setRegistered(true);
        client.queueOutput(client.newReply(Socket::RPL_WELCOME, "42"));
        client.queueOutput(client.newReply(Socket::RPL_YOURHOST, "42_IRC"));
        client.queueOutput(client.newReply(Socket::RPL_CREATED, "by 42Students"));
        client.queueOutput(client.newReply(Socket::RPL_MYINFO, client.getNickname()));//Could be a getInfo instead;
    }
}

/* ------------------------------------------------------------------------ */

/* ----------------------            USER            ---------------------- */

/* ------------------------------------------------------------------------ */

void UserCommand::execute(const Message &msg, Server &, Socket &client) {
    const std::vector<std::string> &p = msg.getParams();

    if (client.isRegistered()) {
        client.queueOutput(client.newReply(Socket::ERR_ALREADYREGISTERED, ""));
        return;
    }

    if (p.size() < 3) {
        client.queueOutput(client.newReply(Socket::ERR_NEEDMOREPARAMS, "(usage) USER <username>"));
        return;
    }

    client.setUsername(p[0]);

    std::string realname;

    if (msg.hasTrailing())
        realname = msg.getTrailing();
    else
        realname = p[2];

    client.setRealname(realname);

    if (!client.isRegistered() && !client.getNickname().empty()) {
        client.setRegistered(true);
        client.queueOutput(client.newReply(Socket::RPL_WELCOME, ""));
        client.queueOutput(client.newReply(Socket::RPL_YOURHOST, "42_IRC"));
        client.queueOutput(client.newReply(Socket::RPL_CREATED, "by 42Students"));
        client.queueOutput(client.newReply(Socket::RPL_MYINFO, client.getNickname()));//Could be a getInfo instead;
    }
}

/* ------------------------------------------------------------------------ */

/* ----------------------            JOIN            ---------------------- */

/* ------------------------------------------------------------------------ */

void JoinCommand::execute(const Message &msg, Server &server, Socket &client) {
    
    if (!registered(client))
        return;

    const std::vector<std::string> &p = msg.getParams();

    if (p.empty()) {
        client.queueOutput(client.newReply(Socket::ERR_NEEDMOREPARAMS, "(usage) JOIN <channel>"));
        return;
    }

    if (p[0] == "0") {
        server.removeClientFromChannels(&client);
        return;
    }

    std::vector<std::string> channels = CommandRegistry::split(p[0], ',');

    for (std::vector<std::string>::iterator it = channels.begin();
            it != channels.end(); ++it) {

        if ((*it)[0] != '#' && (*it)[0] != '&') {
            client.queueOutput(client.newReply(Socket::ERR_NOSUCHCHANNEL, p[0]));
            continue;
        }

        Channel *channel = server.getOrCreate(*it);

        if (!channel)
            continue;

        if (channel->hasMember(&client))
            continue;

        channel->addMember(&client, channel->isEmpty());

        std::string reply = prefix(client) +
                            " JOIN " + *it + "\r\n";

        channel->broadcast(reply, NULL);

        if (channel->getTopic().empty())
            client.queueOutput(client.newReply(Socket::RPL_NOTOPIC, ""));
        else
            client.queueOutput(client.newReply(Socket::RPL_TOPIC, channel->getTopic()));
    }
}

/* ------------------------------------------------------------------------ */

/* ----------------------            PART            ---------------------- */

/* ------------------------------------------------------------------------ */

void PartCommand::execute(const Message &msg, Server &server, Socket &client) {
    
    if (!registered(client))
        return;

    const std::vector<std::string> &p = msg.getParams();

    if (p.empty()) {
        client.queueOutput(client.newReply(Socket::ERR_NEEDMOREPARAMS, "(usage) PART <channel> [:reason]"));
        return;
    }

    std::vector<std::string> channels = CommandRegistry::split(p[0], ',');

    for (std::vector<std::string>::iterator it = channels.begin();
            it != channels.end(); ++it) {

        Channel *channel = server.getChannel(*it);

        if (!channel) {
            client.queueOutput(client.newReply(Socket::ERR_NOSUCHCHANNEL, channel->getName()));
            continue;
        }

        if (!channel->hasMember(&client)) {
            client.queueOutput(client.newReply(Socket::ERR_NOTONCHANNEL, channel->getName()));
            continue;
        }

        std::string reply = prefix(client) + " PART " + *it;

        if (msg.hasTrailing())
            reply += " :" + msg.getTrailing();

        reply += "\r\n";

        channel->broadcast(reply, NULL);
        channel->removeMember(&client);

        if (channel->isEmpty())
            server.removeChannel(*it);
    }
}

/* ------------------------------------------------------------------------ */

/* ----------------------          PRIVMSG           ---------------------- */

/* ------------------------------------------------------------------------ */

void PrivmsgCommand::execute(const Message &msg, Server &server, Socket &client) {
    if (!registered(client))
        return;

    const std::vector<std::string> &p = msg.getParams();

    if (p.empty()) {
        client.queueOutput(client.newReply(Socket::ERR_NORECIPIENT, "(usage) PRIVMSG <target> [:message]"));
        return;
    }

    if (!msg.hasTrailing() || msg.getTrailing().empty()) {
        client.queueOutput(client.newReply(Socket::ERR_NOTEXTTOSEND, "(usage) PRIVMSG <target> [:message]"));
        return;
    }

    std::vector<std::string> targets = CommandRegistry::split(p[0], ',');

    for (std::vector<std::string>::iterator it = targets.begin();
            it != targets.end(); ++it) {

        std::string reply = prefix(client) + " PRIVMSG " + *it + " :" + msg.getTrailing() + "\r\n";

        if ((*it)[0] == '#' || (*it)[0] == '&') {
            Channel *channel = server.getChannel(*it);

            if (!channel) {
                client.queueOutput(client.newReply(Socket::ERR_NOSUCHCHANNEL, channel->getName()));
                continue;
            }

            if (!channel->hasMember(&client)) {
                client.queueOutput(client.newReply(Socket::ERR_CANNOTSENDTOCHAN, "client is not a member"));
                continue;
            }

            channel->broadcast(reply, &client);
        }
        else {
            Socket *target = server.findClientByNick(*it);

            if (!target) {
                client.queueOutput(client.newReply(Socket::ERR_NOSUCHNICK, "client is not a member"));
                continue;
            }

            target->queueOutput(reply);
        }
    }
}

/* ------------------------------------------------------------------------ */

/* ----------------------            QUIT            ---------------------- */

/* ------------------------------------------------------------------------ */

void QuitCommand::execute(const Message &msg, Server &server, Socket &client) {
    std::string reason = "Client Quit";
    std::string reply = prefix(client) + " QUIT :" + reason + "\r\n";

    if (msg.hasTrailing())
        reason = msg.getTrailing();

    server.removeClientFromChannels(&client);
    client.queueOutput(reply);
    client.setClosing();
}

/* ------------------------------------------------------------------------ */

/* ----------------------           TOPIC            ---------------------- */

/* ------------------------------------------------------------------------ */
  
void TopicCommand::execute(const Message &msg, Server &server, Socket &client) {
    
    if (!registered(client))
        return;

    const std::vector<std::string> &p = msg.getParams();

    if (p.empty()) {
        client.queueOutput(client.newReply(Socket::ERR_NEEDMOREPARAMS, "(usage) TOPIC <channel> [:topic]"));
        return;
    }

    Channel *channel = server.getChannel(p[0]);

    if (!channel) {
        client.queueOutput(client.newReply(Socket::ERR_NOSUCHCHANNEL, p[0]));
        return;
    }

    if (!channel->hasMember(&client)) {
        client.queueOutput(client.newReply(Socket::ERR_NOTONCHANNEL, p[0]));
        return;
    }

    if (!msg.hasTrailing()) {
        if (channel->getTopic().empty())
            client.queueOutput(client.newReply(Socket::RPL_NOTOPIC, ""));
        else
            client.queueOutput(client.newReply(Socket::RPL_TOPIC, channel->getTopic()));
        return;
    }

    if (!channel->isOperator(&client)) {
        client.queueOutput(client.newReply(Socket::ERR_CHANOPRIVSNEEDED, p[0]));
        return;
    }

    channel->setTopic(msg.getTrailing());

    std::string reply = prefix(client) + " TOPIC " + p[0] + " :" + msg.getTrailing() + "\r\n";

    channel->broadcast(reply, NULL);
}

/* ------------------------------------------------------------------------ */

/* ----------------------            MODE            ---------------------- */

/* ------------------------------------------------------------------------ */

void ModeCommand::execute(const Message &msg, Server &server, Socket &client) {
    
    if (!registered(client))
        return;

    const std::vector<std::string> &p = msg.getParams();

    if (p.empty()) {
        client.queueOutput(client.newReply(Socket::ERR_NEEDMOREPARAMS, "(usage) MODE <target> [:modes] [:parameters]"));
        return;
    }

    Channel *channel = server.getChannel(p[0]);

    if (!channel) {
        client.queueOutput(client.newReply(Socket::ERR_NOSUCHCHANNEL, p[0]));
        return;
    }

    if (!channel->hasMember(&client)) {
        client.queueOutput(client.newReply(Socket::ERR_NOTONCHANNEL, p[0]));
        return;
    }

    if (!channel->isOperator(&client)) {
        client.queueOutput(client.newReply(Socket::ERR_CHANOPRIVSNEEDED, p[0]));
        return;
    }

    if (p.size() < 2)
        return;

    if (p[1].size() == 2 &&
        p[1][0] == '+' && p[1][1] == 'o') {

        if (p.size() < 3) {
            client.queueOutput(client.newReply(Socket::ERR_NEEDMOREPARAMS, "(usage) MODE <target> [:modes] [:parameters]"));
            return;
        }

        Socket *target = server.findClientByNick(p[2]);

        if (!target || !channel->hasMember(target)) {
            client.queueOutput(client.newReply(Socket::ERR_USERNOTINCHANNEL, target->getNickname()));
            return;
        }

        channel->addOperator(target);

        std::string reply = prefix(client) + " MODE " + p[0] + " +o " + p[2] + "\r\n";

        channel->broadcast(reply, NULL);
        return;
    }
}

/* ------------------------------------------------------------------------ */

/* ----------------------            KICK            ---------------------- */

/* ------------------------------------------------------------------------ */

void KickCommand::execute(const Message &msg, Server &server, Socket &client) {

    if (!registered(client))
        return;

    const std::vector<std::string> &p = msg.getParams();

    if (p.size() < 2) {
        client.queueOutput(client.newReply(Socket::ERR_NEEDMOREPARAMS, "(usage) MODE <channel> <nickname> [:reason]"));
        return;
    }

    Channel *channel = server.getChannel(p[0]);

    if (!channel) {
        client.queueOutput(client.newReply(Socket::ERR_NOSUCHCHANNEL, p[0]));
        return;
    }

    if (!channel->hasMember(&client)) {
        client.queueOutput(client.newReply(Socket::ERR_NOTONCHANNEL, p[0]));
        return;
    }

    if (!channel->isOperator(&client)) {
        client.queueOutput(client.newReply(Socket::ERR_CHANOPRIVSNEEDED, p[0]));
        return;
    }

    Socket *target = server.findClientByNick(p[1]);

    if (!target || !channel->hasMember(target)) {
        client.queueOutput(client.newReply(Socket::ERR_USERNOTINCHANNEL, target->getNickname()));
        return;
    }

    std::string reason = client.getNickname();
    
    if (msg.hasTrailing())
        reason = msg.getTrailing();

    std::string reply = prefix(client) + " KICK " + p[0] + " " + p[1] + " :" + reason + "\r\n";

    channel->broadcast(reply, NULL);
    channel->removeMember(target);

    if (channel->isEmpty())
        server.removeChannel(p[0]);
}

/* ------------------------------------------------------------------------ */

/* ----------------------           INVITE           ---------------------- */

/* ------------------------------------------------------------------------ */

void InviteCommand::execute(const Message &msg, Server &server, Socket &client) {
        
    if (!registered(client))
        return;

    const std::vector<std::string> &p = msg.getParams();

    if (p.size() < 2) {
        client.queueOutput(client.newReply(Socket::ERR_NEEDMOREPARAMS, "(usage) INVITE <nickname> <channel>"));
        return;
    }

    Socket *target = server.findClientByNick(p[0]);

    if (!target) {
        client.queueOutput(client.newReply(Socket::ERR_NOSUCHNICK, target->getNickname()));
        return;
    }

    Channel *channel = server.getChannel(p[1]);

    if (!channel) {
        client.queueOutput(client.newReply(Socket::ERR_NOSUCHCHANNEL, channel->getName()));
        return;
    }

    if (!channel->hasMember(&client)) {
        client.queueOutput(client.newReply(Socket::ERR_NOTONCHANNEL, channel->getName()));
        return;
    }

    if (channel->hasMember(target)) {
        client.queueOutput(client.newReply(Socket::ERR_USERONCHANNEL, target->getNickname()));
        return;
    }

    std::string reply = prefix(client) + " INVITE " + p[0] + " :" + p[1] + "\r\n";

    target->queueOutput(reply);
}

/* ------------------------------------------------------------------------ */

/* ----------------------          DISPATCH          ---------------------- */

/* ------------------------------------------------------------------------ */

void CommandRegistry::exe(const Message &msg, Server &server, Socket &client) {
    const std::string &cmd = msg.getCommand();

    if (cmd == "CAP") {
        CapCommand command;
        command.execute(msg, server, client);
    }
    else if (cmd == "PASS") {
        PassCommand command;
        command.execute(msg, server, client);
    }
    else if (cmd == "NICK") {
        NickCommand command;
        command.execute(msg, server, client);
    }
    else if (cmd == "USER") {
        UserCommand command;
        command.execute(msg, server, client);
    }
    else {
        if (!client.hasPass()) {
            client.queueOutput(client.newReply(Socket::ERR_PASSMISMATCH, ". You need to instert the server password to proceed. [usage] PASS <password>"));
            return;
        }
        if (cmd == "JOIN") {
            JoinCommand command;
            command.execute(msg, server, client);
        }
        else if (cmd == "PART") {
            PartCommand command;
            command.execute(msg, server, client);
        }
        else if (cmd == "PRIVMSG") {
            PrivmsgCommand command;
            command.execute(msg, server, client);
        }
        else if (cmd == "QUIT") {
            QuitCommand command;
            command.execute(msg, server, client);
        }
        else if (cmd == "TOPIC") {
            TopicCommand command;
            command.execute(msg, server, client);
        }
        else if (cmd == "MODE") {
            ModeCommand command;
            command.execute(msg, server, client);
        }
        else if (cmd == "KICK") {
            KickCommand command;
            command.execute(msg, server, client);
        }
        else if (cmd == "INVITE") {
            InviteCommand command;
            command.execute(msg, server, client);
        }
        else {
            client.queueOutput(client.newReply(Socket::ERR_UNKNOWNCOMMAND, cmd));
        }
    }
}


/* ------------------------------------------------------------------------ */

/* ----------------------          UTILITY           ---------------------- */

/* ------------------------------------------------------------------------ */

std::vector<std::string>    CommandRegistry::split(const std::string &str, char sep) {
    std::vector<std::string>    result;
    std::stringstream           ss(str);
    std::string                 item;

    while (std::getline(ss, item, sep))
        if (!item.empty())
            result.push_back(item);

    return (result);
}
