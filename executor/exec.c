/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbayrak <sbayrak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 05:22:07 by sbayrak           #+#    #+#             */
/*   Updated: 2025/07/26 05:47:35 by sbayrak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	run_child_process(t_cmd *cmd, char **argv, char **envp, t_data *data)
{
	int		path_result;
	char	*path;

	path = get_command_path(argv[0], data, &path_result);
	if (redirect_in(cmd, data) || redirect_out(cmd, data))
	{
		free_argv(envp);
		free(path);
		exit(1);
	}
	if (!path)
	{
		exit_cleanup(data);
		handle_exec_error(path_result, envp);
	}
	execve(path, argv, envp);
	perror("execve");
	free(path);
	free_argv(envp);
	free_argv(argv);
	exit(126);
}

int	execute_command(t_cmd *cmd, char **argv, t_data *data)
{
	pid_t	pid;
	int		status;
	char	**envp;

	envp = env_to_envp(data->env);
	if (!envp)
		return (1);
	pid = fork();
	if (pid == 0)
		run_child_process(cmd, argv, envp, data);
	else if (pid < 0)
	{
		perror("fork");
		free_argv(envp);
		exit_cleanup(data);
		return (1);
	}
	waitpid(pid, &status, 0);
	//exit_cleanup(data);//allah belasını versin buranın
	//koda ekleyince pipelı komutlar leaksiz düzgün çalışıyor ama ls ,ls -l , < , > lı komutlarda double free hatası veriyor
	//eklemeyince de pipelı komutlar leak veriyor diğerleri düzgün çalışıyor!!
	free_argv(envp);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}

static int	execute_one_command(t_cmd *cmd, t_data *data)
{
	if (!cmd->argv)
		return (1);
	if (prepare_stdin_for_heredoc(cmd, data))
		return (1);
	if (is_builtin(cmd->argv[0]))
	{
		if (redirect_in(cmd, data) || redirect_out(cmd, data))
			return (1);
		data->exit_status = exec_builtin(cmd, data);
	}
	else
		data->exit_status = execute_command(cmd, cmd->argv, data);
	return (0);
}

static void	exec_and_restore(t_cmd *curr, t_data *data)
{
	int	in;
	int	out;

	in = dup(STDIN_FILENO);
	out = dup(STDOUT_FILENO);
	if (execute_one_command(curr, data))
	{
		restore_std_fds(in, out);
		return ;
	}
	restore_std_fds(in, out);
}

int	executor_execute(t_data *data)
{
	t_cmd	*curr;
	t_cmd	*next;

	if (!data || !data->cmds)
		return (0);
	curr = data->cmds;
	while (curr)
{
	next = curr->next;

	exec_and_restore(curr, data);
	free_cmd_list(curr);
	curr = next;
}
	data->cmds = NULL;
	return (0);
}
