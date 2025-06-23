NAME = minishell

SRCS = srcs/main.c srcs/prompt.c srcs/parser.c

OBJS = $(SRCS:.c=.o)

LIBFT_DIR = libft
LIBFT_LIB = $(LIBFT_DIR)/libft.a


READLINE_DIR = /opt/homebrew/opt/readline
INCLUDES = -Iincludes -I$(LIBFT_DIR) -I$(READLINE_DIR)/include
READLINE = -L$(READLINE_DIR)/lib -lreadline

CC = cc
CFLAGS = -Wall -Wextra -Werror -g $(INCLUDES)

all: $(NAME)

$(NAME): $(LIBFT_LIB) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT_LIB) $(READLINE) -o $(NAME)

$(LIBFT_LIB):
	make -C $(LIBFT_DIR)

clean:
	rm -f $(OBJS)
	make clean -C $(LIBFT_DIR)

fclean: clean
	rm -f $(NAME)
	make fclean -C $(LIBFT_DIR)

re: fclean all
