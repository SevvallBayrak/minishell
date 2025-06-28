#include "minishell.h"
#include "utils.h"
#include "parser.h"

int print_no_red_next_word_error(t_token *next)
{
	if (!next || next->type != T_WORD)
	{
		write(2, "minishell: syntax error near unexpected token `newline'\n", 57);
		return (0);
	}
    return (1);
}
int	handle_redirection_token(t_cmd *cmd, t_token **tok)
{
	t_token *next = (*tok)->next;

	if (!(print_no_red_next_word_error(next)))
        return(0);
	if ((*tok)->type == T_REDIR_IN)
		cmd->infile = strdup(next->value);
	else if ((*tok)->type == T_REDIR_OUT)
	{
		cmd->outfile = strdup(next->value);
		cmd->append = 0;
	}
	else if ((*tok)->type == T_REDIR_APPEND)
	{
		cmd->outfile = strdup(next->value);
		cmd->append = 1;
	}
	else if ((*tok)->type == T_HEREDOC)
	{
		cmd->heredoc_delim = strdup(next->value);
		cmd->is_heredoc = 1;
	}
	*tok = next;
	return (1);
}

int	handle_pipe_token(t_cmd **current)
{
	(*current)->next = malloc(sizeof(t_cmd));
	if (!(*current)->next)
		return (0);
	ft_bzero((*current)->next, sizeof(t_cmd));
	*current = (*current)->next;
	return (1);
} 

char	**append_str_to_array(char **arr, char *new_str)
{
	int	len = 0;
	int	i = -1;
	char	**new_arr;

	// mevcut dizinin uzunluğunu bul
	if (arr)
		while (arr[len])
			len++;

	// +1 yeni eleman +1 NULL için yer aç
	new_arr = malloc(sizeof(char *) * (len + 2));
	if (!new_arr)
		return (NULL);

	// eskileri kopyala (strdup ile!)
	while (++i < len)
	new_arr[i] = strdup(arr[i]);

	// yeniyi ekle
	new_arr[len] = strdup(new_str);
	new_arr[len + 1] = NULL;

	free_str_array(arr); // hem pointer hem içerikler temizlenir
	return (new_arr);
}

t_cmd *parse_tokens(t_token *tokens)
{
	t_cmd *cmds = NULL;
	t_cmd *current = NULL;

	while (tokens)
	{
		if (!current)
		{
			current = init_new_cmd();
			if (!current)
				return (NULL);
			cmds = current;
		}
		if (tokens->type == T_WORD)
			current->argv = append_str_to_array(current->argv, tokens->value);
		else if (tokens->type == T_PIPE)
		{
			if (!handle_pipe_token(&current))
				return (NULL);
		}
		else if (tokens->type >= T_REDIR_IN && tokens->type <= T_HEREDOC)
	        if (!(handle_redirection_token(current, &tokens))) // syntax error olduysa dur
		        return (NULL);
		tokens = tokens->next;
	}
	return (cmds);
}
