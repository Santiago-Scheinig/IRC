#ifndef CLIENTEXCEPTION_HPP
# define CLIENTEXCEPTION_HPP

# include "IRCException.hpp"

# include <string>
# include <map>

/**
 * 
 */
class ClientException : virtual public IRCException {

	public: 
		
	    enum 				errco {

			/* CONNECTION MANAGEMENT */

    		CONNECTION_ACCEPT_FAILED,
    		CONNECTION_READ_FAILED, 
    		CONNECTION_WRITE_FAILED,
    		CONNECTION_TIMEOUT,
    		CONNECTION_LOST,

    		/* CLIENT STATE */

    		INVALID_CLIENT_STATE,
    		CLIENT_RESOURCE_ALLOCATION_FAILED,

    		/* REGISTRATION */

    		INVALID_NICKNAME,
    		INVALID_USERNAME,
    		INVALID_PASSWORD,
    		REGISTRATION_INCOMPLETE,

    		/* COMMAND PROCESSING */

    		UNKNOWN_COMMAND,
    		MALFORMED_COMMAND,
    		INVALID_COMMAND_PARAMETERS,
    		UNSUPPORTED_COMMAND,
    		COMMAND_EXECUTION_FAILED,

    		/* CHANNEL MANAGEMENT */

    		INVALID_CHANNEL_NAME,
    		CHANNEL_NOT_FOUND,
    		CHANNEL_CREATION_FAILED,
    		CHANNEL_ALREADY_EXISTS,
    		CHANNEL_CAPACITY_EXCEEDED,
    		CHANNEL_INVITATION_REQUIRED,
    		CLIENT_BANNED_FROM_CHANNEL,
    		INSUFFICIENT_CHANNEL_PRIVILEGES,

    		/* MESSAGE PROCESSING */

    		EMPTY_MESSAGE,
    		MESSAGE_TOO_LONG,
    		INVALID_MESSAGE_TARGET,
    		MESSAGE_DELIVERY_FAILED,

    		END		
		};

	private:
    	
    	errco 				_code;

    	const std::string  	format(errco code);
    	const std::string 	format(errco code, const std::string& sufix);

	public:

    						ClientException(errco code);
    						ClientException(errco code, const std::string& message);

		errco				getCode();
		virtual				~ClientException() throw();
};

typedef ClientException	CliExc;

#endif