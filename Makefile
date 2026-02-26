NAME			=	webserv

MAKEFLAGS		=	-r -R

CXX				=	c++
CXXFLAGS		=	-MMD -MP -std=c++20
# CXXFLAGS		+=	-Wall -Wextra -Werror
# CXXFLAGS		+=	-fsanitize=address
CXXFLAGS		+=	-g

SRC_DIR			=	src
INC_DIR			=	incl
OBJ_DIR			=	obj

MAIN_FILES		=	main.cpp

CONFIG_MODULE	=	Config
CONFIG_FILES	=	Config.cpp			\
					Parse.cpp			\
					TokenStream.cpp		\

EVENTBASE_MODULE=	Event/Base
EVENTBASE_FILES	=	Event.cpp			\
					EventHandlers.cpp	\

EVENTTYPE_MODULE=	Event/Type
EVENTTYPE_FILES	=	ClientEvent.cpp		\
					ListenEvent.cpp		\

SERVER_MODULE	=	Server
SERVER_FILES	=	CGI.cpp				\
					Epoll.cpp			\
					HttpMessage.cpp		\
					HttpRequest.cpp		\
					HttpResponse.cpp	\
					Server.cpp			\
					Socket.cpp			\

UTILS_MODULE	=	Util
UTILS_FILES		=	Logger.cpp			\
					IO.cpp				\

ALL_MODULES		:=	$(CONFIG_MODULE)	\
					$(EVENTBASE_MODULE)	\
					$(EVENTTYPE_MODULE)	\
					$(HTTP_MODULE)		\
					$(SERVER_MODULE)	\
					$(SOCKET_MODULE)	\
					$(UTILS_MODULE)		\

ALL_FILES		:=	$(MAIN_FILES)											\
					$(addprefix $(CONFIG_MODULE)/, $(CONFIG_FILES))			\
					$(addprefix $(EVENTBASE_MODULE)/, $(EVENTBASE_FILES))	\
					$(addprefix $(EVENTTYPE_MODULE)/, $(EVENTTYPE_FILES))	\
					$(addprefix $(HTTP_MODULE)/, $(HTTP_FILES))				\
					$(addprefix $(SERVER_MODULE)/, $(SERVER_FILES))			\
					$(addprefix $(SOCKET_MODULE)/, $(SOCKET_FILES))			\
					$(addprefix $(UTILS_MODULE)/, $(UTILS_FILES))			\

INCLUDE_DIRS	:=	$(INC_DIR)									\
					$(INC_DIR)/Wrapper							\
					$(addprefix $(INC_DIR)/, $(ALL_MODULES))	\

INCLUDE_FLAGS	:=	$(addprefix -I , $(INCLUDE_DIRS))

SRC				:=	$(addprefix $(SRC_DIR)/, $(ALL_FILES))
OBJ				:=	$(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
DEP				:=	$(OBJ:.o=.d)

all : $(NAME)

$(NAME) : $(OBJ)
	@ echo "${BLUE}$(NAME) compiling...${RESET}"
	$(CXX) $(CXXFLAGS) $^ -o $@
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