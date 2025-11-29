NAME			=	webserv

MAKEFLAGS		=	-r -R

CXX				=	c++
CXXFLAGS		=	-MMD -MP -std=c++11
# CXXFLAGS		+=	-Wall -Wextra -Werror
# CXXFLAGS		+=	-fsanitize=address
# CXXFLAGS		+=	-g

INCLUDE_DIRS	=	incl \
					incl/Wrappers \
					incl/Cgi
INCLUDE_FLAGS	=	$(addprefix -I , $(INCLUDE_DIRS))

SRC_DIR			=	src
SRC_FILES		=	main.cpp \
					Logger.cpp

CONFIG_DIR		:=	$(SRC_DIR)/Config
CONFIG_FILES	=	Config.cpp

HTTP_DIR		:=	$(SRC_DIR)/Http
HTTP_FILES		=	HttpRequest.cpp \
					HttpResponse.cpp

SERVER_DIR		:=	$(SRC_DIR)/Server
SERVER_FILES	=	Epoll.cpp \
					Server.cpp

SOCKETS_DIR		:=	$(SRC_DIR)/Sockets
SOCKETS_FILES	=	ListenSocket.cpp \
					Socket.cpp

CGI_DIR			:=	$(SRC_DIR)/CgiHandler
CGI_FILES		=	CgiHandler.cpp \
					ErrorPageHandler.cpp

SRC				=	$(addprefix $(SRC_DIR)/, $(SRC_FILES)) \
					$(addprefix $(CONFIG_DIR)/, $(CONFIG_FILES)) \
					$(addprefix $(HTTP_DIR)/, $(HTTP_FILES)) \
					$(addprefix $(SERVER_DIR)/, $(SERVER_FILES)) \
					$(addprefix $(SOCKETS_DIR)/, $(SOCKETS_FILES)) \
					$(addprefix $(CGI_DIR)/, $(CGI_FILES))

OBJ_DIR			=	obj
OBJ				:=	$(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
DEP				:=	$(OBJ:%.o=%.d)

all : $(NAME)

$(NAME) : $(OBJ)
	@ echo "${BLUE}$(NAME) compiling...${RESET}"
	$(CXX) $^ $(CXXFLAGS) $(INCLUDE_FLAGS) -o $(NAME)
	@ echo "${GREEN}$(NAME) made!${RESET}"

$(OBJ_DIR)/%.o:$(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDE_FLAGS) -c $< -o $@

-include $(DEP)

clean :
	@if [ -n "$(OBJ_DIR)" ] && [ "$(OBJ_DIR)" != "/" ]; \
	then \
		rm -rf $(OBJ_DIR); echo 'rm -rf $(OBJ_DIR)'; \
	else \
		echo "Warning: OBJ_DIR is undefined"; \
	fi
	@ echo "${BLUE}Object folder removed.${RESET}"

fclean : clean
	rm -f $(NAME)
	@ echo "${YELLOW}$(NAME) fcleaned!${RESET}"

re : clean all

.PHONY: all clean fclean re

## Colours ##
RESET		:= \033[0m
RED			:= \033[1;91m
GREEN		:= \033[1;92m
YELLOW		:= \033[1;93m
BLUE		:= \033[1;94m
PURPLE		:= \033[1;95m
CYAN		:= \033[1;96m
WHITE		:= \033[1;97m
BLACK		:= \033[1;90m