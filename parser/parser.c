#include "minishell.h"
#include "utils.h"
#include "parser.h"

int	print_no_red_next_word_error(t_token *next)
{
	if (!next || next->type != T_WORD)
	{
		write(2,
			"minishell: syntax error near unexpected token `newline'\n",
			57);
		return (0);
	}
	return (1);
}

int	handle_redirection_token(t_cmd *cmd, t_token **tok)
{
	t_token	*next;

	next = (*tok)->next;
	if (!(print_no_red_next_word_error(next)))
		return (0);
	if ((*tok)->type == T_REDIR_IN)
		cmd->infile = ft_strdup(next->value);
	else if ((*tok)->type == T_REDIR_OUT)
	{
		cmd->outfile = ft_strdup(next->value);
		cmd->append = 0;
	}
	else if ((*tok)->type == T_REDIR_APPEND)
	{
		cmd->outfile = ft_strdup(next->value);
		cmd->append = 1;
	}
	else if ((*tok)->type == T_HEREDOC)
	{
		cmd->heredoc_delim = ft_strdup(next->value);
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
	int		len;
	int		i;
	char	**new_arr;

	len = 0;
	i = -1;
	if (arr)
		while (arr[len])
			len++;
	new_arr = malloc(sizeof(char *) * (len + 2));
	if (!new_arr)
		return (NULL);
	while (++i < len)
		new_arr[i] = ft_strdup(arr[i]);
	new_arr[len] = ft_strdup(new_str);
	new_arr[len + 1] = NULL;
	free_argv(arr);
	return (new_arr);
}

static int	process_token(t_token **tokens, t_cmd **current)
{
	if ((*tokens)->type == T_WORD)
	{
		if (ft_strlen((*tokens)->value) == 0)
		{
			*tokens = (*tokens)->next;
			return (1);
		}
		(*current)->argv = append_str_to_array((*current)->argv,
			(*tokens)->value);
	}
	else if ((*tokens)->type == T_PIPE)
	{
		if (!handle_pipe_token(current))
			return (0);
	}
	else if ((*tokens)->type >= T_REDIR_IN && (*tokens)->type <= T_HEREDOC)
	{
		if (!(handle_redirection_token(*current, tokens)))
			return (0);
		return (1);
	}
	*tokens = (*tokens)->next;
	return (1);
}

t_cmd	*parse_tokens(t_token *tokens)
{
	t_cmd	*cmds;
	t_cmd	*current;

	cmds = NULL;
	current = NULL;
	while (tokens)
	{
		if (!current)
		{
			current = init_new_cmd();
			if (!current)
				return (NULL);
			cmds = current;
		}
		if (!process_token(&tokens, &current))
			return (NULL);
	}
	return (cmds);
}
