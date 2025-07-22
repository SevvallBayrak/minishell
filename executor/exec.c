#include "minishell.h"
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdio.h>

typedef enum e_path_result
{
	PATH_OK = 0,
	PATH_NOT_FOUND = 127,
	PATH_NO_PERMISSION = 126
}   t_path_result;

static char *search_path_dirs(char *path_env, char *cmd, int *result)
{
	int i = 0, j;
	char *dir;
	char *full_path;

	while (path_env[i])
	{
		while (path_env[i] == ':')
			i++;
		j = i;
		while (path_env[j] && path_env[j] != ':')
			j++;
		dir = ft_substr(path_env, i, j - i);
		full_path = ft_strjoin_path(dir, cmd);
		free(dir);

		if (access(full_path, F_OK) == 0)
		{
			if (access(full_path, X_OK) == 0)
			{
				*result = PATH_OK;
				return full_path;
			}
			*result = PATH_NO_PERMISSION;
			free(full_path);
			return NULL;
		}
		free(full_path);
		if(!path_env[j])
			break; // Eğer sonuna geldiysek döngüyü kır
		i = j + 1;
	}
	*result = PATH_NOT_FOUND;
	return NULL;
}

char *get_command_path(char *cmd, t_data *data, int *result)
{
	char *path_env;

	*result = PATH_NOT_FOUND;

	if (!cmd)
		return NULL;

	if (cmd[0] == '/' || (cmd[0] == '.' && cmd[1] == '/'))
	{
		if (access(cmd, F_OK) == 0)
		{
			if (access(cmd, X_OK) == 0)
			{
				*result = PATH_OK;
				return ft_strdup(cmd);
			}
			*result = PATH_NO_PERMISSION;
			return NULL;
		}
		return NULL;
	}

	path_env = get_env_value(data->env, "PATH");
	if (!path_env)
		return NULL;

	return search_path_dirs(path_env, cmd, result);
}

void run_child_process(t_cmd *cmd, char **argv, char **envp, t_data *data)
{
	int path_result;
	char *path = get_command_path(argv[0], data, &path_result);

	if (redirect_in(cmd) || redirect_out(cmd))
		exit(1);

	if (!path)
	{ 
		if (path_result == PATH_NO_PERMISSION)
			write(2, "Permission denied\n", 18);
		else
			write (2, "Command not found\n", 18);
		free_argv(envp);
		exit(path_result == PATH_NO_PERMISSION ? 126 : 127);
	}
	execve(path, argv, envp);
	perror("execve");
	free(path);
	free_argv(envp);
	exit(126);
}


int execute_command(t_cmd *cmd, char **argv, t_data *data)
{
	pid_t pid;
	int status;
	char **envp = env_to_envp(data->env);
	if (!envp)
		return (1);

	pid = fork();
	if (pid == 0)
		run_child_process(cmd, argv, envp, data); // ✅ cmd parametresi eklendi
	else if (pid < 0)
	{
		perror("fork");
		free_argv(envp);
		return 1;
	}

	waitpid(pid, &status, 0); 
	free_argv(envp);

	if (WIFEXITED(status))
		return WEXITSTATUS(status);
	if (WIFSIGNALED(status))
		return 128 + WTERMSIG(status);
	return 1;
}

static void restore_std_fds(int in, int out)
{
	dup2(in, STDIN_FILENO);
	dup2(out, STDOUT_FILENO);
	close(in);
	close(out);
}

static int prepare_stdin_for_heredoc(t_cmd *cmd, t_data *data)
{
	int hd_fd;

	if (!cmd->is_heredoc)
		return (0);
	hd_fd = handle_heredoc(cmd, data);
	if (hd_fd == -1)
	{
		data->exit_status = 1;
		return (1);
	}
	dup2(hd_fd, STDIN_FILENO);
	close(hd_fd);
	return (0);
}

static int execute_one_command(t_cmd *cmd, t_data *data)
{
	if (!cmd->argv)
		return (1);
	if (prepare_stdin_for_heredoc(cmd, data))
		return (1);
	if (is_builtin(cmd->argv[0]))
	{
		if (redirect_in(cmd) || redirect_out(cmd))
			return (1);
		data->exit_status = exec_builtin(cmd, data);
	}
	else
		data->exit_status = execute_command(cmd, cmd->argv, data);
	return (0);
}

int executor_execute(t_data *data)
{
	t_cmd *curr;
	t_cmd *next;
	int in, out;

	if (!data || !data->cmds)
		return (0);
	curr = data->cmds;
	while (curr)
	{
		in = dup(STDIN_FILENO);
		out = dup(STDOUT_FILENO);
		next = curr->next;
		if (execute_one_command(curr, data))
		{
			restore_std_fds(in, out);
			free_cmd_list(curr);
			curr = next;
			continue;
		}
		restore_std_fds(in, out);
		free_cmd_list(curr);
		curr = next;
	}
	data->cmds = NULL;
	return (0);
}

