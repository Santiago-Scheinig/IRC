#include "Parser.hpp"

Parser::Parser() {}

Parser::Parser(const Parser& other) {
    (void)other;
}

Parser& Parser::operator=(const Parser& other) {
    (void)other;
    return *this;
}

Parser::~Parser() {}

Message Parser::parse(const std::string& raw_line) const {
    Message msg;
    (void)raw_line;
    return msg;
}
