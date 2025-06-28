#ifndef PARSER_H
# define PARSER_H
#include "minishell.h"
#include "utils.h"

// typedef enum e_token_type
// {
//     WORD,
//     PIPE,
//     REDIR_IN,     // <
//     REDIR_OUT,    // >
//     APPEND,       // >>
//     HEREDOC       // <<
// } t_token_type;

// typedef struct s_token
// {
//     t_token_type   type;
//     char           *value;
//     struct s_token *next;
// } t_token;

int	handle_word(char *input, t_token **tokens, int i);
int handle_quote(char *input, t_token **tokens, int i);

int handle_redirection(char *input, t_token **tokens, int i);
t_token *lexer(char *input);
t_token *create_token(char *value, int type);
void add_token(t_token **list, char *value, int type);

int check_unclosed_quotes(const char *str);






#endif