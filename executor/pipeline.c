/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbayrak <sbayrak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 16:32:40 by sbayrak           #+#    #+#             */
/*   Updated: 2025/07/28 16:33:30 by sbayrak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	close_file(t_cmd *curr, int *pipefd, int *in_fd)
{
	if (curr->next)
	{
		close(pipefd[1]);
		*in_fd = pipefd[0];
	}
}

static int	handle_pipeline_iteration(t_cmd *curr, t_data *data, pid_t *pids,
	int *in_fd)
{
	int		pipefd[2];
	pid_t	pid;
	int		i;

	i = 0;
	while (curr)
	{
		if (curr->next && pipe(pipefd) == -1)
			return (perror("pipe"), cleanup_on_error(pids, i, *in_fd));
		pid = fork();
		if (pid == -1)
			return (perror("fork"), cleanup_on_error(pids, i, *in_fd));
		if (pid == 0)
		{
			if (pids)
				free(pids);
			start_pipeline_child(curr, pipefd, *in_fd, data);
		}
		pids[i++] = pid;
		if (*in_fd != STDIN_FILENO)
			close(*in_fd);
		close_file(curr, pipefd, in_fd);
		curr = curr->next;
	}
	return (0);
}

static int	run_pipeline_loop(t_cmd *cmds, t_data *data, pid_t *pids)
{
	int		in_fd;
	t_cmd	*curr;

	in_fd = STDIN_FILENO;
	curr = cmds;
	if (handle_pipeline_iteration(curr, data, pids, &in_fd))
		return (1);
	return (0);
}

int	run_pipeline(t_cmd *cmds, t_data *data)
{
	int		num_cmds;
	pid_t	*pids;
	int		status;

	num_cmds = count_commands(cmds);
	if (num_cmds == 0)
		return (0);
	pids = malloc(sizeof(pid_t) * num_cmds);
	if (!pids)
		return (perror("malloc"), 1);
	if (run_pipeline_loop(cmds, data, pids))
	{
		free(pids);
		return (1);
	}
	status = wait_all_children(pids, num_cmds);
	data->exit_status = status;
	free(pids);
	return (0);
}
