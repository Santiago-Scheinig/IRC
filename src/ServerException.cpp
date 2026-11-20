#include "ServerException.hpp"

static const prefix	*prefixArr[] = {
	{CONFIG_LOAD_FAILED, "Failed to load server configuration: "},
	{CONFIG_INVALID, "Invalid server configuration: "},
	{SOCKET_CREATION_FAILED, "Failed to create server socket: "},
	{SOCKET_BIND_FAILED, "Failed to bind server socket: "},
	{SOCKET_LISTEN_FAILED, "Failed to listen on server socket: "},
	{POLL_INITIALIZATION_FAILED, "Failed to initialize polling mechanism: "},
	{EVENT_LOOP_FAILED, "Server event loop failure: "},
	{MEMORY_ALLOCATION_FAILED, "Server memory allocation failed: "},
	{RESOURCE_EXHAUSTED, "Server resource exhaustion: "},
	{RESOURCE_UNAVAILABLE, "Server resource unavailable: "},
	{INVALID_INTERNAL_STATE, "Invalid server internal state: "},
	{INTERNAL_OPERATION_FAILED, "Server internal operation failed: "},
	{END, "Invalid server errco: "}
};

ServerException::ServerException(errco code)
	:	IRCException(format(code)),
		_code(code) {}

ServerException::ServerException(errco code, const std::string& message)
	:	IRCException(format(code, message)),
		_code(code) {}

const std::string	format(errco code) {
	for (int i = 0; prefixes[i] != END; i++) {
		if (prefixes[i].code == code)
			return (ERROR + prefixes.at(code) + END);
	}
	return (ERROR + prefixes.at(code) + END);
}

const std::string	format(errco code, const std::string& sufix) {
	for (int i = 0; prefixes[i] != END; i++) {
		if (prefixes[i].code == code)
			return (ERROR + prefixes.at(code) + CLEAN + sufix + END);
	}
	return (ERROR + prefixes.at(code) + CLEAN + sufix + END);}
}