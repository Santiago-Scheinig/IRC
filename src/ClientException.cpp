#include "ClientException.hpp"

static const prefix	prefixArr[] = {
	{CONNECTION_ACCEPT_FAILED, "Failed to accept client connection: "},
	{CONNECTION_READ_FAILED, "Failed to read from client connection: "},
	{CONNECTION_WRITE_FAILED, "Failed to write to client connection: "},
	{CONNECTION_TIMEOUT, "Client connection timed out: "},
	{CONNECTION_LOST, "Client connection lost: "},
	{INVALID_CLIENT_STATE, "Invalid client state: "},
	{CLIENT_RESOURCE_ALLOCATION_FAILED, "Failed to allocate client resources: "},
	{INVALID_NICKNAME, "Invalid client nickname: "},
	{INVALID_USERNAME, "Invalid client username: "},
	{INVALID_PASSWORD, "Invalid client password: "},
	{REGISTRATION_INCOMPLETE, "Client registration incomplete: "},
	{UNKNOWN_COMMAND, "Unknown client command: "},
	{MALFORMED_COMMAND, "Malformed client command: "},
	{INVALID_COMMAND_PARAMETERS, "Invalid command parameters: "},
	{UNSUPPORTED_COMMAND, "Unsupported client command: "},
	{COMMAND_EXECUTION_FAILED, "Failed to execute client command: "},
	{INVALID_CHANNEL_NAME, "Invalid channel name:"},
	{CHANNEL_NOT_FOUND, "Channel not found: "},
	{CHANNEL_CREATION_FAILED, "Failed to create channel: "},
	{CHANNEL_ALREADY_EXISTS, "Channel already exists: "},
	{CHANNEL_CAPACITY_EXCEEDED, "Channel capacity exceeded: "},
	{CHANNEL_INVITATION_REQUIRED, "Channel invitation required: "},
	{CLIENT_BANNED_FROM_CHANNEL, "Client is banned from channel: "},
	{INSUFFICIENT_CHANNEL_PRIVILEGES, "Insufficient channel privileges: "},
	{EMPTY_MESSAGE, "Empty client message: "},
	{MESSAGE_TOO_LONG, "Client message exceeds maximum length: "},
	{INVALID_MESSAGE_TARGET, "Invalid message target: "},
	{MESSAGE_DELIVERY_FAILED, "Failed to deliver client message: "},
	{END, "Invalid client errco: ")
}

					ClientException::ClientException(errco code) 
						:	IRCException(format(code)),
							_code(code) {}

					ClientException::ClientException(errco code, const std::string& message)
						:	IRCException(format(code, message)),
							_code(code) {}

const std::string	ClientException::format(errco code) {
	for (int i = 0; prefixes[i] != END; i++) {
		if (prefixes[i].code == code)
			return (LOG + prefixes.at(code) + END);
	}
	return (LOG + prefixes.at(code) + END);
}

const std::string	ClientException::format(errco code, const std::string& sufix) {
	for (int i = 0; prefixes[i] != END; i++) {
		if (prefixes[i].code == code)
			return (LOGRROR + prefixes.at(code) + CLEAN + sufix + END);
	}
	return (LOG + prefixes.at(code) + CLEAN + sufix + END);}
}
