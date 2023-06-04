CC = c++

CFLAGS = -Wall -Wextra -Werror -std=c++98

HEADER = srcs/server/server.hpp \
		srcs/client/client.hpp \
		srcs/sockets/sockets.hpp \
		srcs/extra.hpp \
		srcs/parsers/parse_config.hpp


NAME = webserv
NAME_CLIENT = client
SRC_FILE =  main.cpp \
	srcs/extra_functions.cpp \
	srcs/sockets/sockets.cpp \
	srcs/server/server.cpp \
	srcs/server/location.cpp \
	srcs/server/headers.cpp \
	srcs/server/cgi_env.cpp \
	srcs/parsers/parse_config.cpp \
	srcs/client/client.cpp \
	srcs/server/autoindex.cpp \
	srcs/server/handle_request.cpp \
	srcs/parsers/location_block.cpp \
	srcs/parsers/server_block.cpp \
	srcs/parsers/Syntax_errors.cpp \
	srcs/parsers/utils.cpp \
	srcs/parsers/v_server.cpp

OBJ_FILE = $(SRC_FILE:.cpp=.o)

all : $(NAME)

%.o:%.cpp
	$(CC) $(CFLAGS)  -c $< -o $@

$(NAME) : $(OBJ_FILE) $(CLIENT) $(HEADER)
	$(CC) $(CFLAGS) $(OBJ_FILE) -o $(NAME)

#	@$(CC) $(CFLAGS) $(SRC_FILE) -lkqueue -lpthread -o $(NAME)

clean :
	rm -f $(OBJ_FILE)

fclean : clean
	rm -f $(NAME)

re : fclean all