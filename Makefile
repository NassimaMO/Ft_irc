NAME			=	ircserv

CC				=	c++

CFLAGS			=	-Wall -Wextra -Werror -g -std=c++98

SRCS			=	main.cpp		validArgs.cpp			\
					Server.cpp		ServerGetSet.cpp	ServerExec.cpp			ServerExecBot.cpp	 ServerVerifInfoClient.cpp \
					Clients.cpp		ClientsGetSet.cpp	ClientsSendRPLErr.cpp	\
					Channels.cpp	ChannelsExecMode.cpp\

OBJ_DIR			=	obj

SRCS_DIR		=	srcs

INCL_DIR		=	includes

OBJ				=	$(addprefix $(OBJ_DIR)/,$(SRCS:%.cpp=%.o))

RM				=	rm -f

all:				${NAME}

$(OBJ_DIR)/%.o:		$(SRCS_DIR)/%.cpp
					@mkdir -p $(OBJ_DIR)
					$(CC) $(CFLAGS) -c $< -o $@ -I$(INCL_DIR)

${NAME}:			${OBJ}
					$(CC) $(CFLAGS) $^ -o $@

clean:
				${RM} -r ${OBJ_DIR}
				${RM} ${OBJ}

fclean:			clean
				${RM} ${NAME}

re:				fclean all

.PHONY: 		all clean fclean re
