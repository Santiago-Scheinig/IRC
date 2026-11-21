#ifndef IRCEXCEPTION_HPP
# define IRCEXCEPTION_HPP

# define ERROR "\033[31m[ERROR] "
# define INFO "\033[33m[INFO] "
# define CLEAN "\033[0m"
# define EOM "\033[0m\n"

# include <ctime>
# include <string>
# include <sstream>
# include <exception>

class IRCException : virtual public std::exception
{

	public:

		struct 				prefix {
			const int 		code;
			const char 		*str; 
		};

	protected:
    	
    	std::string 		_message;


	public:
    	
    						IRCException(const std::string& message);

    	virtual				~IRCException() throw();

    	const char*			what() const throw();
};

#endif