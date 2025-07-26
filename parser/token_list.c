/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_list.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbayrak <sbayrak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 06:41:59 by sbayrak           #+#    #+#             */
/*   Updated: 2025/07/26 23:23:46 by sbayrak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*create_token(char *value, int type, int quote)
{
	t_token	*new;

	new = ft_calloc(sizeof(t_token), sizeof(t_token));
	if (!new)
		return (NULL);
	new->value = ft_strdup(value);
	new->type = type;
	new->quote_type = quote;
	new->next = NULL;
	return (new);
}

void	add_token(t_token **head, char *value, t_token_type type, int q_type)
{
	t_token	*new_token;
	t_token	*last;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return ;
	new_token->value = ft_strdup(value);
	new_token->type = type;
	new_token->quote_type = q_type;
	new_token->next = NULL;
	if (!*head)
		*head = new_token;
	else
	{
		last = *head;
		while (last->next)
			last = last->next;
		last->next = new_token;
	}
}

void	free_token_list(t_token *list)
{
	t_token	*temp;

	while (list)
	{
		temp = list->next;
		free(list->value);
		free(list);
		list = temp;
	}
}

void	free_str_array(char **arr)
{
	int	i;

	i = 0;
	if (!arr)
		return ;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

t_cmd	*init_new_cmd(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	ft_bzero(cmd, sizeof(t_cmd));
	cmd->infile = NULL;
	cmd->outfile = NULL;
	cmd->heredoc_delim = NULL;
	return (cmd);
}
