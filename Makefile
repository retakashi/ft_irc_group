NAME = echoServer
CXX = c++
CXXFLAGS = -std=c++98 -MMD -MP -pedantic -fsanitize=address,undefined -g
SRCS = echoServer.cpp echoServerMain.cpp
DEPFILES = $(OBJS:%.o=%.d)
OBJS = ${addprefix $(OBJS_DIR)/,$(SRCS:.cpp=.o)}
OBJS_DIR = objs
RM = rm -f
.DEFAULT_GOAL :=$(NAME)
all: $(NAME)
$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS)
$(OBJS_DIR)/%.o: %.cpp | $(OBJS_DIR)
	$(CXX) $(CXXFLAGS) -c $< -I include -o $@
$(OBJS_DIR):
	mkdir $(OBJS_DIR)
clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

debug: CXXFLAGS +=-fsanitize=address,undefined -g
debug: re

.PHONY: all clean fclean re
-include $(DEPFILES)
