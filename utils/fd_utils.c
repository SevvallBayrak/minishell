/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbayrak <sbayrak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 05:27:21 by sbayrak           #+#    #+#             */
/*   Updated: 2025/07/27 04:15:16 by sbayrak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	restore_std_fds(int in, int out)
{
	dup2(in, STDIN_FILENO);
	dup2(out, STDOUT_FILENO);
	close(in);
	close(out);
}

int	prepare_stdin_for_heredoc(t_cmd *cmd, t_data *data)
{
	int	hd_fd;

	if(!cmd->argv)
		return(1); 
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

void	handle_exec_error(int result, char **envp)
{
	if (result == PATH_NO_PERMISSION)
		write(2, " Permission denied\n", 18);
	else
		write(2, " command not found\n", 19);
	free_argv(envp);
	exit(result);
}

int	print_no_red_next_word_error(t_token *next)
{
	if (!next || next->type != T_WORD)
	{
		write(2,
			"minishell: syntax error near unexpected token `newline'\n",
			57);
		return (0);
	}
	return (1);
}

static void free_cmd_redirection(t_cmd *cmd)
{
    if (cmd->infile)
        free(cmd->infile);
	else if (cmd->outfile)
    	free(cmd->outfile);
	else if (cmd->outfile)
    	free(cmd->outfile);       
	else if (cmd->heredoc_delim)
    	free(cmd->heredoc_delim);
}

void    assign_redirection(t_cmd *cmd, t_token *tok)
{
    if (tok->type == T_REDIR_IN)
    {
		free_cmd_redirection(cmd);
        cmd->infile = ft_strdup(tok->next->value); // Sonra yenisini ata
    }
    else if (tok->type == T_REDIR_OUT)
    {
		free_cmd_redirection(cmd);
        cmd->outfile = ft_strdup(tok->next->value);
        cmd->append = 0;
    }
    else if (tok->type == T_REDIR_APPEND)
    {
		free_cmd_redirection(cmd);
        cmd->outfile = ft_strdup(tok->next->value);
        cmd->append = 1;
    }
    else if (tok->type == T_HEREDOC)
    {
		free_cmd_redirection(cmd);
        cmd->heredoc_delim = ft_strdup(tok->next->value);
        cmd->is_heredoc = 1;
    }
}
