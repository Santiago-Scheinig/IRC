#ifndef SERVEREXCEPTION_HPP
# define SERVEREXCEPTION_HPP

# include "IRCException.hpp"

# include <map>
# include <string> 

/**
 * 
 */
class ServerException : virtual public IRCException {

	public:

    	enum 	 			errco {

    		/* CONFIGURATION */

    		CONFIG_LOAD_FAILED,
    		CONFIG_INVALID,
		 

		    /* SOCKET MANAGEMENT */

    		SOCKET_CREATION_FAILED,
    		SOCKET_BIND_FAILED,
    		SOCKET_LISTEN_FAILED,

    		/* EVENT MANAGEMENT */

    		POLL_INITIALIZATION_FAILED,
    		EVENT_LOOP_FAILED,

    		/* SERVER RESOURCES */

    		MEMORY_ALLOCATION_FAILED,
    		RESOURCE_EXHAUSTED,
    		RESOURCE_UNAVAILABLE,

    		/* INTERNAL SERVER STATE */

    		INVALID_INTERNAL_STATE,
    		INTERNAL_OPERATION_FAILED,

    		END
		};

	private:
    	
    	errco 				_code;

		const std::string  	format(errco code);
		const std::string 	format(errco code, const std::string& sufix);

	public:

							ServerException(errco code);
		    				ServerException(errco code, const std::string& message);
			    			
		virtual    			~ServerException() throw();
};

typedef ServerException	SrvExc;

#endif