#ifndef ICOMMAND_HPP
# define ICOMMAND_HPP

class ICommand {
public:
    // Orthodox Canonical Form (OCF) Interface requirement
    ICommand() = default;
    ICommand(const ICommand& other) = default;
    ICommand& operator=(const ICommand& other) = default;
    virtual ~ICommand() = default;

    // Pure virtual execute method that concrete commands will implement
    virtual void execute() = 0;
};

#endif