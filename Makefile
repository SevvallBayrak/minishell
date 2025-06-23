NAME = minishell

# Source files
SRCS = srcs/main.c srcs/expand.c srcs/parser.c srcs/prompt.c

# Object files
OBJS = $(SRCS:.c=.o)

# Compiler and flags
CC = cc
CFLAGS = -Wall -Wextra -Werror -g -Iincludes
LDFLAGS = -lreadline

# Colors
GREEN = \033[0;32m
YELLOW = \033[0;33m
RED = \033[0;31m
RESET = \033[0m

all: $(NAME)

$(NAME): $(OBJS)
	@echo "$(YELLOW)Linking $(NAME)...$(RESET)"
	@$(CC) $(OBJS) $(LDFLAGS) -o $(NAME)
	@echo "$(GREEN)$(NAME) created successfully!$(RESET)"

%.o: %.c includes/minishell.h
	@echo "$(YELLOW)Compiling $<...$(RESET)"
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "$(RED)Removing object files...$(RESET)"
	@rm -f $(OBJS)

fclean: clean
	@echo "$(RED)Removing $(NAME)...$(RESET)"
	@rm -f $(NAME)

re: fclean all

# Test with variable expansion
test: $(NAME)
	@echo "$(GREEN)Testing variable expansion...$(RESET)"
	@echo "Starting minishell test..."
	@echo 'echo "$$USER"' | ./$(NAME)
	@echo 'echo '\''$$USER'\''' | ./$(NAME)

# Debug with valgrind
debug: $(NAME)
	@echo "$(YELLOW)Running with valgrind...$(RESET)"
	valgrind --leak-check=full --track-origins=yes ./$(NAME)

# Memory test
memtest: $(NAME)
	@echo "$(YELLOW)Memory leak test...$(RESET)"
	@valgrind --leak-check=full --show-leak-kinds=all ./$(NAME) < /dev/null

.PHONY: all clean fclean re test debug memtest
