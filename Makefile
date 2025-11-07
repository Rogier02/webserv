NAME			=	webserv

MAKEFLAGS		=	-r -R

CXX				=	c++
CXXFLAGS		=	-MMD -MP -std=c++11
CXXFLAGS		+=	-Wall -Wextra -Werror
CXXFLAGS		+=	-fsanitize=address
CXXFLAGS		+=	-g
# CXXFLAGS		+=	-D VERBOSE

LIB_TF_DIR		=	TestFramework
LIB_TF_BIN		:=	$(LIB_TF_DIR)/libTestRunner.a
LIB_TF_INC		:=	$(LIB_TF_DIR)/include

INCLUDE_DIRS	=	incl \
					$(LIB_TF_INC)
INCLUDE_FLAGS	=	$(addprefix -I , $(INCLUDE_DIRS))

SRC_DIR			=	src
SRC_FILES		=	main.cpp

CONFIG_DIR		:=	$(SRC_DIR)/Config
CONFIG_FILES	=	Config.cpp

HTTP_DIR		:=	$(SRC_DIR)/Http
HTTP_FILES		=	Http.cpp

SERVER_DIR		:=	$(SRC_DIR)/Server
SERVER_FILES	=	Server.cpp

SRC				=	$(addprefix $(SRC_DIR)/, $(SRC_FILES)) \
					$(addprefix $(CONFIG_DIR)/, $(CONFIG_FILES)) \
					$(addprefix $(HTTP_DIR)/, $(HTTP_FILES)) \
					$(addprefix $(SERVER_DIR)/, $(SRC_FILES))

OBJ_DIR			=	obj
OBJ				:=	$(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
DEP				:=	$(OBJ:%.o=%.d)

all : $(NAME)

$(NAME) : $(OBJ) $(LIB_TF_BIN)
	@ echo "${BLUE}$(NAME) compiling...${RESET}"
	$(CXX) $^ $(CXXFLAGS) $(LIB_TF_BIN) $(INCLUDE_FLAGS) -o $(NAME)
	@ echo "${GREEN}$(NAME) made!${RESET}"

$(OBJ_DIR)/%.o:$(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDE_FLAGS) -c $< -o $@

clean :
	@ rm -f $(OBJ)
	@ rm $(OBJ_DIR)
	@ echo "${CYAN}Objects removed.${RESET}"

fclean : clean
	@ rmdir $(OBJ_DIR)/$(SRC_TEST)
	@ rmdir $(OBJ_DIR)
	@ echo "${BLUE}Object folder removed.${RESET}"
	@ rm -f $(NAME)
	@ echo "${YELLOW}$(NAME) fcleaned!${RESET}"

re : fclean all

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