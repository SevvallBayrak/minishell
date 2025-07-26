/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbayrak <sbayrak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 05:34:45 by sbayrak           #+#    #+#             */
/*   Updated: 2025/07/26 23:32:45 by sbayrak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

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

static void	write_and_free_heredoc(char *expanded, int *pipefd, char *line)
{
	write(pipefd[1], expanded, ft_strlen(expanded));
	write(pipefd[1], "\n", 1);
	free(line);
	free(expanded);
}

static void chil_heredoc(t_data *data, int *pipefd, t_cmd *cmd)
{
    char    *line;
    char    *expanded;

    signal(SIGINT, SIG_DFL);
    close(pipefd[0]);
    while (1)
    {
        line = readline("> ");
        if (!line || ft_strncmp(line, cmd->heredoc_delim,
                ft_strlen(cmd->heredoc_delim) + 1) == 0)
        {
            free(line); // Döngüden çıkarken line'ı free etmeyi unutma!
            break ;
        }
        // Genişletme kontrolü, cmd yapısındaki bir flag'den gelmeli.
        // Örneğin: if (cmd->is_expandable)
        if (is_expandable_delimiter(cmd->heredoc_delim)) // Örnek kontrol
            expanded = expand_variable(data, line, 0);
        else
            expanded = ft_strdup(line);
        write_and_free_heredoc(expanded, pipefd, line);
    }
}


static int	parent_heredoc(pid_t pid, int *pipefd)
{	
	int		status;

	signal(SIGINT, SIG_IGN); // main shell etkilenmesin
	waitpid(pid, &status, 0);
	signal(SIGINT, SIG_DFL);
	close(pipefd[1]);
	if (WIFSIGNALED(status))
		return (close(pipefd[0]), -1);
	return (pipefd[0]); // STDIN için bu fd kullanılacak
}

int handle_heredoc(t_cmd *cmd, t_data *data)
{
    int     pipefd[2];
    pid_t   pid;

    if (pipe(pipefd) == -1)
        return (perror("pipe"), -1);
    pid = fork();
    if (pid == -1)
        return (perror("fork"), -1);
    if (pid == 0)
    {
        chil_heredoc(data, pipefd, cmd);
        close(pipefd[1]);
		exit_cleanup(data);      
        exit(0);
    }
    else // PARENT
        return (parent_heredoc(pid, pipefd));
}
