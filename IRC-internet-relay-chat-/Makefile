NAME = ircserv
CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -I./includes

SRCS = srcs/main.cpp \
	srcs/server/Server.cpp \
	srcs/server/ServerNetwork.cpp \
	srcs/server/ServerClients.cpp \
	srcs/server/Client.cpp \
	srcs/client/ClientParser.cpp \
	srcs/client/ClientSession.cpp \
	srcs/server/Channel.cpp \
	srcs/channel/ChannelMembers.cpp \
	srcs/channel/ChannelModes.cpp \
	srcs/commands/Auth.cpp \
	srcs/commands/TopicCommand.cpp \
	srcs/commands/ModeCommand.cpp \
	srcs/commands/Invite.cpp \
	srcs/commands/Join.cpp \
	srcs/commands/Kick.cpp \
	srcs/commands/PrivMsg.cpp \
	srcs/commands/Call_Commands.cpp


OBJS = $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re