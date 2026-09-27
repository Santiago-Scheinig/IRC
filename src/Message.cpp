#include "Message.hpp"

// Default Constructor
Message::Message() : _prefix(""), _command(""), _params() {}

// Copy Constructor
Message::Message(const Message& other) {
    *this = other;
}

// Copy Assignment Operator
Message& Message::operator=(const Message& other) {
    if (this != &other) {
        this->_prefix = other._prefix;
        this->_command = other._command;
        this->_params = other._params;
    }
    return *this;
}

// Destructor
Message::~Message() {}

// Parameterized Constructor
Message::Message(const std::string& prefix, const std::string& command, const std::vector<std::string>& params)
    : _prefix(prefix), _command(command), _params(params) {}

// Getters
const std::string& Message::getPrefix() const {
    return this->_prefix;
}

const std::string& Message::getCommand() const {
    return this->_command;
}

const std::vector<std::string>& Message::getParams() const {
    return this->_params;
}
