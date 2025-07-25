/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_handlers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbayrak <sbayrak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 06:15:52 by sbayrak           #+#    #+#             */
/*   Updated: 2025/07/25 06:41:20 by sbayrak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

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

int	handle_word_token(t_token *tok, t_cmd *current)
{
	if (ft_strlen(tok->value) == 0)
		return (0);
	current->argv = append_str_to_array(current->argv, tok->value);
	return (1);
}

void	open_and_close_outfile(t_cmd *cmd)
{
	int	fd;

	if (!cmd->outfile)
		return ;
	if (cmd->append)
		fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd >= 0)
		close(fd);
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
