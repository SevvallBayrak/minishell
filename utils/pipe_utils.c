/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbayrak <sbayrak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 05:39:07 by sbayrak           #+#    #+#             */
/*   Updated: 2025/07/26 22:10:28 by sbayrak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	has_pipe(t_cmd *cmds)
{
	t_cmd	*current;

	current = cmds;
	while (current)
	{
		if (current->next)
			return (1);
		current = current->next;
	}
	return (0);
}

void	close_pipe_ends(int *pipefd)
{
	close(pipefd[0]);
	close(pipefd[1]);
}

int	count_commands(t_cmd *cmds)
{
	int		count;
	t_cmd	*temp;

	count = 0;
	temp = cmds;
	while (temp)
	{
		count++;
		temp = temp->next;
	}
	return (count);
}

int	cleanup_on_error(pid_t *pids, int i, int in_fd)
{
	int	k;

	if (in_fd != STDIN_FILENO)
		close(in_fd);
	k = 0;
	while (k < i)
	{
		waitpid(pids[k], NULL, 0);
		k++;
	}
	free(pids);
	return (1);
}

int	wait_all_children(pid_t *pids, int num_cmds)
{
	int	i;
	int	status;
	int	last_status;

	i = 0;
	last_status = 0;
	while (i < num_cmds)
	{
		waitpid(pids[i], &status, 0);
		if (i == num_cmds - 1)
		{
			if (WIFEXITED(status))
				last_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				last_status = 128 + WTERMSIG(status);
		}
		i++;
	}
	return (last_status);
}
