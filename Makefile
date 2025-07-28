NAME        := minishell
CC          := cc
CFLAGS      := -Wall -Wextra -Werror -g -Iincludes -Ilibft
LDLIBS      := -lreadline

SRCS        := main.c \
               builtins/builtin_utils.c \
               builtins/cd.c \
               builtins/echo.c \
               builtins/env.c \
               builtins/exit.c \
               builtins/export.c \
               builtins/pwd.c \
               builtins/unset.c \
               env/env_utils.c \
               env/init_env.c \
               env/exported_vars_utils.c \
               executor/exec.c \
               executor/heredoc.c \
               executor/pipe_exec.c \
               executor/pipeline.c \
               executor/redirection.c \
               parser/expand.c \
               parser/handle.c \
               parser/lexer.c \
               parser/parse_tokens.c \
               parser/quote_handler.c \
               parser/token_handlers.c \
               parser/token_list.c \
               signal/signal_hadler.c \
               utils/check_utils.c \
               utils/error_handling.c \
               utils/fd_utils.c \
               utils/free_utils.c \
               utils/path_utils.c \
               utils/pipe_utils.c \
               utils/signal_utils.c \
               utils/utils.c

OBJS        := $(SRCS:.c=.o)

LIBFTDIR    := libft
LIBFT       := $(LIBFTDIR)/libft.a

all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(LDLIBS) -o $(NAME)

$(LIBFT):
	$(MAKE) -C $(LIBFTDIR)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(MAKE) -C $(LIBFTDIR) clean
	rm -f $(OBJS)

fclean: clean
	$(MAKE) -C $(LIBFTDIR) fclean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re