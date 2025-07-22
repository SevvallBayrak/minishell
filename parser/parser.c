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

static void	assign_redirection(t_cmd *cmd, t_token *tok)
{
	if (tok->type == T_REDIR_IN)
		cmd->infile = ft_strdup(tok->next->value);
	else if (tok->type == T_REDIR_OUT)
	{
		cmd->outfile = ft_strdup(tok->next->value);
		cmd->append = 0;
	}
	else if (tok->type == T_REDIR_APPEND)
	{
		cmd->outfile = ft_strdup(tok->next->value);
		cmd->append = 1;
	}
	else if (tok->type == T_HEREDOC)
	{
		cmd->heredoc_delim = ft_strdup(tok->next->value);
		cmd->is_heredoc = 1;
	}
}

int	handle_redirection_token(t_cmd *cmd, t_token **tok)
{
	t_token	*next;

	next = (*tok)->next;
	if (!print_no_red_next_word_error(next))
		return (0);
	assign_redirection(cmd, *tok);
	*tok = next;
	return (1);
}

int	handle_pipe_token(t_cmd **current)
{
	t_cmd	*new;

	new = malloc(sizeof(t_cmd));
	if (!new)
		return (0);
	ft_bzero(new, sizeof(t_cmd));
	(*current)->next = new;
	*current = new;
	return (1);
}

char	**append_str_to_array(char **arr, char *new_str)
{
	char	**new_arr;
	int		len;
	int		i;

	len = 0;
	i = -1;
	while (arr && arr[len])
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
static int	init_if_needed(t_cmd **cmds, t_cmd **current)
{
	if (!(*current))
	{
		*current = init_new_cmd();
		if (!(*current))
			return (0);
		*cmds = *current;
	}
	return (1);
}

static int	handle_word_token(t_token *tok, t_cmd *current)
{
	if (ft_strlen(tok->value) == 0)
		return (0);
	current->argv = append_str_to_array(current->argv, tok->value);
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
		if (!init_if_needed(&cmds, &current))
			return (NULL);
		if (tokens->type == T_WORD)
			handle_word_token(tokens, current);
		else if (tokens->type == T_PIPE)
		{
			if (!handle_pipe_token(&current))
				return (NULL);
		}
		else if (tokens->type >= T_REDIR_IN && tokens->type <= T_HEREDOC)
		{
			if (!handle_redirection_token(current, &tokens))
				return (NULL);
		}
		tokens = tokens->next;
	}
	return (cmds);
}
