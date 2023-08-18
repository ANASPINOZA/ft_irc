NAME = server

SRC = server.cpp main.cpp client.cpp Channel.cpp ./commands/Join.cpp ./commands/Invite.cpp ./commands/Kick.cpp msgHandler.cpp ./commands/parsing.cpp

OBJ = $(SRC:.cpp=.o)

CPP = c++

INC = server.hpp client.hpp Channel.hpp  ./commands/commands.hpp msgHandler.hpp

CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -fsanitize=address

all : $(NAME)

$(NAME) : $(OBJ)
	$(CPP) $(CXXFLAGS) $(SRC) -o $@

%.o : %.cpp $(INC)
	$(CC) $(CXXFLAGS) -c $< -o $@ 

clean :
	rm -rf $(OBJ)

fclean : clean
	rm -rf $(NAME)

re : fclean all

