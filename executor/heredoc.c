/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbayrak <sbayrak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 05:34:45 by sbayrak           #+#    #+#             */
/*   Updated: 2025/07/26 06:40:39 by sbayrak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// ## Norm düzeltirken heredoc bozulmuş bu normsuz çalışan versiyonu ## 
#include <minishell.h>

int	is_expandable_delimiter(const char *delim)
{
	size_t	len;

	if (!delim)
		return (0);
	len = ft_strlen(delim);
	if ((delim[0] == '\'' && delim[len - 1] == '\'') ||
		(delim[0] == '\"' && delim[len - 1] == '\"'))
		return (0);
	return (1);
}

int	handle_heredoc(t_cmd *cmd, t_data *data)
{
	int		pipefd[2];
	pid_t	pid;
	int		status;
    t_token  *tmp;

	if (pipe(pipefd) == -1)
		return (perror("pipe"), -1);
	pid = fork();
	if (pid == -1)
		return (perror("fork"), -1);

	if (pid == 0) // CHILD
	{
		char *line, *expanded;
		signal(SIGINT, SIG_DFL); // heredoc kırılabilir olsun
		close(pipefd[0]); // read ucunu kapat
		while (1)
		{
			line = readline("> ");
			if (!line || ft_strncmp(line, cmd->heredoc_delim, ft_strlen(cmd->heredoc_delim) + 1) == 0)
				break;
            while (data->tokens->next)
            {
                tmp = data->tokens->next;
                data->tokens = tmp;
            }            
			if (data->tokens->quote_type == 0)
				expanded = expand_variable(data->env, line, data->exit_status);
			else
				expanded = ft_strdup(line);
			write(pipefd[1], expanded, ft_strlen(expanded));
			write(pipefd[1], "\n", 1);
			free(line);
			free(expanded);
		}
		free(line);
		close(pipefd[1]);
		exit(0);
	}
	else // PARENT
	{
		signal(SIGINT, SIG_IGN); // main shell etkilenmesin
		waitpid(pid, &status, 0);
		signal(SIGINT, SIG_DFL);
		close(pipefd[1]);
		if (WIFSIGNALED(status))
		{
			close(pipefd[0]);
			return (-1);
		}
		return (pipefd[0]); // STDIN için bu fd kullanılacak
	}
}
