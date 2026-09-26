#ifndef PARSER_HPP
# define PARSER_HPP

# include <string>
# include "Message.hpp"

class Parser {
public:
    // Orthodox Canonical Form (OCF)
    Parser();                               // Default Constructor
    Parser(const Parser& other);            // Copy Constructor
    Parser& operator=(const Parser& other);   // Copy Assignment Operator
    ~Parser();                              // Destructor

    // Core Method
    Message parse(const std::string& raw_line) const;
};

#endif