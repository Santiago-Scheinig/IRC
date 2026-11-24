NAME     := ircserv

CXX      := c++
CXXFLAGS := -Wall -Wextra -Werror -std=c++98 -g -Iinclude

SRCS := main.cpp \
        src/Server.cpp \
        src/Socket.cpp \
        src/Channel.cpp \
        src/Message.cpp \
        src/Parser.cpp \
        src/CommandRegistry.cpp \
        src/ACommand.cpp \
        src/Config.cpp \
        src/IRCException.cpp \
        src/ServerException.cpp \
        src/ClientException.cpp

OBJS := $(SRCS:%.cpp=obj/%.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

obj/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf obj

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
