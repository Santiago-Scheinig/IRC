#include "ServerException.hpp"

static const SrvExc::prefix 	prefixArr[] = {
	{SrvExc::CONFIG_LOAD_FAILED, "Failed to load server configuration: "},
	{SrvExc::CONFIG_INVALID, "Invalid server configuration: "},
	{SrvExc::SOCKET_CREATION_FAILED, "Failed to create server socket: "},
	{SrvExc::SOCKET_BIND_FAILED, "Failed to bind server socket: "},
	{SrvExc::SOCKET_LISTEN_FAILED, "Failed to listen on server socket: "},
	{SrvExc::POLL_INITIALIZATION_FAILED, "Failed to initialize polling mechanism: "},
	{SrvExc::EVENT_LOOP_FAILED, "Server event loop failure: "},
	{SrvExc::MEMORY_ALLOCATION_FAILED, "Server memory allocation failed: "},
	{SrvExc::RESOURCE_EXHAUSTED, "Server resource exhaustion: "},
	{SrvExc::RESOURCE_UNAVAILABLE, "Server resource unavailable: "},
	{SrvExc::INVALID_INTERNAL_STATE, "Invalid server internal state: "},
	{SrvExc::INTERNAL_OPERATION_FAILED, "Server internal operation failed: "},
	{SrvExc::END, "Invalid server errco: "}
};

					SrvExc::ServerException(errco code)
						:	IRCException(format(code)),
							_code(code) {}

					SrvExc::ServerException(errco code, const std::string& message)
						:	IRCException(format(code, message)),
							_code(code) {}

					SrvExc::~ServerException() throw() {}

const std::string	SrvExc::format(errco code) {
	std::string msg;
	int 		i = 0;

	while (prefixArr[i].code != END) {
		if (prefixArr[i].code == code) {
			msg = prefixArr[i].str;
			return (ERROR + msg + EOM);
		}
		i++;
	}
	msg = prefixArr[i].str;
	return (ERROR + msg + EOM);
}
 
const std::string	SrvExc::format(errco code, const std::string& sufix) {
	std::string msg;
	int 		i = 0;

	while (prefixArr[i].code != END) {
		if (prefixArr[i].code == code) {
			msg = prefixArr[i].str;
			return (ERROR + msg + sufix + EOM);
		}
		i++;
	}
	msg = prefixArr[i].str;
	return (ERROR + msg + sufix + EOM);
}