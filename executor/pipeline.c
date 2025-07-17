#include "minishell.h"
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdio.h>

int has_pipe(t_cmd *cmds)
{
    t_cmd *current = cmds;
    
	while (current)
	{
		if (current->next)
			return (1);
		current = current->next;
	}
	return (0);
}
void pipe_child_exec(t_cmd *cmd, int in_fd, int out_fd, t_data *data, char *line)
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

	if (redirect_in(cmd) || redirect_out(cmd))
		exit(1);

	if (is_builtin(cmd->argv[0]))
		exit(exec_builtin(cmd, data, line));
	else
		exit(execute_command(cmd->argv, data));
}
void close_pipe_ends(int *pipefd)
{
	close(pipefd[0]);
	close(pipefd[1]);
}
int run_pipeline(t_cmd *cmds, t_data *data, char *line)
{
	int pipefd[2];
	int in_fd = STDIN_FILENO;
	int status;
	pid_t pid;

	while (cmds)
	{
		if(cmds->next)
        {
            if(pipe(pipefd) == -1)
                return (perror("pipe"), 1);
        }
		pid = fork();
		if (pid == 0)
		{
			int out_fd = cmds->next ? pipefd[1] : STDOUT_FILENO;
			pipe_child_exec(cmds, in_fd, out_fd, data, line);
		}

		if (in_fd != STDIN_FILENO)
			close(in_fd);
		if (cmds->next)
		{
			close(pipefd[1]);
			in_fd = pipefd[0];
		}

		cmds = cmds->next;
	}

	waitpid(pid, &status, 0);
	return (0);
}