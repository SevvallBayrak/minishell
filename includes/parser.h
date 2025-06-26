#ifndef PARSER_H
# define PARSER_H

typedef enum e_token_type
{
    WORD,
    PIPE,
    REDIR_IN,     // <
    REDIR_OUT,    // >
    APPEND,       // >>
    HEREDOC       // <<
} t_token_type;

typedef struct s_token
{
    t_token_type   type;
    char           *value;
    struct s_token *next;
} t_token;

int check_unclosed_quotes(const char *line);
t_token	*new_token(t_token_type type, const char *value);
#endif
