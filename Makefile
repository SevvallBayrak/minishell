# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: defcode                                       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/06/24                                 #+#    #+#              #
#    Updated: 2025/06/24                                 ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME        := minishell
CC          := cc
CFLAGS      := -Wall -Wextra -Werror -Iincludes -Ilibft -g

SRCS        := $(shell find . -name "*.c" -not -path "./libft/*")
OBJS        := $(SRCS:.c=.o)

LIBFTDIR    := libft
LIBFT       := $(LIBFTDIR)/libft.a

# --------------------------------------------------

all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -lreadline -o $(NAME)
	@echo "✅ $(NAME) derlendi."

$(LIBFT):
	$(MAKE) -C $(LIBFTDIR)

clean:
	$(MAKE) -C $(LIBFTDIR) clean
	rm -f $(OBJS)
	@echo "🧹 Temizlik yapıldı."

fclean: clean
	$(MAKE) -C $(LIBFTDIR) fclean
	rm -f $(NAME)
	@echo "🧼 Derleme dosyaları silindi."

re: fclean all

.PHONY: all clean fclean re
