/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbayrak <sbayrak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 05:31:16 by sbayrak           #+#    #+#             */
/*   Updated: 2025/07/28 17:44:28 by sbayrak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	update_result(char *path, int *result)
{
	if (access(path, F_OK) == 0)
	{
		if (access(path, X_OK) == 0)
		{
			*result = PATH_OK;
			return (1);
		}
		*result = PATH_NO_PERMISSION;
	}
	return (0);
}

char	*search_path_dirs(char *path_env, char *cmd, int *result)
{
	int		i;
	int		j;
	char	*dir;
	char	*full_path;

	i = 0;
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
		if (update_result(full_path, result))
			return (full_path);
		free(full_path);
		if (!path_env[j])
			break ;
		i = j + 1;
	}
	*result = PATH_NOT_FOUND;
	return (NULL);
}

char	*handle_absolute_cmd(char *cmd, int *result)
{
	if (access(cmd, F_OK) == 0)
	{
		if (access(cmd, X_OK) == 0)
		{
			*result = PATH_OK;
			return (ft_strdup(cmd));
		}
		*result = PATH_NO_PERMISSION;
	}
	return (NULL);
}

char	*get_command_path(char *cmd, t_data *data, int *result)
{
	char	*path_env;
	char	*found_path;

	*result = PATH_NOT_FOUND;
	if (!cmd)
		return (NULL);
	if (cmd[0] == '/' || (cmd[0] == '.' && cmd[1] == '/'))
	{
		if (access(cmd, F_OK) == 0)
		{
			if (access(cmd, X_OK) == 0)
			{
				*result = PATH_OK;
				return (ft_strdup(cmd));
			}
			*result = PATH_NO_PERMISSION;
		}
		return (NULL);
	}
	path_env = get_env_value(data->env, "PATH");
	if (!path_env)
		return (NULL);
	found_path = search_path_dirs(path_env, cmd, result);
	return (found_path);
}

int	print_no_red_next_word_error(t_token *next)
{
	if (!next || next->type != T_WORD)
	{
		write(2, "minishell: syntax error near unexpected token `newline'\n",
			57);
		return (0);
	}
	return (1);
}
