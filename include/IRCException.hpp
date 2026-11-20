#ifndef IRCEXCEPTION_HPP
# define IRCEXCEPTION_HPP

# define ERROR "\033[31m[ERROR]\t"
# define INFO "\033[33m[INFO]\t"
# define CLEAN "\033[0m"
# define END "\033[0m\n"

# include <exception>
# include <string>

class IRCException : public std::exception
{

	protected:

		struct 				prefix {
			const errco 	code;
			const char 		*msg; 
		}    	
    	
    	std::string 		_message;


	public:
    	
    						IRCException(const std::string& message);

    	virtual				~IRCException() const throw();

    	const char*			what() const throw();

    	const std::string 	format() = 0;    	
    	static void			initializeErrco() = 0;
};

#endif