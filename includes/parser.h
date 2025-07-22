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
int handle_quote(char *input, t_token **tokens, int i, t_data *data);
int handle_redirection(char *input, t_token **tokens, int i, t_data *data);
t_token *lexer(char *input, t_data *data);
t_token *create_token(char *value, int type, int quote);
int dispatch_lexer(char *input, int i, t_token **tokens, t_data *data);
void	free_token_list(t_token *list);
t_cmd *parse_tokens(t_token *tokens);
void	free_str_array(char **arr);
t_cmd	*init_new_cmd(void);
void expand_tokens(t_env *env, t_token *tokens, int exit_status);
int check_unclosed_quotes(const char *str);
int	validate_syntax(t_token *tok, t_data *data);
int odd_or_double_quote(char quote);


void	add_token(t_token **head, char *value,
			t_token_type type, int q_type);

int		handle_quote(char *input, t_token **tokens,
			int i, t_data *data);
int		handle_redirection(char *input, t_token **tokens,
			int i, t_data *data);
int		handle_word(char *input, t_token **tokens, int i);
int		handle_pipe_token(t_cmd **current);
int		handle_redirection_token(t_cmd *cmd, t_token **tok);
int		print_no_red_next_word_error(t_token *next);
char	**append_str_to_array(char **arr, char *new_str);
t_cmd	*parse_tokens(t_token *tokens);
t_token	*lexer(char *input, t_data *data);





#endif