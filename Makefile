NAME = Server
CXX = c++
CXXFLAGS = -std=c++98 -MMD -MP -pedantic
SRCS = Server.cpp Main.cpp ClientData.cpp ClientAuth.cpp CmdResponse.cpp Command.cpp
DEPFILES = $(OBJS:%.o=%.d)
OBJS = ${addprefix $(OBJS_DIR)/,$(SRCS:.cpp=.o)}
OBJS_DIR = objs
RM = rm -f
.DEFAULT_GOAL :=$(NAME)
all: $(NAME)

# YERROW = \033[1;33m
$(NAME): $(OBJS)
	@$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS)
	@echo ""
	@echo "███████╗████████╗         ██╗██████╗  ██████╗"
	@echo "██╔════╝╚══██╔══╝         ██║██╔══██╗██╔════╝"
	@echo "█████╗     ██║            ██║██████╔╝██║"     
	@echo "██╔══╝     ██║            ██║██╔══██╗██║"     
	@echo "██║        ██║   ███████╗ ██║██║  ██║╚██████╗"
	@echo "╚═╝        ╚═╝   ╚══════╝ ╚═╝╚═╝  ╚═╝ ╚═════╝\n"
                
$(OBJS_DIR)/%.o: %.cpp | $(OBJS_DIR)
	@$(CXX) $(CXXFLAGS) -c $< -I include -o $@
$(OBJS_DIR):
	@mkdir $(OBJS_DIR)
clean:
	@$(RM) $(OBJS)

fclean: clean
	@$(RM) $(NAME)

re: fclean all

debug: CXXFLAGS +=-fsanitize=address,undefined -g
debug: re

.PHONY: all clean fclean re
-include $(DEPFILES)
