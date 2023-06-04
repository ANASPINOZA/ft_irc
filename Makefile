NAME = server

SRC = server.cpp main.cpp

OBJ = $(SRC:.cpp=.o)

CPP = c++

INC = server.hpp

CXXFLAGS = -Wall -Wextra -Werror -std=c++98

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

