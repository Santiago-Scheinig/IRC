#ifndef MESSAGE_HPP
# define MESSAGE_HPP

# include <string>
# include <vector>

class Message {
private:
    std::string              _prefix;
    std::string              _command;
    std::vector<std::string> _params;

public:
    // Orthodox Canonical Form (OCF)
    Message();
    Message(const Message& other);
    Message& operator=(const Message& other);
    ~Message();

    // Parameterized Constructor
    Message(const std::string& prefix, const std::string& command, const std::vector<std::string>& params);

    // Getters
    const std::string&              getPrefix() const;
    const std::string&              getCommand() const;
    const std::vector<std::string>& getParams() const;
};

#endif