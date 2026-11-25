#include "ClientException.hpp"

/**
 * 
 */
static const CliExc::prefix	prefixArr[] = {
	{CliExc::CONNECTION_ACCEPT_FAILED, "Failed to accept client connection: "},
	{CliExc::CONNECTION_READ_FAILED, "Failed to read from client connection: "},
	{CliExc::CONNECTION_WRITE_FAILED, "Failed to write to client connection: "},
	{CliExc::CONNECTION_TIMEOUT, "Client connection timed out: "},
	{CliExc::CONNECTION_LOST, "Client connection lost: "},
	{CliExc::INVALID_CLIENT_STATE, "Invalid client state: "},
	{CliExc::CLIENT_RESOURCE_ALLOCATION_FAILED, "Failed to allocate client resources: "},
	{CliExc::INVALID_NICKNAME, "Invalid client nickname: "},
	{CliExc::INVALID_USERNAME, "Invalid client username: "},
	{CliExc::INVALID_PASSWORD, "Invalid client password: "},
	{CliExc::REGISTRATION_INCOMPLETE, "Client registration incomplete: "},
	{CliExc::UNKNOWN_COMMAND, "Unknown client command: "},
	{CliExc::MALFORMED_COMMAND, "Malformed client command: "},
	{CliExc::INVALID_COMMAND_PARAMETERS, "Invalid command parameters: "},
	{CliExc::UNSUPPORTED_COMMAND, "Unsupported client command: "},
	{CliExc::COMMAND_EXECUTION_FAILED, "Failed to execute client command: "},
	{CliExc::INVALID_CHANNEL_NAME, "Invalid channel name:"},
	{CliExc::CHANNEL_NOT_FOUND, "Channel not found: "},
	{CliExc::CHANNEL_CREATION_FAILED, "Failed to create channel: "},
	{CliExc::CHANNEL_ALREADY_EXISTS, "Channel already exists: "},
	{CliExc::CHANNEL_CAPACITY_EXCEEDED, "Channel capacity exceeded: "},
	{CliExc::CHANNEL_INVITATION_REQUIRED, "Channel invitation required: "},
	{CliExc::CLIENT_BANNED_FROM_CHANNEL, "Client is banned from channel: "},
	{CliExc::INSUFFICIENT_CHANNEL_PRIVILEGES, "Insufficient channel privileges: "},
	{CliExc::EMPTY_MESSAGE, "Empty client message: "},
	{CliExc::MESSAGE_TOO_LONG, "Client message exceeds maximum length: "},
	{CliExc::INVALID_MESSAGE_TARGET, "Invalid message target: "},
	{CliExc::MESSAGE_DELIVERY_FAILED, "Failed to deliver client message: "},
	{CliExc::END, "Invalid client errco: "}
};
 
/* ------------------------------------------------------------------------ */

/* ----------------------        CONSTRUCTORS        ---------------------- */

/* ------------------------------------------------------------------------ */

/**
 * 
 */
					CliExc::ClientException(errco code) 
						:	IRCException(format(code)),
							_code(code) {}

/**
 * 
 */
					CliExc::ClientException(errco code, const std::string& message)
						:	IRCException(format(code, message)),
							_code(code) {}

/**
 * 
 */
					CliExc::~ClientException() throw() {}

/* ------------------------------------------------------------------------ */

/* ----------------------           UTILITY          ---------------------- */

/* ------------------------------------------------------------------------ */

/**
 * 
 */
const std::string	CliExc::format(errco code) {
	std::string msg;
	
	int 		i = 0;

	while (prefixArr[i].code != END) {
		if (prefixArr[i].code == code) {
			msg = prefixArr[i].str;
			return (INFO + msg + EOM);
		}
		i++;
	}

	msg = prefixArr[i].str;
	return (INFO + msg + EOM);
}

/**
 * 
 */
const std::string	CliExc::format(errco code, const std::string& sufix) {
	std::string msg;

	int 		i = 0;

	while (prefixArr[i].code != END) {
		if (prefixArr[i].code == code) {
			msg = prefixArr[i].str;
			return (INFO + msg + sufix + EOM);
		}
		i++;
	}
	
	msg = prefixArr[i].str;
	return (INFO + msg + sufix + EOM);
}

/* ------------------------------------------------------------------------ */

/* ----------------------          GETTERS           ---------------------- */

/* ------------------------------------------------------------------------ */

/**
 * 
 */
CliExc::errco		CliExc::getCode() {
	return (_code);
}

/* ------------------------------------------------------------------------ */

/* ----------------------            END             ---------------------- */

/* ------------------------------------------------------------------------ */