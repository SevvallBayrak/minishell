/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbayrak <sbayrak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 05:49:22 by sbayrak           #+#    #+#             */
/*   Updated: 2025/07/26 21:27:57 by sbayrak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_heredoc_and_redirects(t_cmd *cmd, t_data *data)
{
	int	hd_fd;

	if (cmd->is_heredoc)
	{
		hd_fd = handle_heredoc(cmd, data);
		if (hd_fd == -1)
			return (1);
		if (dup2(hd_fd, STDIN_FILENO) == -1)
		{
			perror("dup2");
			close(hd_fd);
			return (1);
		}
		close(hd_fd);
	}
	if (redirect_in(cmd, data) || redirect_out(cmd, data))
		return (1);
	return (0);
}

void	pipe_child_exec(t_cmd *cmd, int in_fd, int out_fd, t_data *data)
{
	int	rtn;

	if (in_fd != STDIN_FILENO)
	{
		dup2(in_fd, STDIN_FILENO);
		close(in_fd);
	}
	if (out_fd != STDOUT_FILENO)
	{
		dup2(out_fd, STDOUT_FILENO);
		close(out_fd);
	}
	if (handle_heredoc_and_redirects(cmd, data))
	{
		exit_cleanup(data);
		exit(1);
	}
	if (is_builtin(cmd->argv[0]))
	{
    	rtn = exec_builtin(cmd, data);
    	exit_cleanup(data); 
    	exit(rtn);
	}
	rtn = execute_command(cmd, cmd->argv, data);
	exit_cleanup(data);
	exit(rtn);
}

void	start_pipeline_child(t_cmd *cmd, int *pipefd, int in_fd, t_data *data)
{
	int	out_fd;

	if (cmd->next)
		close(pipefd[0]);
	if (cmd->next)
		out_fd = pipefd[1];
	else
		out_fd = STDOUT_FILENO;
	pipe_child_exec(cmd, in_fd, out_fd, data);
}
