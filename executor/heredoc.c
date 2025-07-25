/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbayrak <sbayrak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 05:34:45 by sbayrak           #+#    #+#             */
/*   Updated: 2025/07/25 05:35:52 by sbayrak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_expandable_delimiter(const char *delim)
{
	size_t	len;

	if (!delim)
		return (0);
	len = ft_strlen(delim);
	if ((delim[0] == '\'' && delim[len - 1] == '\'')
		|| (delim[0] == '\"' && delim[len - 1] == '\"'))
		return (0);
	return (1);
}

static void	write_heredoc_line(t_data *data, int fd, char *line)
{
	char	*expanded;

	if (data->tokens->quote_type == 0)
		expanded = expand_variable(data->env, line, data->exit_status);
	else
		expanded = ft_strdup(line);
	write(fd, expanded, ft_strlen(expanded));
	write(fd, "\n", 1);
	free(expanded);
}

static void	cleanup_tokens(t_data *data)
{
	t_token	*current;
	t_token	*next;

	current = data->tokens;
	while (current)
	{
		next = current->next;
		free(current->value);
		free(current);
		current = next;
	}
	data->tokens = NULL;
}

static void	child_heredoc_loop(t_cmd *cmd, t_data *data, int fd)
{
	char	*line;

	signal(SIGINT, SIG_DFL);
	close(STDIN_FILENO);
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strncmp(line, cmd->heredoc_delim,
				ft_strlen(cmd->heredoc_delim) + 1) == 0)
			break ;
		cleanup_tokens(data);
		write_heredoc_line(data, fd, line);
		free(line);
	}
	free(line);
	close(fd);
	exit(0);
}

int	handle_heredoc(t_cmd *cmd, t_data *data)
{
	int		pipefd[2];
	pid_t	pid;
	int		status;

	if (pipe(pipefd) == -1)
		return (perror("pipe"), -1);
	pid = fork();
	if (pid == -1)
		return (perror("fork"), -1);
	if (pid == 0)
		child_heredoc_loop(cmd, data, pipefd[1]);
	signal(SIGINT, SIG_IGN);
	waitpid(pid, &status, 0);
	signal(SIGINT, SIG_DFL);
	close(pipefd[1]);
	if (WIFSIGNALED(status))
	{
		close(pipefd[0]);
		return (-1);
	}
	return (pipefd[0]);
}
