NAME = server

SRC = server.cpp main.cpp client.cpp Channel.cpp ./commands/Join.cpp ./commands/Bot.cpp ./commands/checkIfDeconected.cpp ./commands/Invite.cpp  ./commands/Privmsg.cpp ./commands/Mode.cpp ./commands/Topic.cpp ./commands/Kick.cpp msgHandler.cpp ./commands/parsing.cpp

OBJ = $(SRC:.cpp=.o)

CPP = c++

INC = server.hpp client.hpp Channel.hpp  ./commands/Commands.hpp

CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -fsanitize=address


all : $(NAME)

$(NAME) : $(OBJ) $(INC)
	$(CPP) $(CXXFLAGS) $(OBJ) -o $@

%.o : %.cpp  $(INC)
	$(CPP) $(CXXFLAGS) -c $< -o $@ 

clean :
	rm -rf $(OBJ)

fclean : clean
	rm -rf $(NAME)

re : fclean all

