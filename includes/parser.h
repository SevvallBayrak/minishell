#ifndef PARSER_H
# define PARSER_H

# include "minishell.h"

int	check_unclosed_quotes(const char *str);
char	**tokenize_input(const char *input, t_shell_state *shell);

#endif
