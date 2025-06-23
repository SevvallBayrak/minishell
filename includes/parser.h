#ifndef PARSER_H
# define PARSER_H
#include <stdbool.h>
char	**tokenize_input(const char *input);
bool	check_unclosed_quotes(const char *str);

#endif
