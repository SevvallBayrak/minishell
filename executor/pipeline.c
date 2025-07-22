#include "minishell.h"
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdio.h>

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

static int	handle_heredoc_and_redirects(t_cmd *cmd, t_data *data)
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
		exit(1);
	if (is_builtin(cmd->argv[0]))
		exit(exec_builtin(cmd, data));
	exit(execute_command(cmd, cmd->argv, data));
}

void	close_pipe_ends(int *pipefd)
{
	close(pipefd[0]);
	close(pipefd[1]);
}

static int	count_commands(t_cmd *cmds)
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

static int	cleanup_on_error(pid_t *pids, int i, int in_fd)
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

static int	wait_all_children(pid_t *pids, int num_cmds)
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
static void	start_pipeline_child(t_cmd *cmd, int *pipefd, int in_fd, t_data *data)
{
	int	out_fd;

	if (cmd->next)
		close(pipefd[0]);
	out_fd = cmd->next ? pipefd[1] : STDOUT_FILENO;
	pipe_child_exec(cmd, in_fd, out_fd, data);
}

static int	handle_pipeline_iteration(
	t_cmd *curr, t_data *data, pid_t *pids,
	int *i, int *in_fd)
{
	int		pipefd[2];
	pid_t	pid;

	if (curr->next && pipe(pipefd) == -1)
		return (perror("pipe"), cleanup_on_error(pids, *i, *in_fd));
	pid = fork();
	if (pid == -1)
		return (perror("fork"), cleanup_on_error(pids, *i, *in_fd));
	if (pid == 0)
		start_pipeline_child(curr, pipefd, *in_fd, data);
	pids[(*i)++] = pid;
	if (*in_fd != STDIN_FILENO)
		close(*in_fd);
	if (curr->next)
	{
		close(pipefd[1]);
		*in_fd = pipefd[0];
	}
	return (0);
}
static int	run_pipeline_loop(t_cmd *cmds, t_data *data, pid_t *pids)
{
	int		i;
	int		in_fd;
	t_cmd	*curr;

	i = 0;
	in_fd = STDIN_FILENO;
	curr = cmds;
	while (curr)
	{
		if (handle_pipeline_iteration(curr, data, pids, &i, &in_fd))
			return (1);
		curr = curr->next;
	}
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
		return (1);
	status = wait_all_children(pids, num_cmds);
	free(pids);
	data->exit_status = status;
	return (0);
}
