/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbayrak <sbayrak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 06:32:51 by sbayrak           #+#    #+#             */
/*   Updated: 2025/07/27 15:08:39 by sbayrak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	handle_token(t_token **tok, t_cmd **curr)
{
	t_token	*t;

	t = *tok;
	if (t->type == T_WORD)
		handle_word_token(t, *curr);
	else if (t->type == T_PIPE)
		return (handle_pipe_token(curr));
	else if (t->type >= T_REDIR_IN && t->type <= T_HEREDOC)
	{
		open_and_close_outfile(*curr);
		if (!handle_redirection_token(*curr, tok))
			return (0);
		return (1);
	}
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
		if (!handle_token(&tokens, &current))
			return (NULL);
		else if (tokens && tokens->type < T_REDIR_IN)
			tokens = tokens->next;
	}
	return (cmds);
}

int	init_if_needed(t_cmd **cmds, t_cmd **current)
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
